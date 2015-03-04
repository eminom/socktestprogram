

#ifndef _SESSION_PROTOCOL__DEF__
#define _SESSION_PROTOCOL__DEF__

#include <string>

struct SockSessionProtocol
{
	virtual void start() = 0;
	virtual void close() = 0;
	virtual void write(const std::string &msg) = 0;
	virtual void read() = 0;
};

#endif
