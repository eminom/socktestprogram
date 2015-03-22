

#include "dump.h"
#include "interop/lua_access/lua_access.h"

#ifdef __APPLE__
extern "C" {
#endif
#include "pbc.h"
#ifdef __APPLE__
}
#endif

#define COUNT 1000000

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


static void	test(struct pbc_env *env) {
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

namespace GameCore
{

void frameUpdate(float dt)
{
    const char *func = "frameUpdate";
	_DeclareState()
	lua_getglobal(L, "__G_TRACEBACK");
	assert(lua_isfunction(L, -1));

	lua_getglobal(L, func);
	assert(lua_isfunction(L, -1));

	lua_pushnumber(L, dt);
    if(lua_pcall(L, 1, 0, -3)){
        lua_pop(L, 2);
        assert( top == lua_gettop(L));
        printf("Error running \"%s\"\n", func);
        return;
    }
	lua_pop(L, 1);
	assert( lua_gettop(L) == top );
}

}