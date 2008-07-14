#include <testlib/testlib_register.h>
//#include <mipa/mipa_add_all_loaders.h>
DECLARE( test_edge_finder );
DECLARE( test_norm_corr1d );
DECLARE( test_norm_corr2d );
DECLARE( test_profile_pdf );
DECLARE( test_region_pdf );
DECLARE( test_region_finder );
DECLARE( test_pose );
DECLARE( test_mr_point_finder );
DECLARE( test_dp_snake );

void register_tests()
{
//    mipa_add_all_loaders();
  REGISTER( test_edge_finder );
  REGISTER( test_norm_corr1d );
  REGISTER( test_norm_corr2d );
  REGISTER( test_profile_pdf );
  REGISTER( test_region_pdf );
  REGISTER( test_region_finder );
  REGISTER( test_pose );
  REGISTER( test_mr_point_finder );
  REGISTER( test_dp_snake );
}


DEFINE_MAIN;
