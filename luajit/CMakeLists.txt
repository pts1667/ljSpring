include_directories(BEFORE ../../streflop)

if (MARCH_FLAG)
  set(LJ_MARCH "-march=${MARCH_FLAG}")
else(MARCH_FLAG)
  set(LJ_MARCH "-mtune=generic")
endif (MARCH_FLAG)

set(CMAKE_C_FLAGS_DEBUG "-fomit-frame-pointer ${LJ_MARCH}")
set(CMAKE_C_FLAGS_RELEASE "-O2 -fomit-frame-pointer ${LJ_MARCH}")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -fomit-frame-pointer ${LJ_MARCH}")
set(CMAKE_C_FLAGS_PROFILE "-O2 -pg -fomit-frame-pointer ${LJ_MARCH}")

set(sources_ljlib
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lib_aux.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lib_base.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lib_bit.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lib_buffer.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lib_debug.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lib_ffi.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lib_init.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lib_io.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lib_jit.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lib_math.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lib_os.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lib_package.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lib_string.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lib_table.c"
)

set(sources_luajit
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_alloc.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_api.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_asm.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_assert.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_bc.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_bcread.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_bcwrite.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_buf.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_carith.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_ccall.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_ccallback.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_cconv.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_cdata.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_char.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_clib.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_cparse.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_crecord.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_ctype.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_debug.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_dispatch.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_err.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_ffrecord.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_func.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_gc.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_gdbjit.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_ir.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_lex.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_lib.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_load.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_mcode.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_meta.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_obj.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_opt_dce.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_opt_fold.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_opt_loop.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_opt_mem.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_opt_narrow.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_opt_sink.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_opt_split.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_parse.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_prng.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_profile.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_record.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_serialize.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_snap.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_state.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_str.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_strfmt.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_strfmt_num.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_strscan.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_tab.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_trace.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_udata.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_vmevent.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_vmmath.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/ljamalg.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/luajit.c"
)

set(sources_genfiles
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lj_vm.S"
)

set(sources_ljamalg
  "${CMAKE_CURRENT_SOURCE_DIR}/src/ljamalg.c"
)

add_subdirectory(builds)

add_library(luajit STATIC ${sources_ljamalg} ${sources_genfiles})
target_compile_definitions(luajit PUBLIC  
  LUAJIT_TARGET=LUAJIT_ARCH_X64
  LJ_NO_UNWIND=1
  LJ_ABI_SOFTFP=0
  LJ_ARCH_HASFPU=1
  _FILE_OFFSET_BITS=64 _LARGEFILE_SOURCE LJ_ABI_SOFTFP=0
  LUAJIT_SECURITY_PRNG=0 LUAJIT_SECURITY_STRHASH=0 LUAJIT_SECURITY_STRID=0  
)

if (MINGW OR MSVC OR CMAKE_CROSS_COMPILING) # windows. CMAKE_CROSS_COMPILING is not a typo. at least, not here...
  target_compile_definitions(luajit PUBLIC WIN LUAJIT_OS=LUAJIT_OS_WINDOWS)
  set_source_files_properties(sources_genfiles PROPERTIES # on Windows, lj_vm.S is a PE Object
    EXTERNAL_OBJECT TRUE
    GENERATED TRUE)
elseif (APPLE) # OS X
  target_compile_definitions(luajit PUBLIC LUAJIT_OS=LUAJIT_OS_OSX)
elseif (UNIX) # Linux
  target_compile_definitions(luajit PUBLIC LUAJIT_OS=LUAJIT_OS_LINUX)
else () # something else
  target_compile_definitions(luajit PUBLIC LUAJIT_OS=LUAJIT_OS_OTHER)
endif()

if (ENABLE_STREFLOP)
  target_link_libraries(luajit PUBLIC streflop)
endif ()