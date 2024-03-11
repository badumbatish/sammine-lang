add_library(Lexer ${CMAKE_CURRENT_LIST_DIR}/Lexer.cpp)
target_include_directories(Lexer PRIVATE ${INCLUDE_PROJECT_DIR})

add_library(Ast ${CMAKE_CURRENT_LIST_DIR}/Ast.cpp)
target_include_directories(Ast PRIVATE ${INCLUDE_PROJECT_DIR})

add_executable(sammine ${CMAKE_CURRENT_LIST_DIR}/sammine.cpp)
target_link_libraries(sammine Lexer)
target_include_directories(sammine PRIVATE ${INCLUDE_PROJECT_DIR})