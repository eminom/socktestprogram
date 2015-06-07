
#include "config.h"
#include "SockSession.h"
#include <iostream>
#include <boost/bind.hpp>
#include <boost/make_shared.hpp>
#include <cstdlib>
#include "io/StreamBuffer.h"

SockSession::SockSession(boost::asio::io_service &io_service, const std::string &host, const std::string &port, const std::string &descr)
	:io_service_(io_service)
	,socket_(io_service)
	,timeout_(3)
	,connecting_timeout_(false)
	,socket_failed_(false)
	,host_(host)
	,port_(port)
	,description_(descr)
{
#ifdef _CltVerbose2
	std::cout<<"SockSession:"<<descr<<std::endl;
#endif
}

SockSession::~SockSession()
{
#ifdef _CltVerbose2
	std::cout<<"~SockSession:"<<description_<<std::endl;
#endif
}

SockSession* SockSession::create(boost::asio::io_service &io, const std::string &host, const std::string &port, const std::string &description)
{
	//return boost::make_shared(new SockSession(io));
	return new SockSession(io, host, port, description);
}

void SockSession::start()
{
	//Socket not initialized. 
	// Connect first. 
	socket_.set_option(boost::asio::ip::tcp::acceptor::linger(false,0));
	socket_.set_option(boost::asio::socket_base::keep_alive(true));
	boost::system::error_code error;
	//handle_read_header(error);
}

void SockSession::setDestination(const std::string &host, const std::string &port){
	host_ = host;
	port_ = port;
}

void SockSession::connect(const char *serverID)
{
	boost::asio::ip::tcp::resolver resolver(socket_.get_io_service());
	boost::asio::ip::tcp::resolver::query query(host_, port_);
	boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	boost::asio::ip::tcp::endpoint ep = *endpoint_iterator;
	connectingServerStr_ = serverID ? serverID : "";
	//boost::shared_ptr<SockSession> me = shared_from_this();	//
	socket_.async_connect(ep, 
		boost::bind(&SockSession::handle_connected, shared_from_this(), 
		boost::asio::placeholders::error/*, endpoint_iterator*/));
	flushOpTime();	//manually. silly but ok. 
}

void SockSession::close()
{
	io_service_.post(boost::bind(&SockSession::handle_closed, shared_from_this()));
}

void SockSession::readStart(){
	boost::asio::async_read(socket_,
		boost::asio::buffer(headLength_),
		boost::bind(&SockSession::handle_headerRead, shared_from_this(),
		boost::asio::placeholders::error)
	);
}

void SockSession::readBody(){
	//Type safe: attention to the template. 
	char *ptr = &buffer_[0];
	boost::asio::async_read(socket_,
		boost::asio::buffer(ptr, buffer_.size()),
		boost::bind(&SockSession::handle_bodyRead, shared_from_this(),
		boost::asio::placeholders::error)
	);
}

void SockSession::handle_headerRead(const boost::system::error_code &error){
	if (error) {
		socket_failed_ = true;
		return;
	}
	int length = 0;
	StreamBuffer is(&headLength_[0], headLength_.size());
	is.readInt32(length);
#ifdef _CltVerbose2
	std::cout<<"In Comming("<<length<<")bytes.";
#endif
	buffer_.resize(length - 4);
	readBody();
}

void SockSession::handle_bodyRead(const boost::system::error_code &error){
	if(error){
		socket_failed_ = true;
		return;
	}
	std::string payload;
	if(buffer_.size() > 4 )	{
		payload = std::string(&buffer_[4], buffer_.size() - 4);
	}
	int typecode = 0;
	StreamBuffer is(&buffer_[0], 4);
	is.readInt32(typecode);
#ifdef _CltVerbose2
	std::cout<<" : typecode["<<typecode<<"]"<<std::endl;
#endif
	auto L = LuaScript::instance()->getLuaState();
	callback_(L, typecode, payload.c_str(), payload.size());
	//std::cout<<buffer_<<std::endl;
	readStart();
}

//on connected
void SockSession::handle_connected(const boost::system::error_code &error/*, boost::asio::ip::tcp::resolver::iterator ep_iterator*/)
{
	try{
		if(!error){
			auto L = LuaScript::instance()->getLuaState();
			onConnected_(L, connectingServerStr_.c_str());
		} else {
			std::cerr<<"Fail to connect!"<<std::endl;
			connecting_timeout_ = true;
		}
		readStart();
	}
	catch(std::exception&ex){
		std::cerr<<"Error"<<std::endl;
	}
	catch(...){
		std::cerr<<"General exception"<<std::endl;
	}
}

void SockSession::handle_closed()
{
	try{
		socket_.close();
	}
	catch(std::exception &e){
		//std::cerr<<"error"<<e<<std::endl;
		std::cerr<<"error closing"<<std::endl;
	}
	catch(...){
		std::cerr<<"error closing"<<std::endl;
	}
}
	
void SockSession::handle_written(const boost::system::error_code &error, 
	std::size_t byteWritten, std::string *buffer)
{
	delete buffer;
	if(error){
		std::cerr<<"Error writing socket"<<std::endl;
		close();	 /// initiate a close
	}
}

void SockSession::write(int typeCode, const char *msg, int rlength)
{
	try{
		std::string *buffer = new std::string;
		int length = rlength + 4 + 4;
		buffer->resize(length);
		memset(&(*buffer)[0], 0, length * sizeof(char));
#ifdef _CltVerbose2
		std::cout<<"Writing to server:"<<"typecode:"<<typeCode<<" bytes = "<<length<<std::endl;
#endif
		*(int*)&((*buffer)[0]) = length;    //local seq.
		*(int*)&((*buffer)[4]) = typeCode;	//type code
		std::string &ref = *buffer;
		if(rlength>0){
			memcpy(&ref[8], &msg[0], rlength);
		}
		boost::asio::async_write(socket_, boost::asio::buffer(*buffer, buffer->size()),
		boost::bind(&SockSession::handle_written, shared_from_this(),
				boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred,
				buffer
			)
		);
		flushOpTime();	//manually. Silly but ok. 
	}
	catch(std::exception &exception){
		std::cerr<<"Error writing:"<<exception.what()<<std::endl;
		//std::cerr<<exception<<std::endl;
	}
	catch(...){
		std::cerr<<"General exception"<<std::endl;
	}
}

void SockSession::flushOpTime(){
	last_op_ = boost::posix_time::microsec_clock::universal_time();
}

void SockSession::setTimeout(float s){
	timeout_ = s;
}

bool SockSession::isTimeout()const{
	auto now = boost::posix_time::microsec_clock::universal_time();
	int pass = int(timeout_ * 1000 * 1000);
	return (now-last_op_).ticks() > pass;
}


bool SockSession::isSocketFailed()const{
	return socket_failed_ || connecting_timeout_;
}

void SockSession::setNetMessagePumper(InComingBufferCallback cb){
	callback_ = cb;
}

void SockSession::setNetConnected(OnServerConnectedCallback cb){
	onConnected_ = cb;
}
