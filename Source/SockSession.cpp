
#include "SockSession.h"
#include <iostream>
#include <boost/bind.hpp>
#include <boost/make_shared.hpp>
#include <cstdlib>
#include "io/StreamBuffer.h"

SockSession::SockSession(boost::asio::io_service &io_service, const std::string &host, const std::string &port)
	:io_service_(io_service)
	,socket_(io_service)
	,timeout_(3)
	,connecting_timeout_(false)
	,socket_failed_(false)
	,host_(host)
	,port_(port)
{
	std::cout<<"SockSession"<<std::endl;
}

SockSession::~SockSession()
{
	std::cout<<"~SockSession"<<std::endl;
}

SockSession* SockSession::create(boost::asio::io_service &io, const std::string &host, const std::string &port)
{
	//return boost::make_shared(new SockSession(io));
	return new SockSession(io, host, port);
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

void SockSession::read(){
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
	std::cout<<"In Comming("<<length<<")bytes.";
	buffer_.resize(length - 4);
	readBody();
}

void SockSession::handle_bodyRead(const boost::system::error_code &error){
	if(error){
		socket_failed_ = true;
		return;
	}
	std::string data;
	//data.swap(buffer_);
	assert(buffer_.size() > 2);

	std::string payload(&buffer_[2], buffer_.size()-2);
	int typecode = 0;
	StreamBuffer is(&buffer_[0], 2);
	is.readInt16(typecode);
	std::cout<<" : typecode["<<typecode<<"]"<<std::endl;
	callback_(typecode, payload.c_str(), payload.size());
	//std::cout<<buffer_<<std::endl;
	read();
}

//on connected
void SockSession::handle_connected(const boost::system::error_code &error/*, boost::asio::ip::tcp::resolver::iterator ep_iterator*/)
{
	try{
		if(!error){
            /*
			const char *msg[]={
				"Stan",
				"Eric",
				"Kyle",
				"Kenny"
			};
            */
			//write("Hello");
			//write( msg[rand()% (sizeof(msg)/sizeof(*msg))]);
			onConnected_(connectingServerStr_.c_str());
		} else {
			std::cerr<<"Fail to connect!"<<std::endl;
			connecting_timeout_ = true;
		}
		read();
	}
	catch(std::exception&ex){
		std::cerr<<"Error"<<std::endl;
	}
	catch(...){
		std::cerr<<"General exception"<<std::endl;
	}
}

// on close
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
	
//on written
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
		int length = rlength + 4 + 2;
		buffer->resize(length);
		memset(&(*buffer)[0], 0, length * sizeof(char));

		std::cout<<"Writing to server:"<<"typecode:"<<typeCode<<" bytes = "<<length<<std::endl;

		//htonl(
		//~
		*(int*)&((*buffer)[0]) = length;   // local seq.
		*(unsigned short*)&((*buffer)[4]) = typeCode;	//type code
		std::string &ref = *buffer;
		memcpy(&ref[6], &msg[0], rlength);
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
