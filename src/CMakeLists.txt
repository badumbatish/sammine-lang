
add_library(project_options INTERFACE)
target_include_directories(project_options INTERFACE 
  ${INCLUDE_PROJECT_DIR}
)
target_link_libraries(project_options INTERFACE 
  LLVMSupport
  ${llvm_libs}
  fmt::fmt
)

# Function to add a library with common settings
function(add_project_library name)
    add_library(${name} OBJECT ${CMAKE_CURRENT_LIST_DIR}/${name}.cpp)
    target_link_libraries(${name} PRIVATE project_options)
endfunction()

add_library(Utilities OBJECT ${CMAKE_CURRENT_LIST_DIR}/Utilities.cpp)
target_link_libraries(Utilities PRIVATE project_options fmt::fmt cpptrace-lib)

add_project_library(Ast)

add_project_library(Lexer)

add_project_library(Parser)
add_project_library(SammineJIT)
add_project_library(CodegenVisitor)
add_project_library(ScopeGeneratorVisitor)
add_project_library(BiTypeChecker)
add_project_library(Types)

add_library(Compiler OBJECT ${CMAKE_CURRENT_LIST_DIR}/Compiler.cpp)
target_link_libraries(Compiler PRIVATE project_options fmt)

add_executable(sammine ${CMAKE_CURRENT_LIST_DIR}/sammine.cpp)
target_include_directories(sammine PRIVATE ${INCLUDE_PROJECT_DIR} ${LLVM_INCLUDE_DIRS})
target_link_libraries(sammine Compiler SammineJIT Utilities Ast Lexer Parser 
  ${llvm_targets} CodegenVisitor ScopeGeneratorVisitor BiTypeChecker Types LLVMSupport ${llvm_libs} fmt argparse)
