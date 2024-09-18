cmake_minimum_required(VERSION 3.22)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
add_subdirectory(externals/)
# LLVM-STUFF
find_package(LLVM REQUIRED CONFIG)
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS_LIST})
llvm_map_components_to_libnames(llvm_libs native orcjit core support irreader)

foreach(target ${LLVM_TARGETS_TO_BUILD})
  list(APPEND llvm_targets "LLVM${target}CodeGen")
endforeach()




option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

# Find ccache
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    set(CMAKE_CUDA_COMPILER_LAUNCHER "${CCACHE_PROGRAM}") # CMake 3.9+
endif()
project(sammine_lang )


# Use lld linker
add_link_options("-fuse-ld=lld")



# PROJECT_INCLUDE_DIR
SET(INCLUDE_PROJECT_DIR "${CMAKE_SOURCE_DIR}/include/")

add_subdirectory(src)

enable_testing()
add_subdirectory(unit-tests)
add_subdirectory(e2e-tests)





