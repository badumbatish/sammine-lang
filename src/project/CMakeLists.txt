add_executable(main ${CMAKE_CURRENT_LIST_DIR}/main.cpp)


add_library(lexer  ${CMAKE_CURRENT_LIST_DIR}/lexer.cpp)
target_include_directories(lexer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/project/)