add_library(lexer SHARED  ${CMAKE_CURRENT_LIST_DIR}/lexer.cpp)
target_include_directories(lexer PRIVATE ${INCLUDE_PROJECT_DIR})
target_link_libraries(lexer)

# codegenVisior
add_library(codegenVisitor SHARED ${CMAKE_CURRENT_LIST_DIR}/codegenVisitor.cpp)
target_include_directories(codegenVisitor PRIVATE ${INCLUDE_PROJECT_DIR})

# parser
add_library(parser SHARED ${CMAKE_CURRENT_LIST_DIR}/parser.cpp)
target_include_directories(parser PRIVATE ${INCLUDE_PROJECT_DIR})

# AST library
add_library(AST SHARED ${CMAKE_CURRENT_LIST_DIR}/AST.cpp)
target_include_directories(AST PRIVATE ${INCLUDE_PROJECT_DIR})

# Main file
add_executable(main ${CMAKE_CURRENT_LIST_DIR}/main.cpp)
target_link_libraries(main PUBLIC ${llvm_libs} PRIVATE lexer parser codegenVisitor AST taocpp::pegtl)
target_include_directories(main PRIVATE ${INCLUDE_PROJECT_DIR})