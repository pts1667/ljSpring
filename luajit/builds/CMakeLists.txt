# build luajit toolchain on host
set(HOST_BIN "${CMAKE_BINARY_DIR}/host")
execute_process(
  COMMAND ${CMAKE_COMMAND} -E make_directory "${HOST_BIN}"
)

if (CMAKE_CROSSCOMPILING OR CMAKE_CROSS_COMPILING) # MXE is really buggy so one may need to include MXE's bin into their path
  execute_process(
    COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}"
    -DCMAKE_BUILD_TYPE="Release"
    -DCMAKE_MAKE_PROGRAM=${CMAKE_MAKE_PROGRAM}
    -DCMAKE_C_COMPILER=/usr/bin/gcc
    -DCMAKE_CXX_COMPILER=/usr/bin/g++
    -DTARGET_WINDOWS=1
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/host"
    WORKING_DIRECTORY "${HOST_BIN}"
    COMMAND_ECHO STDOUT
  )
else ()
  execute_process(
    COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}"
    -DCMAKE_BUILD_TYPE="Release"
    -DCMAKE_MAKE_PROGRAM=${CMAKE_MAKE_PROGRAM}
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/host"
    WORKING_DIRECTORY "${HOST_BIN}"
    COMMAND_ECHO STDOUT
  )
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} --build .
  WORKING_DIRECTORY "${HOST_BIN}"
)

if (MINGW OR MSVC) # windows
  set(LJVM_MODE peobj)
elseif (APPLE) # OS X
  set(LJVM_MODE machasm)
elseif (UNIX) # Linux
  set(LJVM_MODE elfasm)
else () # something else
  set(LJVM_MODE elfasm)
endif()

if (MSVC)
  set(BUILDVM_X "${HOST_BIN}/Release/buildvm.exe")
elseif (CMAKE_GENERATOR STREQUAL "Xcode")
  set(BUILDVM_X "${HOST_BIN}/Release/buildvm")
else ()
  set(BUILDVM_X "${HOST_BIN}/buildvm")
endif ()

set(LJ_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../src")

set(LJVM_BOUT "${LJ_SRC_DIR}/lj_vm.S")
set(LJ_BCDEF "${LJ_SRC_DIR}/lj_bcdef.h")
set(LJ_FFDEF "${LJ_SRC_DIR}/lj_ffdef.h")
set(LJ_LIBDEF "${LJ_SRC_DIR}/lj_libdef.h")
set(LJ_RECDEF "${LJ_SRC_DIR}/lj_recdef.h")
set(LIB_VMDEF "${LJ_SRC_DIR}/jit/vmdef.lua")
set(LJ_FOLDDEF "${LJ_SRC_DIR}/lj_folddef.h")
set(luajit_foldopt_src "${LJ_SRC_DIR}/lj_opt_fold.c")

execute_process(
  COMMAND ${BUILDVM_X} -m ${LJVM_MODE} -o ${LJVM_BOUT}
  COMMAND ${BUILDVM_X} -m bcdef -o ${LJ_BCDEF} ${sources_ljlib}
  COMMAND ${BUILDVM_X} -m ffdef -o ${LJ_FFDEF} ${sources_ljlib}
  COMMAND ${BUILDVM_X} -m libdef -o ${LJ_LIBDEF} ${sources_ljlib}
  COMMAND ${BUILDVM_X} -m recdef -o ${LJ_RECDEF} ${sources_ljlib}
  COMMAND ${BUILDVM_X} -m vmdef -o ${LIB_VMDEF} ${sources_ljlib}
  COMMAND ${BUILDVM_X} -m folddef -o ${LJ_FOLDDEF} ${luajit_foldopt_src}
)