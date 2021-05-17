extern "C" {
    #include "LuaUserC.h"
}

#include "LuaUser.h"

int spring_lua_unsynced_rand_c(lua_State* L) { return spring_lua_unsynced_rand(L); }
uint64_t spring_lua_unsynced_rand_raw_c() { return spring_lua_unsynced_rand_raw(); }
int spring_lua_unsynced_srand_c(lua_State* L) { return spring_lua_unsynced_srand(L); }
uint64_t spring_lua_unsynced_srand_raw_c() { return spring_lua_unsynced_srand_raw(); }