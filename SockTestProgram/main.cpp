
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


	auto start = boost::posix_time::microsec_clock::universal_time();
	while(true){
		auto now = boost::posix_time::microsec_clock::universal_time();
		auto rv = io.poll();
		if( !rv && ss->isSocketFailed()){
			break;
		}
		if( (now-start).ticks() > 1000 * 1000 ){
			const char *either[]={"Hello, world", "Again"};
			ss->write(either[rand()&1]);
			std::cout<<"sending one"<<std::endl;
			start = now;
		}
	}
	return 0;
}