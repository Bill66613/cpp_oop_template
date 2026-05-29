cmake_minimum_required(VERSION 4.0.0)

project(Greeter LANGUAGES CXX)

set(LIBRARY_NAME greeter)

# ── Dependency check ─────────────────────────────────────────────────────────
# CMAKE_PREFIX_PATH is already extended by the top-level CMakeLists.txt to
# include the vcpkg_installed triplet tree.  If gRPC is not installed yet,
# skip this library silently; the caller (test-cpp) detects the missing
# target via if(TARGET greeter).
find_package(gRPC   CONFIG QUIET)
find_package(Protobuf CONFIG QUIET)

if(NOT gRPC_FOUND OR NOT Protobuf_FOUND)
    message(STATUS
        "greeter: gRPC/Protobuf not found — library skipped.\n"
        "         Install via vcpkg manifest: cmake --preset local-windows-vcpkg")
    return()
endif()

# ── Proto code generation ─────────────────────────────────────────────────────
set(_proto_src "${CMAKE_CURRENT_SOURCE_DIR}/proto/greeter.proto")
set(_proto_out "${CMAKE_CURRENT_BINARY_DIR}")

# Suppress warnings on generated sources (protobuf/gRPC emit -W4/-Wall hits).
set(_generated_sources
    "${_proto_out}/greeter.pb.cc"
    "${_proto_out}/greeter.grpc.pb.cc"
)

# Run protoc once to generate both protobuf and gRPC C++ bindings.
add_custom_command(
    OUTPUT
        "${_proto_out}/greeter.pb.cc"
        "${_proto_out}/greeter.pb.h"
        "${_proto_out}/greeter.grpc.pb.cc"
        "${_proto_out}/greeter.grpc.pb.h"
    COMMAND $<TARGET_FILE:protobuf::protoc>
    ARGS
        "--cpp_out=${_proto_out}"
        "--grpc_out=${_proto_out}"
        "--plugin=protoc-gen-grpc=$<TARGET_FILE:gRPC::grpc_cpp_plugin>"
        "-I${CMAKE_CURRENT_SOURCE_DIR}/proto"
        "${_proto_src}"
    DEPENDS
        "${_proto_src}"
        protobuf::protoc
        gRPC::grpc_cpp_plugin
    COMMENT "Generating C++/gRPC bindings from greeter.proto"
    VERBATIM
)

# Disable warnings on generated files — they are not our code.
set_source_files_properties(${_generated_sources} PROPERTIES
    COMPILE_OPTIONS "$<IF:$<CXX_COMPILER_ID:MSVC>,/w,-w>"
)

# ── Library target ────────────────────────────────────────────────────────────
add_library(${LIBRARY_NAME} STATIC
    ${_generated_sources}
    src/greeter_server.cc
    src/greeter_client.cc
)

if(TARGET project_warnings)
    # Apply warnings only to our own sources, not the generated ones
    # (handled above via set_source_files_properties).
    target_link_libraries(${LIBRARY_NAME} PRIVATE project_warnings)
endif()

target_include_directories(${LIBRARY_NAME}
    PRIVATE
        # Generated headers live in the binary dir; only needed by the .cc files.
        "${_proto_out}"
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${LIBRARY_NAME}>
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(${LIBRARY_NAME}
    PUBLIC
        gRPC::grpc++
        protobuf::libprotobuf
)

install(TARGETS ${LIBRARY_NAME} ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
