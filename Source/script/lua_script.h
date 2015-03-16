

#ifndef _LUA_SCRIPT_WRAPPER_DEF__
#define _LUA_SCRIPT_WRAPPER_DEF__

struct lua_State;

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