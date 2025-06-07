
add_library(compiler OBJECT ${CMAKE_CURRENT_LIST_DIR}/Compiler.cpp)
target_link_libraries(compiler PRIVATE project_options fmt)

