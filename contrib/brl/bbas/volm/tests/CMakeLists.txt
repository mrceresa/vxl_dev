# bbas/volm/tests/CMakeLists.txt

ADD_EXECUTABLE( volm_test_all
  test_driver.cxx
  test_spherical_container.cxx
  test_tile.cxx
  test_spherical_shell_container.cxx
)

TARGET_LINK_LIBRARIES( volm_test_all volm testlib vpl )

ADD_TEST(volm_test_spherical_container ${EXECUTABLE_OUTPUT_PATH}/volm_test_all test_spherical_container)
ADD_TEST(volm_test_tile ${EXECUTABLE_OUTPUT_PATH}/volm_test_all test_tile)
ADD_TEST(volm_test_spherical_shell_container ${EXECUTABLE_OUTPUT_PATH}/volm_test_all test_spherical_shell_container)

ADD_EXECUTABLE(volm_test_include test_include.cxx)
TARGET_LINK_LIBRARIES(volm_test_include volm)

