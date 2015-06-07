

#include "SockSessionManager.h"
#include <iostream>

static SockSessionManager* _instance = 0;

SockSessionManager::SockSessionManager()
	:ptr_(0)
{
}

SockSessionManager::~SockSessionManager()
{

}

SockSessionManager* SockSessionManager::instance()
{
	if(!_instance)
	{
		_instance = new SockSessionManager;
	}
	return _instance;

}

void SockSessionManager::write(int typeCode, const char *buffer, int length)
{
	if(ptr_)
	{
		ptr_->write(typeCode, buffer, length);
	} 
	else 
	{
		std::cout<<"Error: session is not present"<<std::endl;
	}
}

//
//void SockSessionManager::setSession(SockSessionPtr session)
//{
//	ptr_ = session;
//}

void SockSessionManager::connectTo(const std::string &host, const std::string &port, const std::string &serverID, const std::string &descr)
{
	SockSessionPtr ss(SockSession::create(*io_, host, port, descr));
	ss->setTimeout(3);
	ss->setNetMessagePumper(redistribute_);
	ss->setNetConnected(serverConnected_);
	ss->connect(serverID.c_str());
	if(ptr_)
	{
		ptr_->close();
	}
	ptr_ = ss;
}

SockSessionPtr& SockSessionManager::current()
{
	return ptr_;
}

SockSessionPtr& SockSessionManager::currentSession()
{
	return instance()->current();
}

bool SockSessionManager::hasCurrentSession()
{
	return instance()->current() != nullptr;
}

void SockSessionManager::setDefaultRedistribute(InComingBufferCallback redist)
{
	redistribute_ = redist;
}

void SockSessionManager::setDefaultServerOn(OnServerConnectedCallback serverOn)
{
	serverConnected_ = serverOn;
}