
#include "sp_config.h"

#include <iostream>
#include <string>
#include <exception>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include "SockSession.h"

#include "script/lua_script.h"

extern "C"{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "pbc.h"

#include "interop/lua_access/lua_access_macros.h"

std::function<void(const char*, int length)> gSender;

#define COUNT 1000000
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
		printf("Cannot open %s\n", pbFileName);
		return 0;
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
	const char *buffer = lua_tolstring(L, 1, &size);
	gSender(buffer, size);
	return 0;	// The return values
}

struct luaL_Reg entries[]={
	{"loadBinaryFile", loadBinaryFileForLua},
	{"sendBuffer", sendBuffer},
	{0,0}
};

static void
read_file (const char *filename , struct pbc_slice *slice) {
	FILE *f = fopen(filename, "rb");
	if (f == NULL) {
		slice->buffer = NULL;
		slice->len = 0;
		return;
	}
	fseek(f,0,SEEK_END);
	slice->len = ftell(f);
	fseek(f,0,SEEK_SET);
	slice->buffer = malloc(slice->len);
	if (fread(slice->buffer, 1 , slice->len , f) == 0)
		exit(1);
	fclose(f);
}


static void
test(struct pbc_env *env) {
	int i;
	for(i=0; i<COUNT; i++)
	{
			struct pbc_wmessage* w_msg = pbc_wmessage_new(env, "at");
			struct pbc_rmessage* r_msg = NULL;
			struct pbc_slice sl;
			char buffer[1024];
			sl.buffer = buffer, sl.len = 1024;
			pbc_wmessage_integer(w_msg, "aa", 123, 0);
			pbc_wmessage_integer(w_msg, "bb", 456, 0);
			pbc_wmessage_string(w_msg, "cc", "test string!", -1);
			pbc_wmessage_buffer(w_msg, &sl);
					
			r_msg = pbc_rmessage_new(env, "at", &sl);
			pbc_rmessage_delete(r_msg);
			pbc_wmessage_delete(w_msg);
	} 
}

void test()
{
	struct pbc_env * env = pbc_new();
	struct pbc_slice slice;
	read_file("..\\proto\\msg.pb", &slice);
	int ret = pbc_register(env, &slice);
	assert(ret == 0);
	//test(env);
	pbc_delete(env);
}

void expandFuncRes(int res)
{
	switch(res){
	case LUA_ERRRUN:
		printf("LUA_ERRRUN\n");
		break;
	case LUA_ERRMEM:
		printf("LUA_ERRMEM\n");
		break;
	case LUA_ERRERR:
		printf("LUA_ERRERR\n");
		break;
	case 0:
		printf("Executing success\n");
		break;
	}
}

void decodeBuffer(const char *buffer, int bufferSize)
{
	_DeclareState()
	lua_getglobal(L, "decoderX");
	assert(lua_isfunction(L, -1));

	int top = lua_gettop(L);
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
	int afterTop = lua_gettop(L);
	int res = lua_pcall(L, 1, 0, 0); 
	expandFuncRes(res);
	printf("Calling done.\n");
	//lua_pcall(L, 1, 0, 0);
}

void randSend()
{
	_DeclareState()
	lua_getglobal(L, "encoderX");
	assert(lua_isfunction(L, -1));
	int res = lua_pcall(L, 0, 0, 0);
	expandFuncRes(res);
}

int main()
{
	test(); 
	srand((unsigned int)time(0));
	boost::asio::io_service io;
	SockSessionPtr ss(SockSession::create(io));
	ss->setCallback(decodeBuffer);
	ss->connect();
	ss->setTimeout(3);

	gSender = [&ss](const char *buffer, int length){
		std::cout<<"About to send a buffer to server"<<std::endl;
		ss->write(buffer, length);
	};

	lua_State *L = LuaScript::instance()->getLuaState();
	luaL_register(L, "mm", entries);
	LuaScript::instance()->loadInit("embed/init.lua");

	const char *text = "hello, world";
	decodeBuffer(text, strlen(text));
	//
	//lua_getglobal(L, "decoderX");
	//assert(lua_isfunction(L, -1));
	//lua_pcall(L, 0, 0, 0);

	auto start = boost::posix_time::microsec_clock::universal_time();
	while(true){
		auto now = boost::posix_time::microsec_clock::universal_time();
		auto rv = io.poll();
		if( !rv && ss->isSocketFailed()){
			break;
		}

#if 1
//#if _TEST
		if( (now-start).ticks() > 1000 * 1000 ){
			//const char *either[]={"Hello, world", "Ride again"};
   //         const char *chosen = either[rand()&1];
			//ss->write(chosen);
			//std::cout<<"<<\""<<chosen<<"\""<<std::endl;
			randSend();

			start = now;
		}
#endif  // End of _TEST

	}


	//
	std::cout<<"End of program"<<std::endl;
	system("pause");
	return 0;
}