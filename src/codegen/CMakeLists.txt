
add_project_library(SammineJIT)
add_project_library(CodegenVisitor)
add_project_library(TypeConverter)

add_library(_codegen)
target_include_directories(_codegen PRIVATE ${INCLUDE_PROJECT_DIR} ${LLVM_INCLUDE_DIRS})
target_link_libraries(_codegen SammineJIT CodegenVisitor TypeConverter LLVMSupport ${llvm_libs} ${llvm_targets})
