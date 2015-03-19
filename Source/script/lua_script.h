

#ifndef _LUA_SCRIPT_WRAPPER_DEF__
#define _LUA_SCRIPT_WRAPPER_DEF__

#ifdef __cplusplus
extern "C" {
#endif

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#ifdef __cplusplus
}
#endif


class LuaScript
{
private:
	LuaScript();
	~LuaScript();

public:
	static LuaScript* instance();
	void loadInit(const char *file_path);
	lua_State* getLuaState();

private:
	void init();
	void deinit();

private:
	lua_State *L_;
};


#endif