

#ifndef _INTERTOP_CONF__
#define _INTERTOP_CONF__


#include "interop/exports/exports.h"

#define _InitLuaVM(L)\
    do{\
        luaopen_mm(L);\
    }while(0);\

#endif

