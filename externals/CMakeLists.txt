include(FetchContent)

# set(PROJECT_BUILD_TYPE ${CMAKE_BUILD_TYPE})
#
# set(CMAKE_BUILD_TYPE Release)
message("Setting external dependencies build type to : ${CMAKE_BUILD_TYPE}")
FetchContent_Declare(
  cpptrace
  GIT_REPOSITORY https://github.com/jeremy-rifkin/cpptrace.git
  GIT_TAG        v0.7.1 # <HASH or TAG>
  SYSTEM
)
FetchContent_MakeAvailable(cpptrace)



FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG        9cf9f38eded63e5e0fb95cd536ba51be601d7fa2 # 10.2.1
  )
FetchContent_MakeAvailable(fmt)


# fetch latest argparse
FetchContent_Declare(
    argparse
    GIT_REPOSITORY https://github.com/p-ranav/argparse.git
    SYSTEM
)
set(ARGPARSE_BUILD_TESTS=off CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(argparse)

# set(CMAKE_BUILD_TYPE ${PROJECT_BUILD_TYPE})
# message("Done with setting external dependencies, setting build type back to ${CMAKE_BUILD_TYPE}")



