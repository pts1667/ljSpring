#ifndef LUA_LUSERC_H
#define LUA_LUSERC_H

#include "../luajit/src/lua.h"
#include "stdint.h"

extern const char* spring_lua_get_handle_name(lua_State* L);

struct SLuaAllocState;
struct SLuaAllocError;

typedef struct SLuaAllocState SLuaAllocState;
typedef struct SLuaAllocError SLuaAllocError;

// (these should) never (be) called from synced Lua states
extern int spring_lua_unsynced_rand_c(lua_State* L);
extern uint64_t spring_lua_unsynced_rand_raw_c();
extern int spring_lua_unsynced_srand_c(lua_State* L);
extern uint64_t spring_lua_unsynced_srand_raw_c();

#endif