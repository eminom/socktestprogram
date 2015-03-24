
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
#include "interop/exports/exports.h"
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

	std::string host = executeStringFunc("GetHostName", "");
	std::string port = executeStringFunc("GetHostPort", "");

	SockSessionManager::instance()->setDefaultRedistribute(decodeBuffer);
	boost::asio::io_service io;
	IoServiceOwner owner(&io, SockSessionManager::instance());
	SockSessionManager::instance()->connectTo(host, port);

	auto start = boost::posix_time::microsec_clock::universal_time();
	while(true)
	{
		auto now = boost::posix_time::microsec_clock::universal_time();
		auto rv = io.poll();
		if( !rv && SockSessionManager::currentSession()->isSocketFailed()){
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