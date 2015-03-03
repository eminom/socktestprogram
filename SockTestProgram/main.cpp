
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
	ss->connect();
	ss->setTimeout(3);

	while(true){
		auto rv = io.poll();
		if(!rv && ss->isTimeout()){
			break;
		}
	}
	return 0;
}