
#include "sp_config.h"

#include <iostream>
#include <string>
#include <exception>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include "SockSession.h"
#include "SockSessionManager.h"

#include "script/lua_script.h"
#include "pbc.h"
#include "interop/lua_access/lua_access_macros.h"
#include "dump.h"


//#include <windows.h>
void luaopen_mm(lua_State *L);

void decodeBuffer(int typecode, const char *buffer, int bufferSize)
{
	_DeclareState()
	lua_getglobal(L, "__G_TRACEBACK");
	assert(lua_isfunction(L, -1));

	lua_getglobal(L, "redirectNetBuffer");
	assert(lua_isfunction(L, -1));

	//Type-code
	lua_pushinteger(L, 1);

	//Buffer
	luaL_Buffer b;
	luaL_buffinit(L, &b);
	int n = bufferSize;
	int rlen = LUAL_BUFFERSIZE;  /* try to read that much each time */
	int index = 0;
	do {
		char *p = luaL_prepbuffer(&b);
		if (rlen > n) rlen = n;  /* cannot read more than asked */
		memcpy(p,buffer + index,rlen);
		luaL_addsize(&b, rlen);
		index += rlen;
		n -= rlen;  /* still have to read `n' chars */
	} while (n > 0);  /* until end of count or eof */
	luaL_pushresult(&b);  /* close buffer */
	int res = lua_pcall(L, 2, 0, -4); 
	lua_pop(L, 1);
	assert( top == lua_gettop(L));
}

// Main Entry::
int main()
{
	srand((unsigned int)time(0));
	boost::asio::io_service io;
	SockSessionPtr ss(SockSession::create(io));
	ss->setCallback(decodeBuffer);
	ss->connect();
	ss->setTimeout(3);
	SockSessionManager::instance()->setSession(ss);
	luaopen_mm(LuaScript::instance()->getLuaState());
	LuaScript::instance()->loadInit("inside/init.lua");

	auto start = boost::posix_time::microsec_clock::universal_time();
	while(true)
	{
		auto now = boost::posix_time::microsec_clock::universal_time();
		auto rv = io.poll();
		if( !rv && ss->isSocketFailed()){
			break;
		}
		auto pass = (now-start).ticks();
		start = now;
		float passf = pass / 1000.0f / 1000.0f;
		GameCore::frameUpdate(passf);
	}

	//
	std::cout<<"End of program"<<std::endl;
	system("pause");
	return 0;
}