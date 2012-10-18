//This is brl/bseg/boxm2/volm/boxm2_volm_wr3db_index.h
#ifndef boxm2_volm_wr3db_index_h_
#define boxm2_volm_wr3db_index_h_
//:
// \file
// \brief  A class to index a given voxel world for fast matching to volumetric queries
//         for each location hypothesis, keep a vector of values for the first layer of the container with vmin*2 resolution voxels
//         this is a "condensed" index for the container
//         the indexed values are the depth along the ray traced towards that voxel in the world, 
//         however the depth itself is not saved, the interval corresponding to that depth in the container is saved
//         the number of intervals is assumed to less than 255 so a vector of char values is stored for each location hypothesis
//         the full container is filled using this "condensed" index during matching
//         this class also has a method to "inflate" the index computing vis and prob values to full size container given its condensed form
//           when inflated, a vector of char values is returned for each voxel in the container
//                          the last bit of char is visibility, 0 or 1
//                          second to last bit is probability, 0 for unoccupied, 1 for occupied
//
// \author Ozge C. Ozcanli 
// \date October 07, 2012
// \verbatim
//   Modifications
//
// \endverbatim
// 

#include <vbl/vbl_ref_count.h>
#include <bbas/volm/volm_spherical_container.h>
#include <bbas/volm/volm_spherical_container_sptr.h>
#include <boxm2/boxm2_scene.h>
#include <boxm2/volm/boxm2_volm_locations_sptr.h>


class boxm2_volm_wr3db_index : public vbl_ref_count
{
  public:
    boxm2_volm_wr3db_index(volm_spherical_container_sptr cont);
    ~boxm2_volm_wr3db_index();
    
    //: create the index for a given location in the scene
    bool index_location(boxm2_scene_sptr scene, vgl_point_3d<float>& loc, vcl_vector<char>& values);
    
    //: create a condensed index for given locations of a scene and write as binary, use the vmin layer of spherical container
    bool index_locations(boxm2_scene_sptr scene, boxm2_volm_loc_hypotheses_sptr h);
    bool write_index(vcl_string out_file);
    bool read_index(vcl_string in_file);
    
    //: inflate the index for ith location and return a vector of char values where last bit is visiblity and second to last is prob (occupied or not)
    bool inflate_index_vis_and_prob(unsigned i, vcl_vector<char>& vis_prob);
   
  protected:
    volm_spherical_container_sptr cont_;
    unsigned int voxels_size_;
    unsigned int offset_;     // offset of first layer with min_res*2 resolution
    unsigned int end_offset_; 
     
    //: keep a value in [0-255] to code the index of the distance layer in the container
    vcl_vector< vcl_vector<char> > index_;  
};

#endif  // boxm2_volm_wr3db_index_h_
