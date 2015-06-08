

#include "dump.h"
#include "lua_script.h"
#include "interop/access/lua_access_macros.h"
#include <cassert>


/////////////
#ifdef __APPLE__
extern "C"
#endif
int luaopen_protobuf_c(lua_State *L);

static LuaScript* _instance = nullptr;

LuaScript::LuaScript(){
	init();
}

LuaScript::~LuaScript(){
	deinit();
}

void LuaScript::deinit(){
	if(L_){
		lua_close(L_);
		L_ = 0;
	}          
}

void LuaScript::init(){
	assert( ! L_ );
	L_ = luaL_newstate();
	luaL_openlibs(L_);
	luaopen_protobuf_c(L_);

	const char *traceback = 
"function __G__TRACKBACK__(msg)\n"
"	local msg = debug.traceback(msg, 3)\n"
"	print(msg)\n"
"	return msg\n"
"end\n"
;

	luaL_loadstring(L_, traceback);
	int initcode = lua_pcall(L_, 0, 0, 0);
	assert(!initcode);
}

LuaScript* LuaScript::instance()
{
	if(!_instance){
		_instance = new LuaScript;
	}
	return _instance;
}

void LuaScript::loadInit(const char *file_path)
{
	int top = lua_gettop(L_);
	lua_getglobal(L_, _GTrackBack);
	assert(lua_isfunction(L_, -1));
	luaL_loadfile(L_, file_path);
    if(!lua_isfunction(L_, -1)){
        lua_pop(L_, 2);
        printf("Loading \"%s\" failed\n", file_path);
        assert( top == lua_gettop(L_));
        return;
    }
    if( lua_pcall(L_, 0, 0, -2)){
        printf("Error loading init script in \"%s\"\n", file_path);
        lua_pop(L_, 1);
    }
	lua_pop(L_, 1);	//Balance
	assert(top==lua_gettop(L_));
}


lua_State* LuaScript::getLuaState()
{
	return L_;
}