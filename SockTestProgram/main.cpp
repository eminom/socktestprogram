
#include <iostream>
#include <string>
#include <exception>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include "SockSession.h"

int main()
{
	srand((unsigned int)time(0));
	boost::asio::io_service io;
	SockSessionPtr ss(SockSession::create(io));
	//ss->test();
	//ss.start();
	//ss->start();
	ss->connect();
	io.run();
	//io.poll();
	return 0;
}