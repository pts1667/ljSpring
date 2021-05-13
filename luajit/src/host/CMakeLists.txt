# Technically most of this isn't needed
project(luajit_host C ASM)
include(TestBigEndian)

set(CMAKE_C_FLAGS_DEBUG "-fomit-frame-pointer ${LJ_MARCH}")
set(CMAKE_C_FLAGS_RELEASE "-O2 -fomit-frame-pointer ${LJ_MARCH}")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -fomit-frame-pointer ${LJ_MARCH}")
set(CMAKE_C_FLAGS_PROFILE "-O2 -pg -fomit-frame-pointer ${LJ_MARCH}")

set(DASM_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../dynasm")
set(DASM "${DASM_DIR}/dynasm.lua")
set(DASM_ARCH "x64")
set(DASM_DASC "${CMAKE_CURRENT_SOURCE_DIR}/../vm_${DASM_ARCH}.dasc")
set(DASM_FLAGS "-D JIT -D P64 -D HFABI -D FPU -D FFI -D NO_UNWIND")
set(IS_BE 0)
TEST_BIG_ENDIAN(IS_BE)

if (IS_BE MATCHES 1)
  set(DASM_FLAGS "${DASM_FLAGS} -D ENDIAN_BE")
else ()
  set(DASM_FLAGS "${DASM_FLAGS} -D ENDIAN_LE")
endif ()

if (TARGET_WINDOWS)
  set(DASM_FLAGS "${DASM_FLAGS} -D WIN")
endif()

set(luajit_minilua_sources
  "${CMAKE_CURRENT_SOURCE_DIR}/minilua.c"
)

set(luajit_host_sources
  "${CMAKE_CURRENT_SOURCE_DIR}/buildvm.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/buildvm_asm.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/buildvm_fold.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/buildvm_lib.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/buildvm_peobj.c"
)

# build minilua
add_executable(minilua ${luajit_minilua_sources})
target_link_libraries(minilua PRIVATE m)

separate_arguments(DASM_FLAG_LIST WINDOWS_COMMAND "${DASM_FLAGS}")
add_custom_target(buildvm_arch
  COMMAND minilua ${DASM} ${DASM_FLAG_LIST} -o ${CMAKE_CURRENT_SOURCE_DIR}/buildvm_arch.h ${DASM_DASC}
  DEPENDS minilua
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  VERBATIM
)

# build buildvm
include_directories(../)

add_executable(buildvm ${luajit_host_sources})
add_dependencies(buildvm buildvm_arch)
target_include_directories(buildvm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../dynasm)
target_compile_definitions(buildvm PUBLIC
  LUAJIT_TARGET=LUAJIT_ARCH_X64
  LJ_NO_UNWIND=1
  LJ_ABI_SOFTFP=0
  LJ_ARCH_HASFPU=1
  _FILE_OFFSET_BITS=64 _LARGEFILE_SOURCE LJ_ABI_SOFTFP=0
  LUAJIT_SECURITY_PRNG=0 LUAJIT_SECURITY_STRHASH=0 LUAJIT_SECURITY_STRID=0
)