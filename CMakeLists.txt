
cmake_minimum_required(VERSION 4.0)

project("Test Cpp" VERSION 1.0.0 LANGUAGES CXX)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Place all runtime outputs (exe + DLLs) in the same directory so the
# executable can find shared libraries without PATH manipulation.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

option(CPP_OOP_ENABLE_WARNINGS "Enable the project warning profile" ON)

add_compile_definitions(
    PROJECT_NAME="${PROJECT_NAME}"
    PROJECT_VERSION="${PROJECT_VERSION}"
)

message(STATUS "Host system: ${CMAKE_HOST_SYSTEM_NAME}/${CMAKE_HOST_SYSTEM_PROCESSOR}")
message(STATUS "Target system: ${CMAKE_SYSTEM_NAME}/${CMAKE_SYSTEM_PROCESSOR}")

if(CMAKE_CROSSCOMPILING)
    message(STATUS "Cross-compiling is enabled")
endif()

if(UNIX AND NOT APPLE)
    set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
    set(CMAKE_INSTALL_RPATH "$ORIGIN/lib:$ORIGIN/../lib:$ORIGIN")
endif()

if(CPP_OOP_ENABLE_WARNINGS)
    add_library(project_warnings INTERFACE)

    if(MSVC)
        target_compile_options(project_warnings INTERFACE /W4 /permissive-)
    else()
        target_compile_options(project_warnings INTERFACE
            -Wall
            -Wextra
            -Wpedantic
            -Wconversion
            -Wsign-conversion
            -Wshadow
            -Wdouble-promotion
            -Wformat-security
            -Wnull-dereference
        )

        if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            target_compile_options(project_warnings INTERFACE
                -Wduplicated-cond
                -Wfloat-equal
            )
        elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
            target_compile_options(project_warnings INTERFACE -Wfloat-equal)
        endif()
    endif()
endif()

add_subdirectory(libs/helper)
add_subdirectory(libs/animal)
add_subdirectory(libs/sqlite)

add_executable(test-cpp src/main.cc)

target_link_libraries(test-cpp PRIVATE ${CMAKE_DL_LIBS} helper animal sqlite)

if(TARGET project_warnings)
    target_link_libraries(test-cpp PRIVATE project_warnings)
endif()

# On Windows with shared libraries, copy all required DLLs next to the
# executable after each build so it can run directly from the build tree.
if(WIN32)
    add_custom_command(
        TARGET test-cpp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:test-cpp>
            $<TARGET_FILE_DIR:test-cpp>
        COMMAND_EXPAND_LISTS
    )
endif()

install(TARGETS test-cpp RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
