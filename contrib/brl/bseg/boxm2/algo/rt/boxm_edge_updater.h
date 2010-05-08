#ifndef boxm2_edge_updater_h_
#define boxm2_edge_updater_h_

#include <vcl_vector.h>
#include <vcl_string.h>

#include <boxm2/boxm_aux_traits.h>
#include <boxm2/boxm_apm_traits.h>
#include <boxm2/boxm_scene.h>

#include <boxm2/sample/boxm_edge_sample.h>

template<class T_loc, class APM, class AUX>
class boxm_edge_updater
{
 public:
  boxm_edge_updater(boxm_scene<boct_tree<T_loc, boxm_edge_sample<APM> > > &scene,
                    vcl_vector<vcl_string> const& image_ids);

  ~boxm_edge_updater(){}

  bool add_cells();

 protected:
  vcl_vector<vcl_string> image_ids_;

  boxm_scene<boct_tree<T_loc, boxm_edge_sample<APM> > > &scene_;
};

#endif
