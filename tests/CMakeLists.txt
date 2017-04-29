find_package(GMock REQUIRED)

set (GTEST_BACKEND_SOURCE
   test_main.cpp
   test_bst.cpp
)

include_directories (${PROJECT_NAME_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/src)

add_executable (gtest-test-bst ${GTEST_BACKEND_SOURCE})

target_link_libraries (
    gtest-test-bst
    pthread
    ${GTEST_BOTH_LIBRARIES}
    ${GMOCK_LIBRARIES}
)

add_custom_target (check COMMAND gtest-test-bst)
