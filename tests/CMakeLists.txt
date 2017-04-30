# Use valgrind if its installed
find_program(VALGRIND valgrind)

if (VALGRIND)
  set(VALGRIND_CMD "${VALGRIND}" "--error-exitcode=1" "--trace-children=yes" "--leak-check=full" "--show-leak-kinds=definite")
  set(VALGRIND_CMD "${VALGRIND}" "--errors-for-leak-kinds=definite" "--track-fds=yes" "--num-callers=128")
endif()

find_package(GMock REQUIRED)

set (GTEST_BACKEND_SOURCE
   test_main.cpp
   test_bst.cpp
)

include_directories (${PROJECT_NAME_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/src)

add_executable (gtest_test_bst ${GTEST_BACKEND_SOURCE})

target_link_libraries (
    gtest_test_bst
    pthread
    ${GTEST_BOTH_LIBRARIES}
    ${GMOCK_LIBRARIES}
)

add_custom_target (check COMMAND ${VALGRIND_CMD} ${CMAKE_BINARY_DIR}/gtest_test_bst)
