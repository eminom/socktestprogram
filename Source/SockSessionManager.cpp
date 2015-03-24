

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

void SockSessionManager::setSession(SockSessionPtr session)
{
	ptr_ = session;
}