

#include "lua_script.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include <cassert>


/////////////
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
	luaL_dofile(L_, file_path);
}


lua_State* LuaScript::getLuaState()
{
	return L_;
}