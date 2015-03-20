

#ifndef _LUA_ACCESS_MACROS_DEF__
#define _LUA_ACCESS_MACROS_DEF__

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "script/lua_script.h"

#define _DeclareState()	\
	lua_State *L = LuaScript::instance()->getLuaState();\
	int top = lua_gettop(L);

#endif