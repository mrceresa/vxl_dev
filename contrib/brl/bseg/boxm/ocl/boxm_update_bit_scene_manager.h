#ifndef boxm_update_bit_scene_manager_h_
#define boxm_update_bit_scene_manager_h_
//:
// \file
#include <vcl_string.h>
#include <vcl_iostream.h>
#include <vcl_vector.h>
#include <vnl/vnl_vector_fixed.h>
#include <vbl/vbl_array_2d.h>
#include <bocl/bocl_manager.h>
#include <bocl/bocl_utils.h>
#include <boxm/ocl/boxm_ocl_bit_scene.h>

#include <vil/vil_image_view.h>
#include <vpgl/vpgl_perspective_camera.h>
#include <vul/vul_file_iterator.h>

class boxm_update_bit_scene_manager : public bocl_manager<boxm_update_bit_scene_manager>
{
  typedef vnl_vector_fixed<int, 4> int4;
  typedef vnl_vector_fixed<float, 16> float16;
  typedef vnl_vector_fixed<int, 2> int2;

 public:
  typedef float obs_type;

  boxm_update_bit_scene_manager() :
    program_(0),
    render_kernel_(0),
    refine_kernel_(0) {}
  ~boxm_update_bit_scene_manager() {
    if (program_)
      clReleaseProgram(program_);
  }

  //----------------------------------------------------------------------------
  // Setup/deconstruct methods
  //----------------------------------------------------------------------------
  //allocates/deallocates host and gpu side scene buffers 
  //(calls set and clean scene buffers)
  bool init_scene(boxm_ocl_bit_scene * scene,
                  vpgl_camera_double_sptr cam,
                  vil_image_view<float> &obs,
                  float prob_thresh);
  bool uninit_scene(); 
  bool setup_online_processing();
  bool finish_online_processing();
  
  //----------------------------------------------------------------------------
  // Execute methods
  //----------------------------------------------------------------------------
  //update, render, refine publicly callable methods
  bool rendering();
  bool refine();
  bool update();
  
  //: before calling render and update, make sure persp cam is set (maybe combine w/ above methods for simplicity)
  bool set_persp_camera(vpgl_perspective_camera<double> * pcam);
  bool write_persp_camera_buffers();

  //: before calling update, make sure input image, image dimensions and camera are set
  bool set_input_image(vil_image_view<float>  obs);
  bool write_image_buffer();

  //: set 2d workgroup size
  void set_bundle_ni(unsigned bundle_x) {bni_=bundle_x;}
  void set_bundle_nj(unsigned bundle_y) {bnj_=bundle_y;}

  
  //Enqueues a read command for output image and scene.  saves image/scene
  bool read_output_image();
  bool read_scene();
  void save_image();
  bool save_scene();
  
  
  //----------------------------------------------------------------------------
  // Get methods (unsure if when these are used)
  //----------------------------------------------------------------------------
  unsigned wni() const {return wni_;}
  unsigned wnj() const {return wnj_;}
  cl_float * output_image() {return image_;}
    
  //---------------------------------------------------------------------------
  // Appearance density setup - this can be factored out of public and done
  // during setup
  //---------------------------------------------------------------------------
  //set up appearence density
  bool setup_app_density(bool use_uniform=true, float mean = 0.0f, float sigma = 0.0f);
  int setup_app_density_buffer();
  bool clean_app_density();
  int clean_app_density_buffer();
  cl_mem& app_density(){return app_density_buf_;}
  bool setup_norm_data(bool use_uniform=true,float mean = 0.0f, float sigma = 0.0f);
  bool clean_norm_data();
  
  //set offset buffers (offset for update, making sure you don't run into the same
  //block twice
  bool set_offset_buffers(int off_x, int off_y, int factor);
  bool release_offset_buffers();
  
  //----------------------------------------------------------------------------
  //necessary CL items (program, command queue and kernels
  //three separate groups of kernels for 3 separate functions
  //----------------------------------------------------------------------------
  // for pass0 to compute seg len
  cl_program program_;
  cl_command_queue command_queue_;
  cl_kernel render_kernel_;
  cl_kernel refine_kernel_;
  vcl_vector<cl_kernel> update_kernels_;
  
  //----------------------------------------------------------------------------
  // input image (can make this private too, have a setter)
  // MIGHT NEED A THIRD, one for writing out...
  //----------------------------------------------------------------------------
  cl_float * image_;              //input image
  cl_mem     image_buf_;          //input image buf
  cl_uint  * image_gl_;           //gl image (expected image?)
  cl_mem     image_gl_buf_;       //gl image buf

 protected:
  //----------------------------------------------------------------------------
  // PROTECTED helper methods (no need for these to be public, makes api simpler)
  //----------------------------------------------------------------------------
  
  //called by init/uninit: sets/cleans scene buffers
  bool set_scene_buffers();
  bool clean_scene_buffers();
  
  //set kernels/build programs (set kernels calls build programs)
  bool set_kernels();
  bool release_kernels();
  bool build_update_program(vcl_string const& functor, bool use_cell_data);
  bool build_rendering_program();
  bool build_refining_program();
    
  //: executes specified kernel
  bool run(cl_kernel, unsigned pass);
  
  //: sets args for each kernel (should only be called once)
  bool set_args();

  //: set/release command_queue
  bool set_commandqueue();
  bool release_commandqueue();
  bool set_workspace(cl_kernel kernel, unsigned pass);
  
  //persp camera setup
  bool set_persp_camera();
  bool set_persp_camera_buffers();
  bool release_persp_camera_buffers();
  bool clean_persp_camera();

  //set_input_view
  bool set_input_image();
  bool set_input_image_buffers();
  bool set_image_dims_buffers();
  bool release_input_image_buffers();
  bool clean_input_image();

 /*****************************************
  * scene information
  *****************************************/
  RenderSceneInfo * scene_info_;      //world origin, dimensions, block len, buffer size all packed in here
  cl_ushort       * block_ptrs_;      //(int4) 3d array, points to (buff,offset) for each block
  cl_ushort       * mem_ptrs_;        //(int2) points to tree_cells_ free mem
  cl_uchar        * cells_;           //(int2) packed tree cell data
  
  /* optimized data format in 4 buffers */
  cl_float        * cell_alpha_;      //array of floats  (prob density)
  cl_uchar        * cell_mixture_;    //array of uchar8s (mixutre model)
  cl_ushort       * cell_num_obs_;    //array of short4s (num obs counts)
  cl_float        * cell_aux_data_;   //array of float4s (auxiliary data
  
  /* other */
  cl_uchar        * bit_lookup_;
  cl_float          prob_thresh_;     //update prob thresh
  cl_float        * output_debug_;    //output for debugging
  
 /*****************************************
  * update information
  *****************************************/
  cl_uint   * img_dims_;              //input image dimensions

  /* offset for non-overlapping sections */
  cl_uint     offset_x_;
  cl_uint     offset_y_;
  cl_uint     factor_;

  /* bounding box for each tree */
  cl_float * tree_bbox_;

  cl_float* app_density_;
  cl_mem    app_density_buf_;

  // camera
  cl_float * persp_cam_;

  cl_uint bni_;
  cl_uint bnj_;

  cl_int scene_x_;
  cl_int scene_y_;
  cl_int scene_z_;

  // workspace dimensions which will be
  // greater than or equal to image dimensions
  cl_uint wni_;
  cl_uint wnj_;

 /*****************************************
  *pointers to cl memory on GPU
  *****************************************/
  /* scene buffers */
  cl_mem    scene_info_buf_;
  cl_mem    block_ptrs_buf_;
  cl_mem    mem_ptrs_buf_;
  cl_mem    cells_buf_;
  cl_mem    cell_alpha_buf_;
  cl_mem    cell_mixture_buf_;
  cl_mem    cell_num_obs_buf_;
  cl_mem    cell_aux_data_buf_;
  cl_mem    bit_lookup_buf_;

  /* update buffers */
  cl_mem    tree_bbox_buf_;
  cl_mem    persp_cam_buf_;
  cl_mem    img_dims_buf_;
  cl_mem    offset_x_buf_;
  cl_mem    offset_y_buf_;
  cl_mem    factor_buf_;

  /*debugger ... */
  cl_mem    output_debug_buf_;

 /*****************************************
  *helper member variables
  *****************************************/
  boxm_ocl_bit_scene* scene_;
  vpgl_camera_double_sptr cam_;
  vil_image_view<float>  input_img_;
  vil_image_view<float>  expected_img_;

  float gpu_time_;
  vcl_size_t globalThreads[2];
  vcl_size_t localThreads[2];
};

#endif // boxm_update_bit_scene_manager_h_