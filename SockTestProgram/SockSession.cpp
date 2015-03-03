
#include "SockSession.h"
#include <iostream>
#include <boost/bind.hpp>
#include <boost/make_shared.hpp>

#include <cstdlib>

SockSession::SockSession(boost::asio::io_service &io_service)
	:io_service_(io_service)
	,socket_(io_service)
	,timeout_(3)
{
	std::cout<<"SockSession"<<std::endl;
}

SockSession::~SockSession()
{
	std::cout<<"~SockSession"<<std::endl;
}

SockSession* SockSession::create(boost::asio::io_service &io)
{
	//return boost::make_shared(new SockSession(io));
	return new SockSession(io);
}

void SockSession::start()
{
	//Socket not initialized. 
	// Connect first. 
	socket_.set_option(boost::asio::ip::tcp::acceptor::linger(true,0));
	socket_.set_option(boost::asio::socket_base::keep_alive(true));
	boost::system::error_code error;
	//handle_read_header(error);
}

void SockSession::connect()
{
	boost::asio::ip::tcp::resolver resolver(socket_.get_io_service());
	boost::asio::ip::tcp::resolver::query query("127.0.0.1", "4001");
	boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	boost::asio::ip::tcp::endpoint ep = *endpoint_iterator;

	//std::cout<<"before async-connect"<<std::endl;
	//boost::shared_ptr<SockSession> me = shared_from_this();	//
	socket_.async_connect(ep, 
		boost::bind(&SockSession::handle_connected, shared_from_this(), 
		boost::asio::placeholders::error/*, endpoint_iterator*/));
	flushOpTime();	//manually. silly but ok. 
	//std::cout<<"Site 2000A"<<std::endl;
}

void SockSession::close()
{
	io_service_.post(boost::bind(&SockSession::handle_closed, shared_from_this()));
}

void SockSession::read(){
}


//on connected
void SockSession::handle_connected(const boost::system::error_code &error/*, boost::asio::ip::tcp::resolver::iterator ep_iterator*/)
{
	try{
		if(!error){
			const char *msg[]={
				"Stan",
				"Eric",
				"Kyle",
				"Kenny"
			};
			//write("Hello");
			write( msg[rand()% (sizeof(msg)/sizeof(*msg))]);
		} else {
			std::cerr<<"Fail to connect!"<<std::endl;
		}
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
	if(buffer){
		delete buffer;
	}
	if(error){
		std::cerr<<"Error writing socket"<<std::endl;
		close();	 /// initiate a close
	}
}

void SockSession::write(const std::string &msg)
{
	try{
		std::string *buffer = new std::string;
		buffer->append(msg);
		boost::asio::async_write(socket_, boost::asio::buffer(*buffer, buffer->size()),
			boost::bind(&SockSession::handle_written, shared_from_this(),
				boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred,
				buffer
			)
		);
		flushOpTime();	//manually. silly but ok. 
	}
	catch(std::exception &exception){
		std::cerr<<"Error writing"<<std::endl;
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

bool SockSession::isTimeout(){
	auto now = boost::posix_time::microsec_clock::universal_time();
	int pass = int(timeout_ * 1000 * 1000);
	return (now-last_op_).ticks() > pass;
}
