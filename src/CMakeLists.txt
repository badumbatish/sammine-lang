add_library(Lexer ${CMAKE_CURRENT_LIST_DIR}/Lexer.cpp)
target_include_directories(Lexer PRIVATE ${INCLUDE_PROJECT_DIR})

add_library(Ast ${CMAKE_CURRENT_LIST_DIR}/Ast.cpp)
target_include_directories(Ast PRIVATE ${INCLUDE_PROJECT_DIR})
target_link_libraries(Ast PRIVATE LLVMSupport)

add_library(Parser ${CMAKE_CURRENT_LIST_DIR}/Parser.cpp)
target_include_directories(Parser PRIVATE ${INCLUDE_PROJECT_DIR})
target_link_libraries(Parser PRIVATE Ast)


add_library(Codegen ${CMAKE_CURRENT_LIST_DIR}/CodegenVisitor.cpp)
target_include_directories(Codegen PRIVATE ${INCLUDE_PROJECT_DIR})
target_link_libraries(Codegen PRIVATE LLVMSupport)

add_library(Compiler ${CMAKE_CURRENT_LIST_DIR}/Compiler.cpp)
target_include_directories(Compiler PRIVATE ${INCLUDE_PROJECT_DIR})
target_link_libraries(Compiler Lexer Parser Codegen)

add_library(SammineJIT ${CMAKE_CURRENT_LIST_DIR}/SammineJIT.cpp)
target_include_directories(SammineJIT PRIVATE ${INCLUDE_PROJECT_DIR})
target_link_libraries(SammineJIT PRIVATE LLVMSupport)

add_executable(sammine ${CMAKE_CURRENT_LIST_DIR}/sammine.cpp)
target_include_directories(sammine PRIVATE ${INCLUDE_PROJECT_DIR})
target_link_libraries(sammine Compiler)
