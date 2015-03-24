

#ifndef _SOCK_SESSION_MANAGER__DEF__
#define _SOCK_SESSION_MANAGER__DEF__

#include "SessionProtocol.h"
#include "SockSession.h"

class SockSessionManager
{
private:
	SockSessionManager();
	~SockSessionManager();

public:
	void write(int typeCode, const char *buffer, int length);

public:
	static SockSessionManager* instance();
	void setSession(SockSessionPtr);

private:
	SockSessionPtr ptr_;
};


#endif