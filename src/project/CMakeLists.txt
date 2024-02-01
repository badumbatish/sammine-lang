

add_library(lexer SHARED  ${CMAKE_CURRENT_LIST_DIR}/lexer.cpp)
target_include_directories(lexer PRIVATE ${INCLUDE_PROJECT_DIR})
target_link_libraries(lexer PRIVATE taocpp::pegtl)
add_library(parser SHARED ${CMAKE_CURRENT_LIST_DIR}/parser.cpp )

target_link_libraries(parser PRIVATE lexer)
target_include_directories(parser PRIVATE ${INCLUDE_PROJECT_DIR})

add_executable(main "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/AST.cpp"
)
target_link_libraries(main PUBLIC ${llvm_libs} PRIVATE lexer parser)
target_include_directories(main PRIVATE ${INCLUDE_PROJECT_DIR})


