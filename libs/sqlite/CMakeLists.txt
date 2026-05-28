
cmake_minimum_required(VERSION 4.0.0)

project(SQLite LANGUAGES CXX)

set(LIBRARY_NAME sqlite)
set(CPP_OOP_VCPKG_INSTALLED_DIR "${CMAKE_SOURCE_DIR}/vcpkg_installed" CACHE PATH "Local vcpkg_installed directory used for package discovery")

set(LIBRARY_HEADERS_DIR include/${LIBRARY_NAME})
set(LIBRARY_HEADERS ${LIBRARY_HEADERS_DIR}/sqlite.hh)

set(LIBRARY_SOURCE_DIR src)
set(LIBRARY_SOURCE
    ${LIBRARY_SOURCE_DIR}/sqlite.cc
)

add_library(${LIBRARY_NAME} STATIC
    ${LIBRARY_HEADERS}
    ${LIBRARY_SOURCE}
)

if(TARGET project_warnings)
  target_link_libraries(${LIBRARY_NAME} PRIVATE project_warnings)
endif()

set(_cpp_oop_vcpkg_triplet "${VCPKG_TARGET_TRIPLET}")

if(NOT _cpp_oop_vcpkg_triplet)
  if(WIN32)
    set(_cpp_oop_vcpkg_triplet "x64-windows")
  elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|AMD64)$")
    set(_cpp_oop_vcpkg_triplet "x64-linux")
  endif()
endif()

if(_cpp_oop_vcpkg_triplet AND EXISTS "${CPP_OOP_VCPKG_INSTALLED_DIR}/${_cpp_oop_vcpkg_triplet}")
  list(PREPEND CMAKE_PREFIX_PATH "${CPP_OOP_VCPKG_INSTALLED_DIR}/${_cpp_oop_vcpkg_triplet}")
endif()

find_package(unofficial-sqlite3 CONFIG QUIET)

if(TARGET unofficial::sqlite3::sqlite3)
  target_link_libraries(${LIBRARY_NAME} PRIVATE unofficial::sqlite3::sqlite3)
else()
  find_package(SQLite3 REQUIRED)

  if(TARGET SQLite::SQLite3)
    target_link_libraries(${LIBRARY_NAME} PRIVATE SQLite::SQLite3)
  else()
    target_include_directories(${LIBRARY_NAME} SYSTEM PRIVATE ${SQLite3_INCLUDE_DIRS})
    target_link_libraries(${LIBRARY_NAME} PRIVATE ${SQLite3_LIBRARIES})
  endif()
endif()


target_include_directories(${LIBRARY_NAME} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${LIBRARY_NAME}>
    $<INSTALL_INTERFACE:include/${LIBRARY_NAME}>
)

target_include_directories(${LIBRARY_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

install(TARGETS ${LIBRARY_NAME} ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
