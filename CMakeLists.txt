cmake_minimum_required(VERSION 3.22)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



project(sammine_lang)


# LLVM-STUFF
find_package(LLVM REQUIRED CONFIG)
#include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})
llvm_map_components_to_libnames(llvm_libs native orcjit support core irreader)

# PROJECT_INCLUDE_DIR
SET(INCLUDE_PROJECT_DIR "${CMAKE_SOURCE_DIR}/include/")

include(src/CMakeLists.txt)
include(unit-test/CMakeLists.txt)
