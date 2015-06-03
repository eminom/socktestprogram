
#ifndef _L_NOTIFICATION__DEF__
#define _L_NOTIFICATION__DEF__

struct lua_State;
void L_onDecodeBuffer(lua_State *L, int typecode, const char *buffer, int bufferSize);
void L_onServerConnectionEstablished(lua_State *L, const char *serverID);

#endif