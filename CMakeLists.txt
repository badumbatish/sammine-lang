cmake_minimum_required(VERSION 3.22)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
#option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

project(sammine_lang)
# LLVM-STUFF
find_package(LLVM REQUIRED CONFIG)

# Find ninja
find_program(NINJA_EXECUTABLE ninja)
if(NINJA_EXECUTABLE)
  set(CMAKE_GENERATOR "Ninja")
  message(STATUS "Using Ninja build system")
else()
  message(STATUS "Ninja not found, using default generator")
endif()
# PROJECT_INCLUDE_DIR
SET(INCLUDE_PROJECT_DIR "${CMAKE_SOURCE_DIR}/include/")
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

llvm_map_components_to_libnames(llvm_libs native orcjit support core irreader)

add_subdirectory(src)

enable_testing()
add_subdirectory(unit-test)





