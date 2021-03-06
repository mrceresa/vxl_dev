#include <testlib/testlib_test.h>
#include <boxm2/volm/boxm2_volm_locations.h>
#include <vpgl/vpgl_lvcs_sptr.h>
#include <vpgl/vpgl_lvcs.h>
#include <boxm2/boxm2_scene.h>
#include <vil/vil_load.h>
#include <boxm2/volm/boxm2_volm_locations.h>
#include <boxm2/volm/boxm2_volm_locations_sptr.h>

static void test_volm_locations()
{
  vcl_vector<volm_tile> tiles = volm_tile::generate_p1_wr1_tiles();
  boxm2_volm_loc_hypotheses h(tiles[0]);
  h.locs_.push_back(vgl_point_3d<float>(1,1,1));
  h.locs_.push_back(vgl_point_3d<float>(1,2,3));
  h.pixels_.push_back(vcl_pair<unsigned, unsigned>(2,2));
  h.pixels_.push_back(vcl_pair<unsigned, unsigned>(20,32));
  
  h.write_hypotheses("test.bin");
  boxm2_volm_loc_hypotheses h2("test.bin");
  
  TEST("# of hypotheses locs ", h2.locs_.size(), 2);
  TEST("# of hypotheses pixels ", h2.pixels_.size(), 2);
  TEST("tile hemi ", h.tile_.hemisphere_, h2.tile_.hemisphere_);
  double lat, lon; h.tile_.cam_.img_to_global(100, 100, lon, lat); 
  double lat2, lon2; h2.tile_.cam_.img_to_global(100, 100, lon2, lat2); 
  TEST("tile cam img to global lat ", lat, lat2);
  TEST("tile cam img to global lon ", lon, lon2);

}

TESTMAIN(test_volm_locations);


