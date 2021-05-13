set(sources_ljspring
  "include/LuaUser.cpp"
)

add_subdirectory(luajit)

add_library(luajitSpring STATIC EXCLUDE_FROM_ALL ${sources_ljspring})
target_include_directories(luajitSpring BEFORE PUBLIC "luajit/src")
target_link_libraries(luajitSpring PUBLIC luajit)