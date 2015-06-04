

#ifndef _SESSION_PROTOCOL__DEF__
#define _SESSION_PROTOCOL__DEF__

#include <string>

struct SockSessionProtocol
{
	virtual void start() = 0;
	virtual void close() = 0;
	virtual void write(int typeCode, const char *msg, int length) = 0;
	virtual void readStart() = 0;
};

#endif
