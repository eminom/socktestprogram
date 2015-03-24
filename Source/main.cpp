
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
#include "interop/lua_access/lua_access.h"
#include "interop/exports/L_notification.h"
#include "dump.h"

//#include <windows.h>
void luaopen_mm(lua_State *L);

#ifdef __APPLE__
#include <unistd.h>

void adaptDebugging()
{
    char *path = getcwd(0,0);
    printf("Current working path is %s\n", path);
    free(path);
}

#else
void adaptDebugging(){}
#endif


// Main Entry::
int main()
{
	srand((unsigned int)time(0));
    adaptDebugging();

	//Init scripting
	luaopen_mm(LuaScript::instance()->getLuaState());
	LuaScript::instance()->loadInit("inside/init.lua");
	SockSessionManager::instance()->setDefaultRedistribute(L_onDecodeBuffer);
	SockSessionManager::instance()->setDefaultServerOn(L_onServerConnectionEstablished);
	boost::asio::io_service io;
	IoServiceOwner owner(&io, SockSessionManager::instance());
	//SockSessionManager::instance()->connectTo(host, port);

	executeVoidFunc("startUp","");
	auto start = boost::posix_time::microsec_clock::universal_time();
	while(true)
	{
		auto now = boost::posix_time::microsec_clock::universal_time();
		auto rv = io.poll();
		if( !rv && (SockSessionManager::hasCurrentSession() && SockSessionManager::currentSession()->isSocketFailed())){
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