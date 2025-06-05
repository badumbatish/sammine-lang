
add_library(Compiler OBJECT ${CMAKE_CURRENT_LIST_DIR}/Compiler.cpp)
target_link_libraries(Compiler PRIVATE project_options fmt)

add_executable(sammine ${CMAKE_CURRENT_LIST_DIR}/sammine.cpp)
target_include_directories(sammine PRIVATE ${INCLUDE_PROJECT_DIR} ${LLVM_INCLUDE_DIRS})
target_link_libraries(sammine Compiler SammineJIT Utilities Ast Lexer Parser 
  ${llvm_targets} CodegenVisitor ScopeGeneratorVisitor BiTypeChecker Types AstPrinterVisitor LLVMSupport ${llvm_libs} fmt argparse)
