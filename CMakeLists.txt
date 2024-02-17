cmake_minimum_required(VERSION 3.24)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# https://cmake.org/cmake/help/latest/policy/CMP0135.html
# Set CMP0135 to NEW for fetchContent
cmake_policy(SET CMP0135 NEW)
SET(INCLUDE_PROJECT_DIR "${CMAKE_SOURCE_DIR}/include/project/")
SET(INCLUDE_LOGGING_DIR "${CMAKE_SOURCE_DIR}/include")

# Set logging on
add_compile_definitions(SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE SPDLOG_TRACE_ON)

project(sammine_lang)

find_package(LLVM REQUIRED CONFIG)
find_package(pegtl)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Set your project compile flags.
# E.g. if using the C++ header files
# you will need to enable C++11 support
# for your compiler.

include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})
llvm_map_components_to_libnames(llvm_libs native orcjit support core irreader)


include(src/CMakeLists.txt)
include(integration_tests/CMakeLists.txt)
#include_directories(${CMAKE_SOURCE_DIR}/include/) # For including headers-path









# google test

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing() # Should only be in the root folder's CMakeLists.txt
include(GoogleTest) # Should only be in the root folder's CMakeLists.txt
