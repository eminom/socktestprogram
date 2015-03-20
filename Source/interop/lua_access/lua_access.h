

#ifndef _LUA_ACCESS_DEF_H__
#define _LUA_ACCESS_DEF_H__

#include "lua_access_macros.h"
#include <string>
#include <cassert>

struct lua_State;

#define log(...)	printf(__VA_ARGS__);


#ifndef CCLog
#define CCLog(...)	{fprintf(stderr, __VA_ARGS__); fprintf(stderr,"\n");}
#endif

#define _preprocess_widget()\
		case 'x':\
		{\
			Widget *widget = va_arg(args, Widget*);\
			stack->pushObject(widget,"ccui.Widget");\
		}\
		break;\

//#define _preprocess_ccnode()\
//		case 'c':\
//		{\
//			Node *node = va_arg(args,Node*);\
//			stack->pushObject(node, "cc.Node");\
//		}\
//		break;

#define _preprocess_ccnode()

#if !defined(_prepare_ccnode_parameters)
	#define _prepare_ccnode_parameters()\
		_preprocess_ccnode()
#endif

#define _CheckTop()\
	if( lua_gettop(L) != top ){\
		CCLog("Error stack top corrupted!\n");\
		assert(0);\
	}

#define _GetBoolFuncVoidVal(funcName)\
	_DeclareState()	\
	int top = lua_gettop(L);\
	int traceback = 0;\
	lua_getglobal(L, "__G__TRACKBACK__");\
	if(lua_isfunction(L, -1)){\
		traceback = -2;\
	}\
	lua_getglobal(L, funcName);\
	if ( ! lua_isfunction(L, -1) )\
	{\
		CCLog("Error executing %s", funcName);\
		lua_pop(L, 2);\
		_CheckTop()\
		return false;	\
	}\
	\
	if( lua_pcall(L, 0, 1, traceback) )\
	{\
		CCLog("Error executing %s", funcName);\
		lua_pop(L, 2);\
		_CheckTop()\
		return false;\
	}\
	\
	int value = lua_toboolean(L, -1);\
	lua_pop(L, 2);\
	_CheckTop()


#define _GetStringFuncVoidVal(funcName)\
	_DeclareState()		\
	int top = lua_gettop(L);\
	int traceback = 0;\
	lua_getglobal(L, "__G__TRACKBACK__");\
	if(lua_isfunction(L, -1)){\
		traceback = -2;\
	}\
	lua_getglobal(L, funcName);\
	if ( ! lua_isfunction(L, -1) )\
	{\
		CCLog("Error executing %s", funcName);\
		lua_pop(L, 2);\
		_CheckTop()\
		return "";	\
	}\
	\
	if( lua_pcall(L, 0, 1, traceback) )\
	{\
		CCLog("Error executing %s", funcName);\
		lua_pop(L, 2);\
		_CheckTop()\
		return "";\
	}\
	\
	const char *value = lua_tostring(L, -1);\
	value = value ? value:"";\
	lua_pop(L, 2);\
	_CheckTop()


#define _GetIntegerFuncVoidVal(funcName)\
	_DeclareState()		\
	int top = lua_gettop(L);\
	int traceback = 0;\
	lua_getglobal(L, "__G__TRACKBACK__");\
	if(lua_isfunction(L, -1)){\
		traceback = -2;\
	}\
	lua_getglobal(L, funcName);\
	if ( ! lua_isfunction(L, -1) )\
	{\
		CCLog("Error executing %s", funcName);\
		lua_pop(L, 2);\
		_CheckTop()\
		return 0;	\
	}\
	\
	if( lua_pcall(L, 0, 1, traceback) )\
	{\
		CCLog("Error executing %s", funcName);\
		lua_pop(L, 2);\
		_CheckTop()\
		return 0;\
	}\
	\
	int value = lua_tointeger(L, -1);\
	lua_pop(L, 2);\
	_CheckTop()


#define ExecuteFunctionOnTop(args, returnCount, ...)\
    int sz = strlen(format);\
	int parameterCount = sz;\
	_execute_function_on_top_body(args, returnCount, __VA_ARGS__)

#define ExecuteFunctionWithPrepushed(args, prepushed, returnCount, ...)\
    int sz = strlen(format);\
	int parameterCount = sz;\
	parameterCount += prepushed;\
	_execute_function_on_top_body(args, returnCount, __VA_ARGS__)

//Do NOT va-_end(args)
#define _execute_function_on_top_body(args, returnCount, ...)\
	for(int i=0;i<sz;++i)\
	{\
		switch(format[i])\
		{\
		case 'v':\
			{\
				void* ptr = va_arg(args, void*);\
				lua_pushlightuserdata(L, ptr);\
			}break;\
		case 'u':\
			{\
				int (*func)(lua_State*L) = (int(*)(lua_State*))va_arg(args, void*);\
				lua_pushcfunction(L, func);\
			}break;\
		case 'b':	\
			{\
				int bVal = va_arg(args, int);\
				lua_pushboolean(L, bVal);\
			}break;\
		case 'f':\
			{\
				float fVal = va_arg(args, double);\
				lua_pushnumber(L, fVal);\
			}break;\
		case 'd':\
		case 'i':	\
			{\
				int iVal = va_arg(args,int);\
				lua_pushinteger(L, iVal);\
			}break;\
		case 's':	\
			{\
				const char *str = va_arg(args, const char*);\
				lua_pushstring(L, str);\
			}\
			break;\
		case 'o':\
			{\
				int ref = va_arg(args, int);\
				lua_rawgeti(L, LUA_REGISTRYINDEX, ref);\
				if(!lua_istable(L,-1)){ \
					printf("ERROR NOT A TABLE\n");\
					assert(false);\
				}\
			}\
			break;\
		case 'O':\
			{\
				int ref = va_arg(args, int);\
				if( LUA_REFNIL == ref) {\
					lua_pushnil(L);\
				} else {\
					lua_rawgeti(L, LUA_REGISTRYINDEX, ref);\
					if(!lua_istable(L,-1)){ \
						printf("ERROR NOT A TABLE\n");\
						assert(false);\
					}\
				}\
			}\
			break;\
		_prepare_ccnode_parameters()\
		default:\
			log("Error unknown object identifier :%c", format[i]);\
			--parameterCount;\
			break;\
		}\
	}\
	\
	int traceback = 0;\
	int funcIndex = -(parameterCount+1);\
    lua_getglobal(L, "__G__TRACKBACK__");\
    if (!lua_isfunction(L, -1))\
    {\
        lua_pop(L, 1);\
    }\
    else\
    {\
        lua_insert(L, funcIndex - 1);\
        traceback = funcIndex - 1;\
    }\
	\
	if( lua_pcall(L, parameterCount, returnCount, traceback) )\
	{\
		log("Error executing %s", funcName);\
		lua_settop(L, top);\
		return __VA_ARGS__;\
	}


#define _ExecuteBody(returnCount, ...)\
	_DeclareState()	\
	lua_getglobal(L, funcName);\
	if( !lua_isfunction(L, -1) )\
	{\
		log("%s is not a function. Error.", funcName);\
		assert(false);\
		lua_settop(L,top);\
		return __VA_ARGS__;\
	}\
	va_list args;\
	va_start(args, format);\
	ExecuteFunctionOnTop(args, returnCount, __VA_ARGS__)\
	va_end(args);

#define _get_table_field_onto_stack(type_checker, ...)\
	if (result){\
		*result = false;\
	}\
	lua_rawgeti(L, LUA_REGISTRYINDEX, ref);\
	if ( !lua_istable(L, -1)){\
		log("Error get table for %s: not a table", name);\
		lua_settop(L, top);\
		return __VA_ARGS__;\
	}\
	lua_pushstring(L, name);\
	lua_gettable(L, -2);\
	if( !type_checker(L,-1)){\
		log("Error get table %s(%s): type error", #type_checker, name);\
		log("(%s) is actually %s\n",name, toLuaType(L, -1));\
		lua_settop(L, top);\
		return __VA_ARGS__;\
	}\

#define _get_table_field(type_checker, caster, def_rv)\
	_get_table_field_onto_stack(type_checker, def_rv)\
	auto rv = caster(L, -1);\
	lua_settop(L, top);\
	if (result) {\
		*result = true;\
	}


void executeVoidFunc(const char *funcName, const char *format, ...);
std::string executeStringFunc(const char *funcName, const char *format, ...);
float executeNumberFunc(const char *funcName, const char *format,...);
int executeIntegerFunc(const char *funcName, const char *format="",...);
float executeFloatFunc(const char *funcName, const char *format,...);
int executeTableFunc(const char *funcName, const char *format, ...);

void execVoidFunc(int ref, const char *format, ...);

//Holding objects at C
int ljMakeTable();
//int ljLoadFuncHandle(const char *name);
//int ljCreateTableFromFuncRef(int ref);

//getters
int ljGetTableInteger(int ref, const char *name, bool *result=nullptr);
bool ljGetTableBoolean(int ref, const char *name, bool *result=nullptr);
std::string	ljGetTableString(int ref, const char *name, bool *result=nullptr);
float ljGetTableFloat(int ref, const char *name, bool *result=nullptr);

//execute some funcs on tab
int ljRunObjInteger(int ref, const char *funcName, bool *result, const char *format, ...);
void ljRunObjVoid(int ref, const char *funcName, bool *result, const char *format, ...);
void ljRunObjVoidSelfUserData(int ref, const char *funcName, bool *result, void *p);


int ljDuplicateObj(int ref);
void ljReleaseObj(int &ref);
int ljLoadObj(const char *name);

const char* toLuaType(lua_State *L, int index);

///
class FuncRef {

public:
	FuncRef();
	FuncRef(const char*name);
	~FuncRef();
	FuncRef(const FuncRef&);
	FuncRef& operator=(const FuncRef&);
	int createObj();
	void execute();

private:
	void loadFunc(const char *name);

private:
	int _ref;
};

class ObjContainer{
public:
	ObjContainer();
	~ObjContainer();
	bool addObject(int obj);
	int tableRef();
	void clear();

private:
	int _ref;
	int _objCount;

private:
	ObjContainer(const ObjContainer&);
	ObjContainer&operator=(const ObjContainer&);
};

#endif