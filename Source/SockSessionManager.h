

#ifndef _SOCK_SESSION_MANAGER__DEF__
#define _SOCK_SESSION_MANAGER__DEF__

#include "SessionProtocol.h"
#include "SockSession.h"
#include "IoServiceOwner.h"

class SockSessionManager:public IoServiceHolder
{
private:
	SockSessionManager();
	~SockSessionManager();

public:
	void write(int typeCode, const char *buffer, int length);

private:
	virtual void registerIO(boost::asio::io_service *io)
	{
		io_ = io;
	}

	virtual void unregisterID(boost::asio::io_service *io)
	{
		io_ = nullptr;
	}

public:
	static SockSessionManager* instance();
	static SockSessionPtr& currentSession();

	//void setSession(SockSessionPtr);
	void connectTo(const std::string &host, const std::string &port);
	SockSessionPtr& current();
	void setDefaultRedistribute(InComingBufferCallback);

private:
	SockSessionPtr ptr_;
	boost::asio::io_service *io_;
	InComingBufferCallback redistribute_;
};


#endif