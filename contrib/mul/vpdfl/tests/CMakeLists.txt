# mul/vpdfl/tests/CMakeLists.txt

ADD_EXECUTABLE( vpdfl_test_all
  test_driver.cxx
  test_axis_gaussian.cxx
  test_gaussian.cxx
  test_mixture.cxx
  test_pc_gaussian.cxx
  )
TARGET_LINK_LIBRARIES( vpdfl_test_all vpdfl mbl vnl_io vsl vpl testlib )

ADD_TEST( vpdfl_test_axis_gaussian ${EXECUTABLE_OUTPUT_PATH}/vpdfl_test_all test_axis_gaussian )
ADD_TEST( vpdfl_test_gaussian ${EXECUTABLE_OUTPUT_PATH}/vpdfl_test_all test_gaussian )
ADD_TEST( vpdfl_test_mixture ${EXECUTABLE_OUTPUT_PATH}/vpdfl_test_all test_mixture )
ADD_TEST( vpdfl_test_pc_gaussian ${EXECUTABLE_OUTPUT_PATH}/vpdfl_test_all test_pc_gaussian )

ADD_EXECUTABLE( vpdfl_test_include test_include.cxx )
TARGET_LINK_LIBRARIES( vpdfl_test_include vpdfl )
