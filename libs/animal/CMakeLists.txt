
cmake_minimum_required(VERSION 3.0.0)

set(PROJECT_NAME Animal)

project(${PROJECT_NAME})

set(LIBRARY_NAME animal)

set(LIBRARY_HEADERS_DIR include/${LIBRARY_NAME})
set(LIBRARY_HEADERS ${LIBRARY_HEADERS_DIR}/animal.hh)

set(LIBRARY_SOURCE_DIR src)
set(LIBRARY_SOURCE
    ${LIBRARY_SOURCE_DIR}/animal.cc
)

add_library(${LIBRARY_NAME} SHARED
    ${LIBRARY_HEADERS}
    ${LIBRARY_SOURCE}
)

target_include_directories(${LIBRARY_NAME} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${LIBRARY_NAME}>
    $<INSTALL_INTERFACE:include/${LIBRARY_NAME}>
)

target_include_directories(${LIBRARY_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# add_subdirectory(test)
set(LIB_DIR /home/nog6hc/workspace/testing/cpp_oop_template/build)
install (TARGETS ${LIBRARY_NAME} LIBRARY DESTINATION ${LIB_DIR})