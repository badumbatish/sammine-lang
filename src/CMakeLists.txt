
#include_directories(${LLVM_INCLUDE_DIRS})
add_library(project_options INTERFACE)
target_include_directories(project_options INTERFACE 
  ${INCLUDE_PROJECT_DIR}
  ${LLVM_INCLUDE_DIRS}
)
target_link_libraries(project_options INTERFACE 
  LLVMSupport
  ${llvm_libs}
)

# Function to add a library with common settings
function(add_project_library name)
    add_library(${name} OBJECT ${CMAKE_CURRENT_LIST_DIR}/${name}.cpp)
    target_link_libraries(${name} PRIVATE project_options)
endfunction()

function(target_set_warning_as_error TARGET_NAME)
if(MSVC)
  target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX /wd4100)
else()
  target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror -Wno-unused-parameter)
endif()
endfunction()


add_library(Utilities OBJECT ${CMAKE_CURRENT_LIST_DIR}/Utilities.cpp)
target_link_libraries(Utilities PRIVATE project_options fmt::fmt cpptrace-lib)

add_subdirectory(AST)
add_subdirectory(MIR)

add_project_library(Lexer)

add_project_library(Parser)
add_project_library(SammineJIT)

add_library(Compiler OBJECT ${CMAKE_CURRENT_LIST_DIR}/Compiler.cpp)
target_link_libraries(Compiler PRIVATE project_options fmt)

add_executable(sammine ${CMAKE_CURRENT_LIST_DIR}/sammine.cpp)
target_include_directories(sammine PRIVATE ${INCLUDE_PROJECT_DIR} ${LLVM_INCLUDE_DIRS})
target_link_libraries(sammine Compiler SammineJIT Utilities AST Lexer Parser 
  ${llvm_targets} project_options fmt argparse)
