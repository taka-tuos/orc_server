#ifndef __ORC_SERVER__H__
#define __ORC_SERVER__H__
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

int luaSystemInit(char *sz);
int luaSystemCall();

#endif