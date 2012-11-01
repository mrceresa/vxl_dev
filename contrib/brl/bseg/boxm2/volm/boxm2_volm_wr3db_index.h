//This is brl/bseg/boxm2/volm/boxm2_volm_wr3db_index.h
#ifndef boxm2_volm_wr3db_index_h_
#define boxm2_volm_wr3db_index_h_
//:
// \file
// \brief  A class to index a given voxel world for fast matching to volumetric queries
//         for each location hypothesis, keep a vector of values given by the spherical shell container 
//         this is a "condensed" index which can be inflated to get a value for each voxel in a spherical container
//         the indexed values are the depth along the ray traced along the directions given by spherical shell container, 
//         however the depth itself is not saved, the interval corresponding to that depth in the spherical container is saved
//         the number of intervals is assumed to be less than 255 so a vector of char values is stored for each location hypothesis
//         
//         the full container can be filled using this "condensed" index during matching for each location
//         this class has a method to "inflate" the index to the full size of the container, by computing vis and prob values 
//           when inflated, a vector of char values is returned for each voxel in the container
//                          the values are one of the combinations: 
//                                     VIS_OCC = 0 visible and occluded, 
//                                     VIS_UNOCC = 1 visible and unoccluded
//                                     NONVIS_UNKNOWN = 2 not visible and occlusion is unknown
//
//         the class supports single-threaded asynchronous I/O, 
//         keeps 2 buffers to support io from one buffer while the other one (active buffer) is being filled/used
//         supports index creation and read as single pass operations on the specified binary files
// 
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
#include <vcl_fstream.h>

class boxm2_volm_wr3db_index : public vbl_ref_count
{
  public:
    enum combinations { VIS_OCC = 0, VIS_UNOCC = 1, NONVIS_UNKNOWN = 2 };
    enum mode { READ = 0, WRITE = 1, NOT_INITIALIZED = 2 };
    
    //: layer_size is the size of index array for each hypothesis, buffer_capacity is the max GBs on RAM for this class to use
    boxm2_volm_wr3db_index(int layer_size, float buffer_capacity);
    ~boxm2_volm_wr3db_index();
    
    //: io as chunks of data to a set of files in the specified folder
    bool initialize_read(vcl_string file_name);
    bool initialize_write(vcl_string file_name);
    bool finalize();
    
    //: return the max number of indices on active cache
    unsigned int buffer_size() { return buffer_size_; }
    unsigned int current_id() { return current_id_; }
    unsigned int current_global_id() { return current_global_id_; }
    
    //: just appends to the end of the current active buffer, nothing about which location hypothesis these values correspond is known, 
    //  caller is responsible to keep the ordering consistent with the hypotheses ordering
    bool add_to_index(vcl_vector<char>& values);
    //: caller is responsible to pass a valid array of size layer_size
    bool add_to_index(char* values); 
    
    //: retrieve the next index, use the active_cache, if all on the active_cache has been retrieved, read from disc
    //  caller is responsible to resize values array to layer_size
    bool get_next(vcl_vector<char>& values);
    //: caller is responsible to pass a valid array of size layer_size
    bool get_next(char* values);
    
    //: binary io
    bool close_file(vcl_string out_file);
    
    //: inflate an index value array and return a vector of chars whose values are one of the combinations VIS_OCC, VIS_UNOCC, NONVIS_UNKNOWN
    static bool inflate_index_vis_and_prob(vcl_vector<char>& values, volm_spherical_container_sptr cont, vcl_vector<char>& vis_prob);
   
  protected:
    unsigned int read_to_buffer(char* buf);
  
    unsigned int layer_size_;     // number of values in an index, this is given by the spherical shell container
    unsigned int buffer_size_;
    unsigned int current_id_;     // id on the active buffer
    unsigned int current_global_id_;  // global id on the entire index object
    mode m_;
    vcl_string file_name_;
    
    char *active_buffer_;  
    
    unsigned long file_size_;
    unsigned long read_so_far_;
    unsigned int active_buffer_size_;  // during reading there may be less than buffer_size_ on the active cache
    
    vcl_fstream f_obj_;
};

#endif  // boxm2_volm_wr3db_index_h_
