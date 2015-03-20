
#include "lua_access.h"
#include "refobj.h"

RefObject::RefObject()
	:_ref(LUA_REFNIL){

}

RefObject::~RefObject(){
	ljReleaseObj(_ref);
}

RefObject::RefObject(const char *name){
	loadRef(name);
}

void RefObject::loadRef(const char *name) {
	ljReleaseObj(_ref);
	_ref = ljLoadObj(name);
	assert(_ref != LUA_REFNIL);
}

void RefObject::loadFromTop(){
	ljReleaseObj(_ref);
	
	_DeclareState()
	if( !lua_istable(L, -1)){
		printf("Not a table , sorry !\n");
		lua_pop(L, 1);   // always balance the stack
		assert(0);
		return;
	}
	_ref = luaL_ref(L, LUA_REGISTRYINDEX);
	assert(_ref != LUA_REFNIL);
}

void RefObject::loadFromFunc(const char *name){
	ljReleaseObj(_ref);
	_DeclareState()
	int traceback = 0;
	lua_getglobal(L, "__G__TRACKBACK__");
	if(lua_isfunction(L, -1)){
		traceback = -2;
	}
	lua_getglobal(L, name);
	if(!lua_isfunction(L, -1)){
		printf("No a function for %s\n", name);
		lua_pop(L, 2);
		assert( top == lua_gettop(L));
		assert(0);
		return;
	}
	if( lua_pcall(L, 0, 1, traceback)){
		printf("Error running %s\n", name);
		lua_pop(L, 2);
		assert( top == lua_gettop(L));
		assert(0);
		return;
	}
	_ref = luaL_ref(L, LUA_REGISTRYINDEX);
	lua_pop(L, 1);
	assert( top == lua_gettop(L));
	assert( _ref != LUA_REFNIL);
}

int RefObject::tableRef()const {
	return _ref;
}

void RefObject::createTableRef() {
	//~Do nothing at all.
}
