
#include "L_notification.h"
#include "interop/lua_access/lua_access_macros.h"
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstring>

void L_onDecodeBuffer(int typecode, const char *buffer, int bufferSize)
{
	_DeclareState()
	lua_getglobal(L, "__G_TRACEBACK");
	assert(lua_isfunction(L, -1));

	lua_getglobal(L, "redirectNetBuffer");
	assert(lua_isfunction(L, -1));

	//Type-code
	lua_pushinteger(L, typecode);

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
	if(res){
		lua_pop(L, 1);
	}
	lua_pop(L, 1);
	assert( top == lua_gettop(L));
}

void L_onServerConnectionEstablished(const char *serverID)
{
	_DeclareState()
	lua_getglobal(L, "__G_TRACEBACK");
	assert(lua_isfunction(L, -1));

	lua_getglobal(L, "serverConnectOn");
	assert(lua_isfunction(L, -1));

	lua_pushstring(L, serverID);
	int res = lua_pcall(L, 1, 0, -3);
	lua_pop(L, 1);
	assert( top == lua_gettop(L));
}