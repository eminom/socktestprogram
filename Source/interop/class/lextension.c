
#include "lextension.h"
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include <assert.h>

void *luaX_checkudata (lua_State *L, int ud, const char *tname) {
  void *p = lua_touserdata(L, ud);
  int before = 0;
  int top = lua_gettop(L);
  if (p != NULL) {
	  luaL_getmetatable(L, tname);
	  if( !lua_istable(L, -1)){ 
		  lua_pop(L, 1);
		  luaL_typerror(L, ud, tname);
		  return 0;
	  }
	  lua_getmetatable(L, ud);	//~ dynamic
	  before = 1;
	  while(1){
		  if (!lua_istable(L, -1)) {
			  lua_pop(L, before + 1);
			  luaL_typerror(L, ud, tname);
			  return 0;
		  }
		  if (lua_rawequal(L, -1, -(before+1))) {
			lua_pop(L, before + 1);
			assert( top == lua_gettop(L) );
			return p;
		  }
		  lua_getmetatable(L, -1);
		  //luaL_getmetafield(L, -1, "__index");
		  before++;
	  }
  }
   
  // balanced ones
  luaL_typerror(L, ud, tname);
  return 0;
} 