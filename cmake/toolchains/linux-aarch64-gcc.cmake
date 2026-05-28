set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

set(_aarch64_root "$ENV{AARCH64_GNU_ROOT}")
set(_aarch64_sysroot "$ENV{AARCH64_SYSROOT}")

if(NOT _aarch64_root)
    message(FATAL_ERROR "AARCH64_GNU_ROOT must point to the cross toolchain root")
endif()

if(NOT _aarch64_sysroot)
    message(FATAL_ERROR "AARCH64_SYSROOT must point to the target sysroot")
endif()

set(CMAKE_C_COMPILER "${_aarch64_root}/bin/aarch64-linux-gnu-gcc")
set(CMAKE_CXX_COMPILER "${_aarch64_root}/bin/aarch64-linux-gnu-g++")
set(CMAKE_SYSROOT "${_aarch64_sysroot}")

list(PREPEND CMAKE_FIND_ROOT_PATH "${_aarch64_sysroot}" "${_aarch64_root}")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)