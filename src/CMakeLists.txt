add_executable(sammine ${CMAKE_CURRENT_LIST_DIR}/sammine.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Lexer.cpp)
target_include_directories(sammine PRIVATE ${INCLUDE_PROJECT_DIR})