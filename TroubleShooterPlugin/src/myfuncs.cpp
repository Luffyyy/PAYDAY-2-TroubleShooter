#include "myfuncs.h"

int say_hello(lua_State *L) {
	// Require the first arg be an integer
	unsigned long val = luaL_checklong(L, 1);

	// Add the passed value shifted by two bits as a return value
	lua_pushinteger(L, val << 2);

	// Print out a log message
	PD2HOOK_LOG_LOG("Hello world!");

	// The number of Lua values this function returns.
	return 1;
}
