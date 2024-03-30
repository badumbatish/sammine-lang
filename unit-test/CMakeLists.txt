Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.4.0 # or a later release
)

FetchContent_MakeAvailable(Catch2)

file(GLOB_RECURSE test_sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

set(lib_source Lexer Parser)

add_executable(tests)
target_sources(tests PRIVATE ${test_sources})
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain ${lib_source})
target_include_directories(tests PRIVATE ${INCLUDE_PROJECT_DIR})

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)

catch_discover_tests(tests)