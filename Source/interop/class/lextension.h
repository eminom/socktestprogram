

#ifndef _L_EXTENSION_DEF_
#define _L_EXTENSION_DEF_

struct lua_State;

#ifdef __cplusplus
extern "C" {
#endif 
void *luaX_checkudata (struct lua_State *L, int ud, const char *tname);

#ifdef __cplusplus
}
#endif

#endif