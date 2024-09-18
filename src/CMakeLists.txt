
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

add_project_library(Utilities)

add_project_library(Ast)

add_project_library(Lexer)

add_project_library(Parser)
add_project_library(SammineJIT)
add_project_library(CodegenVisitor)
add_project_library(ScopeGeneratorVisitor)
add_project_library(Compiler)


add_library(basics INTERFACE)
target_link_libraries(basics INTERFACE Utilities Ast Lexer Parser)

add_library(visitors INTERFACE)
target_link_libraries(visitors INTERFACE CodegenVisitor ScopeGeneratorVisitor)

add_executable(sammine ${CMAKE_CURRENT_LIST_DIR}/sammine.cpp)
target_include_directories(sammine PRIVATE ${INCLUDE_PROJECT_DIR} ${LLVM_INCLUDE_DIRS})
target_link_libraries(sammine Compiler SammineJIT Utilities Ast Lexer Parser 
    ${llvm_targets} CodegenVisitor ScopeGeneratorVisitor LLVMSupport ${llvm_libs} )
