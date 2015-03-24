

#include "exports.h"
#include "script/lua_script.h"
#include <cstdlib>
#include <cstring>

#include "SockSessionManager.h"
#include "interop/lua_access/lua_access_macros.h"

static int loadBinaryFileForLua(lua_State * s)
{
	size_t size = 0;
	const char * pbFileName = luaL_checklstring(s,1,&size);
	unsigned int bufferSize = 0;

	unsigned char * buffer = 0;
	if( FILE *fin = fopen(pbFileName, "rb") )
	{
		fseek(fin, 0, SEEK_END);
		size = ftell(fin);
		fseek(fin, 0, SEEK_SET);
		bufferSize = size;
		buffer = new unsigned char[bufferSize];
		memset(buffer, 0, sizeof(unsigned char) * (bufferSize));
		fread(buffer, 1, bufferSize, fin);
		fclose(fin);
	} 
	else 
	{
		luaL_error(s, "Cannot open %s", pbFileName);
	}

	luaL_Buffer b;
	luaL_buffinit(s, &b);
	int n = bufferSize;
	int rlen = LUAL_BUFFERSIZE;  /* try to read that much each time */
	int index = 0;
	do {
		char *p = luaL_prepbuffer(&b);
		if (rlen > n) rlen = n;  /* cannot read more than asked */
		memcpy(p, buffer + index,rlen);
		luaL_addsize(&b, rlen);
		index += rlen;
		n -= rlen;  /* still have to read `n' chars */
	} while (n > 0);  /* until end of count or eof */
	luaL_pushresult(&b);  /* close buffer */
	return 1;
}

static int sendBuffer(lua_State *L)
{
	size_t size = 0;
	if( lua_gettop(L) != 2 )
	{
		luaL_error(L, "Missing parameters for SenderBuffer");
	}
	const char *buffer = lua_tolstring(L, 1, &size);
	int typeCode = luaL_checkint(L, 2);
	SockSessionManager::instance()->write(typeCode, buffer, size);
	return 0;	// The return values
}

static int connectServer(lua_State *L)
{
	if( lua_gettop(L) != 3)
	{
		luaL_error(L, "connect server parameter missing");
	}
	const char *host = luaL_checkstring(L, 1);
	host = host ? host: "";
	const char *port = luaL_checkstring(L, 2);
	port = port ? port: "";
	const char *serverID = luaL_checkstring(L, 3);
	serverID = serverID ? serverID:"";
	SockSessionManager::instance()->connectTo(host, port, serverID);
	return 0;
}

struct luaL_Reg entries[]={
	{"LoadBinaryFile", loadBinaryFileForLua},
	{"SendBuffer", sendBuffer},
	{"ConnectServer", connectServer},
	{0,0}
};

void luaopen_mm(lua_State *L)
{
	luaL_register(L, "mm", entries);
}

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