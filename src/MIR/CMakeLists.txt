
add_library(MIR OBJECT BasicBlock.cpp CFG.cpp)
target_include_directories(MIR PRIVATE ${INCLUDE_PROJECT_DIR}
  ${LLVM_INCLUDE_DIRS}
)
target_link_libraries(MIR PRIVATE project_options)
