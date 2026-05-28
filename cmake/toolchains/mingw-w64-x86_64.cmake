set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(_mingw_root "$ENV{MINGW_W64_ROOT}")

if(NOT _mingw_root)
    message(FATAL_ERROR "MINGW_W64_ROOT must point to the MinGW-w64 toolchain root")
endif()

set(CMAKE_C_COMPILER "${_mingw_root}/bin/x86_64-w64-mingw32-gcc.exe")
set(CMAKE_CXX_COMPILER "${_mingw_root}/bin/x86_64-w64-mingw32-g++.exe")
set(CMAKE_RC_COMPILER "${_mingw_root}/bin/x86_64-w64-mingw32-windres.exe")

list(PREPEND CMAKE_FIND_ROOT_PATH "${_mingw_root}")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)