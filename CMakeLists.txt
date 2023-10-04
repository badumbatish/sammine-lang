cmake_minimum_required(VERSION 3.24)

# https://cmake.org/cmake/help/latest/policy/CMP0135.html
# Set CMP0135 to NEW for fetchContent
cmake_policy(SET CMP0135 NEW)

project(sammine_lang)

include(src/CMakeLists.txt)
include(integration_tests/CMakeLists.txt)
include_directories(${CMAKE_SOURCE_DIR}/include/project/) # For including headers-path

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)








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
