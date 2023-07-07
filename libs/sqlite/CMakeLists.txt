
cmake_minimum_required(VERSION 3.0.0)

set(PROJECT_NAME SQLite)

project(${PROJECT_NAME})

set(LIBRARY_NAME sqlite)

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

find_package (SQLite3)
if (SQLITE3_FOUND)
  include_directories(${SQLITE3_INCLUDE_DIRS})
  target_link_libraries (${LIBRARY_NAME} ${SQLITE3_LIBRARIES} sqlite3)
endif (SQLITE3_FOUND)

target_include_directories(${LIBRARY_NAME} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${LIBRARY_NAME}>
    $<INSTALL_INTERFACE:include/${LIBRARY_NAME}>
)

target_include_directories(${LIBRARY_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# add_subdirectory(test)
