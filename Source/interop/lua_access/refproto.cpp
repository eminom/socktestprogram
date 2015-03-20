

#include <cassert>
#include "refproto.h"

//#define _prepare_ccnode_parameters()
#include "lua_access.h"
//#include "interop/interoplog.h"

//Using CCNode

#define _ProtoLoader	"Load"

//Need to balance the stack
#define _CheckTableRef(__ref)\
	{\
		assert(__ref != LUA_REFNIL);\
		lua_rawgeti(L, LUA_REGISTRYINDEX, __ref);\
		assert(lua_istable(L,-1));\
		lua_pop(L, 1);\
	}

#define _PreBase()\
	_DeclareState()\
	int self_ = tableRef();\
	assert (self_ != LUA_REFNIL);

#define _Retri(name_, getter_)\
	_PreBase()\
	bool ok = false;\
	auto rv = getter_(self_, name_, &ok);\
	assert(ok);\
	return rv;

void LuaRefProto::loadWith(int ref) {
	mix(ref, _ProtoLoader);
}

void LuaRefProto::load() {
	_PreBase()
	bool ok = false;
	int rv = ljRunObjInteger(self_, _ProtoLoader, &ok, "o", self_);
	assert(ok);
	assert(1==rv);
}

int LuaRefProto::mix(int ref, const char *name) {
	_PreBase()
	_CheckTableRef(ref)
	bool ok = false;
	int rv = ljRunObjInteger(self_, name, &ok, "oo", self_, ref);
	assert(ok);
	//assert(1==rv);
	return rv;
}

void LuaRefProto::fireOn(const char *event) {
	_PreBase()
	bool ok = false;
	int rv = ljRunObjInteger(self_, event, &ok, "o", self_);
	assert(1==rv);
}

int LuaRefProto::getInt(const char *name) {
	_Retri(name, ljGetTableInteger)
}

bool LuaRefProto::getBoolean(const char *name) {
	_Retri(name, ljGetTableBoolean)
}

std::string LuaRefProto::getString(const char *name) {
	_Retri(name, ljGetTableString);
}

float LuaRefProto::getFloat(const char *name){
	_Retri(name, ljGetTableFloat);
}

int LuaRefProto::getIntFormat(const char *name, bool *result, const char *format, va_list args) {
	int ref = tableRef();
	_DeclareState()
	_get_table_field_onto_stack(lua_isfunction, 0)
	const char *funcName = name;
	lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
	if(!lua_istable(L, -1)){
		log("Error ref not a table");
		lua_settop(L, top);
		return 0;
	}
	ExecuteFunctionWithPrepushed(args, 1, 1, 0)
	if(!lua_isnumber(L, -1)){
		log("Error return value is not number !!");
		log("which is actually %s", toLuaType(L,-1));
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

int LuaRefProto::execInt(const char *name, const char *format, ...){
	va_list args;
	va_start(args, format);
	bool result = false;
	int retval = getIntFormat(name, &result, format, args);
	assert(result);
	va_end(args);
	return retval;
}

std::string LuaRefProto::getStringFormat(const char *name, bool *result, const char *format, va_list args){
	int ref = tableRef();
	_DeclareState()
	_get_table_field_onto_stack(lua_isfunction, "")
	const char *funcName = name;
	lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
	if(!lua_istable(L, -1)){
		log("Error ref not a table");
		lua_settop(L, top);
		return "";
	}
	ExecuteFunctionWithPrepushed(args, 1,1, "")
	if (!lua_isstring(L,-1)){
		lua_settop(L, top);
		return "";
	}
	const char *rv = lua_tostring(L, -1);
	std::string retval;
	if(rv){
		retval = rv;
	}
	lua_settop(L, top);
	if(result){
		*result = true;
	}
	return retval;
}

float LuaRefProto::getFloatFormat(const char *name, bool *result, const char *format, va_list args) {
	int ref = tableRef();
	_DeclareState()
	_get_table_field_onto_stack(lua_isfunction, 0)
	const char *funcName = name;
	lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
	if (!lua_istable(L, -1)) {
		log("Error ref not a table");
		assert(false);
		lua_settop(L, top);
		return 0;
	}
	ExecuteFunctionWithPrepushed(args, 1,1, 0)
	if (!lua_isnumber(L,-1)){
		log("Error return value is not float");
		assert(false);
		lua_settop(L, top);
		return 0;
	}
	float retv = lua_tonumber(L, -1);
	lua_settop(L, top);
	if(result){
		*result = true;
	}
	return retv;
}

std::string LuaRefProto::execString(const char *name, const char *format, ...){
	va_list args;
	va_start(args, format);
	bool result = false;
	std::string retval = getStringFormat(name, &result, format, args);
	assert(result);
	va_end(args);
	return retval;
}

void LuaRefProto::getVoidFormat(const char *name, bool *result, const char *format, va_list args) {
	int ref = tableRef();
	assert(ref != LUA_REFNIL);
	_DeclareState()
	_get_table_field_onto_stack(lua_isfunction)
	const char *funcName = name;
	lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
	if(!lua_istable(L, -1)){
		log("Error ref not a table");
		lua_settop(L, top);
		return;
	}
	ExecuteFunctionWithPrepushed(args, 1, 0)
	if(result){
		*result = true;
	}
	lua_settop(L, top);
}

void LuaRefProto::execVoid(const char *name, const char *format, ...) {
	va_list args;
	va_start(args, format);
	bool result = false;
	getVoidFormat(name, &result, format, args);
	va_end(args);
	assert(result);
}

void LuaRefProto::runVoidFormat(const char *name, bool *result, const char *format, ...) {
	int ref = tableRef();
	assert(ref != LUA_REFNIL);
	_DeclareState()
	_get_table_field_onto_stack(lua_isfunction)
	const char *funcName = name;
	va_list args;
    va_start(args, format);
	ExecuteFunctionWithPrepushed(args, 0, 0)
	va_end(args);
	if(result){
		*result = true;
	}
	lua_settop(L, top);
}

bool LuaRefProto::getBooleanFormat(const char *name, bool *result, const char *format, va_list args) {
	int ref = tableRef();
	_DeclareState()
	_get_table_field_onto_stack(lua_isfunction, false)
	const char *funcName = name;
	lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
	if(!lua_istable(L, -1)){
		log("Error ref not a table");
		lua_settop(L, top);
		return 0;
	}
	ExecuteFunctionWithPrepushed(args, 1, 1, false)
	if(!lua_isboolean(L, -1)){
		log("Error return value not a boolean");
		lua_settop(L, top);
		return 0;
	}
	bool rv = lua_toboolean(L, -1);
	lua_settop(L, top);
	if(result){
		*result = true;
	}
	return rv;
}

bool LuaRefProto::execBoolean(const char *name, const char *format, ...){
	va_list args;
	va_start(args, format);
	bool result = false;
	bool res = getBooleanFormat(name, &result, format, args);
	assert(result);
	va_end(args);
	return res;
}

float LuaRefProto::execFloat(const char *name, const char *format,...) {
	va_list args;
	va_start(args, format);
	bool result = false;
	float rv = getFloatFormat(name, &result, format, args);
	assert(result);
	va_end(args);
	return rv;
}