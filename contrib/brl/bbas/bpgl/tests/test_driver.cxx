#include <testlib/testlib_register.h>


DECLARE(test_segmented_rolling_shutter_camera );
DECLARE(test_poly_radial_distortion );
DECLARE(test_kml_camera );

void
register_tests()
{
  REGISTER(test_segmented_rolling_shutter_camera  );
  REGISTER(test_poly_radial_distortion  );
  REGISTER(test_kml_camera  );
}

DEFINE_MAIN;


