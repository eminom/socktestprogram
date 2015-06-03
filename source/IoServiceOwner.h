

#ifndef __IO_SERVICE_OWNER__DEF
#define __IO_SERVICE_OWNER__DEF

#include <boost/asio.hpp>

struct IoServiceHolder
{
	virtual void registerIO(boost::asio::io_service *io) = 0;
	virtual void unregisterID(boost::asio::io_service *io) = 0;
};

class IoServiceOwner
{
public:
	IoServiceOwner(boost::asio::io_service *io, IoServiceHolder *holder)
		:io_(io)
		,ptr_(holder)
	{
		ptr_->registerIO(io);
	}

	~IoServiceOwner()
	{
		ptr_->unregisterID(io_);
	}

private:
	boost::asio::io_service *const io_;
	IoServiceHolder * const ptr_;
};



#endif