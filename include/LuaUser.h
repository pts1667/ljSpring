/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#ifndef SPRING_LUA_USER_H
#define SPRING_LUA_USER_H

#ifdef __cplusplus
extern "C" {
	#include "../luajit/src/lua.h"
}
#endif

extern void LuaCreateMutex(lua_State* L);
extern void LuaDestroyMutex(lua_State* L);
extern void LuaLinkMutex(lua_State* L_parent, lua_State* L_child);
extern void LuaMutexLock(lua_State* L);
extern void LuaMutexUnlock(lua_State* L);
extern void LuaMutexYield(lua_State* L);

extern const char* spring_lua_get_handle_name(lua_State* L);

struct SLuaAllocState;
struct SLuaAllocError;

struct SLuaAllocError {
	// includes space for multiple messages, since we do not record them immediately
	char msgBuf[16384] = {0};
	char* msgPtr = nullptr;
};

extern void* spring_lua_alloc(void* ud, void* ptr, size_t osize, size_t nsize);
extern void spring_lua_alloc_get_stats(SLuaAllocState* state);
extern int spring_lua_alloc_get_error(SLuaAllocError* error);
extern int spring_lua_alloc_skip_gc(float gcLoadMult);
extern void spring_lua_alloc_update_stats(int clearStatsFrame);


extern void spring_lua_ftoa(float f, char *buf, int precision);
extern void spring_lua_format(float f, const char* fmt, char *buf);

// (these should) never (be) called from synced Lua states
extern int spring_lua_unsynced_rand(lua_State* L);
extern uint64_t spring_lua_unsynced_rand_raw();
extern int spring_lua_unsynced_srand(lua_State* L);
extern uint64_t spring_lua_unsynced_srand_raw();

#endif // SPRING_LUA_USER_H
