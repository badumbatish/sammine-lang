add_executable(main "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/AST.cpp"
)

target_link_libraries(main PRIVATE lexer parser)

add_library(lexer  ${CMAKE_CURRENT_LIST_DIR}/lexer.cpp)
target_include_directories(lexer PUBLIC ${INCLUDE_PROJECT_DIR})

add_library(parser  ${CMAKE_CURRENT_LIST_DIR}/parser.cpp)
target_include_directories(parser PUBLIC ${INCLUDE_PROJECT_DIR})

