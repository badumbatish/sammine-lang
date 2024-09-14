# Add a custom target for running ctest that depends on the lit tests
include(CTest)
# configure_file(lit.cfg.py .)
add_custom_target(e2e-tests
    COMMAND lit --verbose ${CMAKE_SOURCE_DIR}/e2e-tests 
    WORKING_DIRECTORY ${CMAKE_LIST_DIR}
)

