
#include "lua_access.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

const char* toLuaType(lua_State *L, int index){
	int t = lua_type(L, index);
	const char *typeStr = "Unknown";
	switch(t){
	case LUA_TNIL:
		typeStr = "nil";
		break;
	case LUA_TBOOLEAN:
		typeStr = "boolean";
		break;
	case LUA_TLIGHTUSERDATA:
		typeStr = "lightuserdata";
		break;
	case LUA_TNUMBER:
		typeStr = "number";
		break;
	case LUA_TSTRING:
		typeStr = "string";
		break;
	case LUA_TTABLE:
		typeStr = "table";
		break;
	case LUA_TFUNCTION:
		typeStr = "function";
		break;
	case LUA_TUSERDATA:
		typeStr = "userdata";
		break;
	case LUA_TTHREAD:
		typeStr = "thread";
		break;
	}
	return typeStr;
}


void executeVoidFunc(const char *funcName, const char *format, ...)
{
	_ExecuteBody(0)
	lua_settop(L,top);
}


//returns the table ref id(in LUA_REGISTRYINDEX)
int executeTableFunc(const char *funcName, const char *format, ...) {
	_ExecuteBody(1, LUA_REFNIL)
	if (!lua_istable(L, -1) ) {
		printf("Is not a table !!!\n");
		assert(false);
	}
	int ref = luaL_ref(L, LUA_REGISTRYINDEX);		//make reference and pop
	return ref;
}

std::string executeStringFunc(const char *funcName, const char *format, ...)
{
	_ExecuteBody(1, "")
	if( !lua_isstring(L, -1) )
	{
		log("Error executing %s", funcName);
		lua_settop(L, top);
		return "";
	}

	const char *topStr = lua_tostring(L, -1);
	std::string rv;
	if(topStr)
	{
		rv = topStr;
	}
	lua_settop(L,top);
	return rv;
}


float executeNumberFunc(const char *funcName, const char *format, ...)
{
	_ExecuteBody(1, 0)
	if( !lua_isnumber(L,-1))
	{
		log("Error executing %s", funcName);
		lua_settop(L, top);
		return 0;
	}

	float rv = lua_tonumber(L, -1);
	lua_settop(L, top);
	return rv;
}

int executeIntegerFunc(const char *funcName, const char *format,...)
{
	_ExecuteBody(1, 0)
	if( !lua_isnumber(L,-1))
	{
		log("Error executing %s", funcName);
		lua_settop(L, top);
		return 0;
	}
	int rv = lua_tointeger(L, -1);
	lua_settop(L, top);
	return rv;
}

float executeFloatFunc(const char *funcName, const char *format,...)
{
	_ExecuteBody(1,0)
	if( !lua_isnumber(L,-1))
	{
		log("Error executing %s", funcName);
		lua_settop(L, top);
		return 0;
	}
	float rv = lua_tonumber(L, -1);
	lua_settop(L, top);
	return rv;
}

void execVoidFunc(int ref, const char *format, ...)
{
	_DeclareState()
	lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
	assert( lua_isfunction(L, -1) );
	if (!lua_isfunction(L, -1)){
		log("object on top is not a function. which is actually %s", toLuaType(L, -1));
		lua_settop(L, top);	//~Pop the top which is put on right now.
		return;
	}
	va_list args;
	va_start(args, format);
	const char *funcName = "anonymous func";
	ExecuteFunctionOnTop(args, 0)
	va_end(args);
	lua_settop(L, top);
}

//These APIs are for retaining object at C
int ljMakeTable() {
	_DeclareState()
	lua_newtable(L);
	int r = luaL_ref(L, LUA_REGISTRYINDEX);		//And the table is poped.
	return r;
}

int ljGetTableInteger(int ref, const char *name, bool *result) {
	_DeclareState()
	_get_table_field(lua_isnumber, lua_tointeger, 0)
	return rv;
}

bool ljGetTableBoolean(int ref, const char *name, bool *result) {
	_DeclareState() 
	_get_table_field(lua_isboolean, lua_toboolean, false)
	return bool(rv);
}

std::string	ljGetTableString(int ref, const char *name, bool *result) {
	_DeclareState()
	_get_table_field(lua_isstring, lua_tostring, "")
	return rv ? rv:"";
}

float ljGetTableFloat(int ref, const char *name, bool *result) {
	_DeclareState()
	_get_table_field(lua_isnumber, lua_tonumber, 0)
	return rv;
}

int ljRunObjInteger(int ref, const char *name, bool *result, const char *format, ...){
	_DeclareState()
	_get_table_field_onto_stack(lua_isfunction, 0)
	const char *funcName = name;
	va_list args;
	va_start(args, format);
	ExecuteFunctionOnTop(args, 1, 0)
	va_end(args);
	if(!lua_isnumber(L, -1)){
		log("%s return value is not number!", name);
		log("which is actually %s", toLuaType(L, -1));
		lua_settop(L, top);
		return 0;
	}
	auto rv = lua_tointeger(L, -1);
	lua_settop(L, top);
	if(result){
		*result = true;
	}
	return rv;
}

void ljRunObjVoid(int ref, const char *name, bool *result, const char *format, ...) {
	_DeclareState()
	_get_table_field_onto_stack(lua_isfunction)
	const char *funcName = name;
	va_list args;
	va_start(args, format);
	ExecuteFunctionOnTop(args, 0)
	va_end(args);
	if(result){
		*result = true;
	}
	lua_settop(L, top);
}

void ljRunObjVoidSelfUserData(int ref, const char *name, bool *result, void *p) {
	_DeclareState()
	_get_table_field_onto_stack(lua_isfunction)
	assert(lua_istable(L, -2));
	lua_pushvalue(L, -2);
	const char *funcName = name;
	lua_pushlightuserdata(L, p);
	int parameterCount = 2;
	int traceback = 0;
	int funcIndex = -(parameterCount+1);
    lua_getglobal(L, "__G__TRACKBACK__");
    if (!lua_isfunction(L, -1))
    {
        lua_pop(L, 1);
    }
    else
    {
        lua_insert(L, funcIndex - 1);
        traceback = funcIndex - 1;
    }
	if( lua_pcall(L, parameterCount, 0, traceback) )
	{
		log("Error executing %s", funcName);
		lua_settop(L,top);
		return;
	}
	if(result){
		*result = true;	//Finally.
	}
	lua_settop(L,top);
	return;
}

int ljDuplicateObj(int ref) {
	if (LUA_REFNIL == ref ){
		return LUA_REFNIL;
	}
	_DeclareState()
	lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
	return luaL_ref(L, LUA_REGISTRYINDEX);
}

void ljReleaseObj(int &ref){
	if (LUA_REFNIL == ref) {
		return;
	}
	_DeclareState()
	luaL_unref(L, LUA_REGISTRYINDEX, ref);
	ref = LUA_REFNIL;
}

int ljLoadObj(const char *name){
	_DeclareState()
	lua_getglobal(L,name);
	if( !lua_istable(L, -1)){
		log("%s is not a table", name);
		lua_settop(L, top);
		return LUA_REFNIL;
	}
	int rv = luaL_ref(L, LUA_REGISTRYINDEX);
	assert( top == lua_gettop(L));
	return rv;
}

int ljCreateTableFromFuncRef(int ref, int retvals, int(*checker)(lua_State *L, int n)=nullptr) {
	_DeclareState()
	lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
	if (!lua_isfunction(L,-1)){
		log("Ref %d is not a function", ref);
		lua_settop(L, top);
		return LUA_REFNIL;
	}
	//considering add some parameters
	int numArgs = 0;
    int functionIndex = -(numArgs + 1);
    if (!lua_isfunction(L, functionIndex))  {
        log("value at stack [%d] is not function", functionIndex);
        lua_settop(L, top);
        return LUA_REFNIL;
    }
    int traceback = 0;
    lua_getglobal(L, "__G__TRACKBACK__");                         /* L: ... func arg1 arg2 ... G */
    if (!lua_isfunction(L, -1)) {
        lua_pop(L, 1);                                            /* L: ... func arg1 arg2 ... */
    } else  {
        lua_insert(L, functionIndex - 1);                         /* L: ... G func arg1 arg2 ... */
        traceback = functionIndex - 1;
    }
    int error = 0;
    error = lua_pcall(L, numArgs, 1, traceback);                  /* L: ... [G] ret */
    if (error) {
        if (traceback == 0) {
            log("[LUA ERROR] %s", lua_tostring(L, - 1));        /* L: ... error */
        }
		lua_settop(L, top);
        return LUA_REFNIL;
    }

	if( 1 == retvals){
		if( !checker(L, -1)){
			log("return value is not a table");
			lua_settop(L, top);
			return LUA_REFNIL;
		}
		//Duplicate this
		int ret = luaL_ref(L, LUA_REGISTRYINDEX);
		lua_settop(L, top);
		return ret;
	}
	// Do not expect to receive any return values
	lua_settop(L,top);	//Balance
	return LUA_REFNIL;
}

//The return value is a reference.
//The invalid retval is LUA_REFNIL
int ljLoadFuncHandle(const char *name) {
	_DeclareState()
	std::string cmd = "return require \"";
	cmd.append(name);
	cmd.append("\"");
	luaL_loadstring(L, cmd.c_str());
	int numArgs = 0;		//Fixed
    int functionIndex = -(numArgs + 1);
    if (!lua_isfunction(L, functionIndex)) {
        log("value at stack [%d] is not function", functionIndex);
        lua_pop(L, numArgs + 1); // remove function and arguments
        return LUA_REFNIL;
    }

    int traceback = 0;
    lua_getglobal(L, "__G__TRACKBACK__");                         /* L: ... func arg1 arg2 ... G */
    if (!lua_isfunction(L, -1))  {
        lua_pop(L, 1);                                            /* L: ... func arg1 arg2 ... */
    } else {
        lua_insert(L, functionIndex - 1);                         /* L: ... G func arg1 arg2 ... */
        traceback = functionIndex - 1;
    }
    int error = 0;
    error = lua_pcall(L, numArgs, 1, traceback);                  /* L: ... [G] ret */
    if (error) {
        if (traceback == 0)   {
            log("[LUA ERROR] %s", lua_tostring(L, - 1));        /* L: ... error */
            lua_pop(L, 1); // remove error message from stack
        } else {
            lua_pop(L, 2); // remove __G__TRACKBACK__ and error message from stack
        }
        return LUA_REFNIL;
    }
	if( !lua_isfunction(L, -1)){
		log("return value is a %s", toLuaType(L, -1));
		lua_settop(L, top);
		assert(false);
		return LUA_REFNIL;
	}
	assert(lua_isfunction(L, -1));
	int ret = luaL_ref(L, LUA_REGISTRYINDEX);
    if (traceback) {
        lua_pop(L, 1); // remove __G__TRACKBACK__ from stack      /* L: ... */
    }
    return ret;
}


//////////////////////
FuncRef::FuncRef()
	:_ref(LUA_REFNIL) {
}

FuncRef::FuncRef(const char *name)
	:_ref(LUA_REFNIL) {
	loadFunc(name);
}

FuncRef::~FuncRef() {
	ljReleaseObj(_ref);
}

FuncRef::FuncRef(const FuncRef&rhs)
	:_ref(LUA_REFNIL){
	_ref = ljDuplicateObj(rhs._ref);
}

FuncRef& FuncRef::operator=(const FuncRef &rhs){
	ljReleaseObj(_ref);
	_ref = ljDuplicateObj(rhs._ref);
	return *this;
}

void FuncRef::loadFunc(const char *name){
	assert( LUA_REFNIL == _ref );
	_ref = ljLoadFuncHandle(name);
	assert( LUA_REFNIL != _ref);
}

int FuncRef::createObj(){
	assert( LUA_REFNIL != _ref);
	return ljCreateTableFromFuncRef(_ref, 1, [](lua_State *L, int n)->int{return lua_istable(L,n);});
}

void FuncRef::execute() {
	ljCreateTableFromFuncRef(_ref, 0);
}

ObjContainer::ObjContainer():
	_ref(LUA_REFNIL),
	_objCount(0) {
	_ref = ljMakeTable();
}

ObjContainer::~ObjContainer(){
	ljReleaseObj(_ref);
}

void ObjContainer::clear(){
	ljReleaseObj(_ref);
	_objCount = 0;	//From Zero;
	assert( LUA_REFNIL == _ref);
	_ref = ljMakeTable();	//Another table;
}

bool ObjContainer::addObject(int obj) {
	_DeclareState()
	lua_rawgeti(L, LUA_REGISTRYINDEX, _ref);
	assert(lua_istable(L,-1)) ;
	lua_rawgeti(L,LUA_REGISTRYINDEX, obj);
	if (!lua_istable(L,-1)){
		assert(false);
		lua_pop(L, 2);
		return false;
	}
	int indexKey = _objCount + 1;	//The last one
	lua_rawseti(L, -2, indexKey);
	lua_pop(L, 1);
	assert( lua_gettop(L) == top);
	_objCount++;
	return true;
}

int ObjContainer::tableRef(){
	return _ref;
}
