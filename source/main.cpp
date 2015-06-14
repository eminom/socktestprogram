
#include "sp_config.h"

#include <iostream>
#include <string>
#include <exception>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include "socket/SockSession.h"
#include "socket/SockSessionManager.h"

#include "script/lua_script.h"
#include "pbc.h"
#include "interop/access/lua_access_macros.h"
#include "interop/access/lua_access.h"
#include "interop/exports/L_notification.h"
#include "dump.h"

//#include <windows.h>
#include "interop/exports/exports.h"

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

typedef std::function<void()> PrepareHandle;
typedef std::function<void()> BeforeHandle;
typedef std::function<void()> AfterHandle;
typedef std::function<void(float)> UpdateHandle;

class ConnFw
{
public:
	ConnFw(){
		prepare_ = [](){};
		before_  = [](){};
		after_   = [](){};
		update_  = [](float dt){};
	}
	void setPrepare(PrepareHandle ph){
		prepare_ = ph;
	}
	void setBefore(BeforeHandle bh){
		before_ = bh;
	}
	void setAfter(AfterHandle ah){
		after_ = ah;
	}
	void setUpdate(UpdateHandle uh){
		update_ = uh;
	}

	void run()
	{
		prepare_();
		boost::asio::io_service io;
		IoServiceOwner owner(&io, SockSessionManager::instance());
		before_();
		// master loop
		auto start = boost::posix_time::microsec_clock::universal_time();
		while (true) {
			auto now = boost::posix_time::microsec_clock::universal_time();
			auto pass = (now-start).ticks();
			start = now;
			float passf = pass / 1000.0f / 1000.0f;
			update_(passf);
			auto rv = io.poll();
			if( !rv && (SockSessionManager::hasCurrentSession() && SockSessionManager::currentSession()->isSocketFailed())){
				break;
			}
		}
		after_();
	}

private:
	PrepareHandle prepare_;
	BeforeHandle before_;
	AfterHandle after_;
	UpdateHandle update_;
};

// Main Entry::
int main() {
	srand((unsigned int)time(0));
    adaptDebugging();
	ConnFw cf;
	cf.setPrepare([](){
			luaopen_mm(LuaScript::instance()->getLuaState()); //Init scripting
			LuaScript::instance()->loadInit("app/init.lua");
			SockSessionManager::instance()->setDefaultRedistribute(L_onDecodeBuffer);
			SockSessionManager::instance()->setDefaultServerOn(L_onServerConnectionEstablished);
		});
	cf.setBefore([](){
			executeVoidFunc(LuaScript::instance()->getLuaState(), "startUp","");
		}
	);
	//cf.setBefore([](){
	//	SockSessionManager::instance()->connectTo("192.168.1.105", "11000", "Connection");
	//});
	cf.setAfter([](){
		std::cout<<"End of program"<<std::endl;
		system("pause");
	});
	cf.setUpdate([](float dt){
		GameCore::frameUpdate(dt);
	});
	cf.run();
	return 0;
}