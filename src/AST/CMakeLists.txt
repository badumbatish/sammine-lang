add_library(AST OBJECT Ast.cpp CodegenVisitor.cpp ScopeGeneratorVisitor.cpp)
target_include_directories(AST PRIVATE ${INCLUDE_PROJECT_DIR}
  ${LLVM_INCLUDE_DIRS}
)
target_link_libraries(AST PRIVATE project_options)
target_set_warning_as_error(AST)
