# Add the test source files to CMake so that CLion recognises them properly.
FILE(GLOB_RECURSE TEST_LIST
        ${CMAKE_CURRENT_LIST_DIR}/test/*.*
)

list(APPEND SRC_LIST ${TEST_LIST})
