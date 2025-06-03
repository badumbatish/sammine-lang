add_library(Utilities OBJECT ${CMAKE_CURRENT_LIST_DIR}/Utilities.cpp)
target_link_libraries(Utilities PRIVATE project_options fmt::fmt cpptrace-lib)
