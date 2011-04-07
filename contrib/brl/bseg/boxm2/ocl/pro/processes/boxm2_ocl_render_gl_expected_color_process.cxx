// This is brl/bseg/boxm2/ocl/pro/processes/boxm2_ocl_render_gl_expected_color_process.cxx
//:
// \file
// \brief  A process for rendering the scene.
//
// \author Vishal Jain
// \date Mar 10, 2011

#include <bprb/bprb_func_process.h>

#include <vcl_fstream.h>
#include <vcl_algorithm.h>
#include <boxm2/ocl/boxm2_opencl_cache.h>
#include <boxm2/boxm2_scene.h>
#include <boxm2/boxm2_block.h>
#include <boxm2/boxm2_data_base.h>
#include <boxm2/ocl/boxm2_ocl_util.h>
#include <vil/vil_save.h>
#include <vil/vil_image_view.h>
//brdb stuff
#include <brdb/brdb_value.h>

//directory utility
#include <vul/vul_timer.h>
#include <vcl_where_root_dir.h>
#include <bocl/bocl_device.h>
#include <bocl/bocl_kernel.h>
#include <boxm2/ocl/algo/boxm2_ocl_render_expected_image_function.h>

namespace boxm2_ocl_render_gl_expected_color_process_globals
{
  const unsigned n_inputs_ = 8 ;
  const unsigned n_outputs_ = 0;
  vcl_size_t lthreads[2]={8,8};

  static vcl_map<vcl_string,vcl_vector<bocl_kernel*> > kernels;

  void compile_kernel(bocl_device_sptr device,vcl_vector<bocl_kernel*> & vec_kernels, vcl_string opts)
  {
    //gather all render sources... seems like a lot for rendering...
    vcl_vector<vcl_string> src_paths;
    vcl_string source_dir = vcl_string(VCL_SOURCE_ROOT_DIR) + "/contrib/brl/bseg/boxm2/ocl/cl/";
    src_paths.push_back(source_dir + "scene_info.cl");
    src_paths.push_back(source_dir + "pixel_conversion.cl");
    src_paths.push_back(source_dir + "bit/bit_tree_library_functions.cl");
    src_paths.push_back(source_dir + "backproject.cl");
    src_paths.push_back(source_dir + "statistics_library_functions.cl");
    src_paths.push_back(source_dir + "ray_bundle_library_opt.cl");
    src_paths.push_back(source_dir + "bit/render_rgb.cl");
    src_paths.push_back(source_dir + "bit/cast_ray_bit.cl");

    //set kernel options
    //#define STEP_CELL step_cell_render(mixture_array, alpha_array, data_ptr, d, &vis, &expected_int);
    vcl_string options = opts + " -D RENDER ";
    options += " -D DETERMINISTIC ";
    options += " -D STEP_CELL=step_cell_render(aux_args,data_ptr,llid,d)";

    //have kernel construct itself using the context and device
    bocl_kernel * ray_trace_kernel=new bocl_kernel();
    ray_trace_kernel->create_kernel( &device->context(),
                                     device->device_id(),
                                     src_paths,
                                     "render_bit_scene",   //kernel name
                                     options,              //options
                                     "boxm2 opencl render random blocks"); //kernel identifier (for error checking)
    vec_kernels.push_back(ray_trace_kernel);
    
    //create normalize image kernel
    vcl_vector<vcl_string> norm_src_paths;
    norm_src_paths.push_back(source_dir + "pixel_conversion.cl");
    norm_src_paths.push_back(source_dir + "bit/normalize_kernels.cl");
    bocl_kernel * normalize_render_kernel=new bocl_kernel();
    normalize_render_kernel->create_kernel( &device->context(),
                                            device->device_id(),
                                            norm_src_paths,
                                            "normalize_render_kernel_rgb_gl",   //kernel name
                                            " -D NORMALIZE_RENDER_GL ",              //options
                                            "normalize render kernel"); //kernel identifier (for error checking)

    vec_kernels.push_back(normalize_render_kernel);
  }

}

bool boxm2_ocl_render_gl_expected_color_process_cons(bprb_func_process& pro)
{
  using namespace boxm2_ocl_render_gl_expected_color_process_globals;

  //process takes 1 input
  vcl_vector<vcl_string> input_types_(n_inputs_);
  input_types_[0] = "bocl_device_sptr";
  input_types_[1] = "boxm2_scene_sptr";
  input_types_[2] = "boxm2_opencl_cache_sptr";
  input_types_[3] = "vpgl_camera_double_sptr";
  input_types_[4] = "unsigned";
  input_types_[5] = "unsigned";
  input_types_[6] = "bocl_mem_sptr"; // exp image buffer;
  input_types_[7] = "bocl_mem_sptr"; // exp image dimensions buffer;


  vcl_vector<vcl_string> output_types_(n_outputs_);

  return pro.set_input_types(input_types_) && pro.set_output_types(output_types_);
}

bool boxm2_ocl_render_gl_expected_color_process(bprb_func_process& pro)
{
  using namespace boxm2_ocl_render_gl_expected_color_process_globals;

  if ( pro.n_inputs() < n_inputs_ ){
    vcl_cout << pro.name() << ": The input number should be " << n_inputs_<< vcl_endl;
    return false;
  }
  //get the inputs
  unsigned i = 0;
  bocl_device_sptr device= pro.get_input<bocl_device_sptr>(i++);
  boxm2_scene_sptr scene =pro.get_input<boxm2_scene_sptr>(i++);

  boxm2_opencl_cache_sptr opencl_cache= pro.get_input<boxm2_opencl_cache_sptr>(i++);
  vpgl_camera_double_sptr cam= pro.get_input<vpgl_camera_double_sptr>(i++);
  unsigned ni=pro.get_input<unsigned>(i++);
  unsigned nj=pro.get_input<unsigned>(i++);
  bocl_mem_sptr exp_image =pro.get_input<bocl_mem_sptr>(i++);
  bocl_mem_sptr exp_img_dim =pro.get_input<bocl_mem_sptr>(i++);

  bool foundDataType = false;
  vcl_string data_type,options;
  if( scene->has_data_type(boxm2_data_traits<BOXM2_GAUSS_RGB>::prefix()) ) {
    data_type = boxm2_data_traits<BOXM2_GAUSS_RGB>::prefix(); 
    foundDataType = true; 
    options=" -D MOG_TYPE_8 ";
  }
  if(!foundDataType) {
    vcl_cout<<"boxm2_ocl_render_gl_expected_color_process ERROR: scene doesn't have BOXM2_GAUSS_RGB data type"<<vcl_endl;
    return false;
  }

  //: create a command queue.
  int status=0;
  cl_command_queue queue = clCreateCommandQueue(device->context(),*(device->device_id()),
                                                CL_QUEUE_PROFILING_ENABLE,&status);
  if (status!=0) return false;
  vcl_string identifier=device->device_identifier()+options;

  //: compile the kernel
  if (kernels.find(identifier)==kernels.end())
  {
    vcl_cout<<"===========Compiling kernels==========="<<vcl_endl;
    vcl_vector<bocl_kernel*> ks;
    compile_kernel(device,ks,options);
    kernels[identifier]=ks;
  }

  unsigned cl_ni=RoundUp(ni,lthreads[0]);
  unsigned cl_nj=RoundUp(nj,lthreads[1]);
  
  //create float4 image here 
  float* buff = new float[4*cl_ni*cl_nj];
  vcl_fill(buff, buff + 4*cl_ni*cl_nj, 0.0f); 
  bocl_mem_sptr exp_color = new bocl_mem(device->context(), buff, 4*cl_ni*cl_nj*sizeof(float), "color im buffer (float4) buffer");
  exp_color->create_buffer(CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR);

  // visibility image
  float* vis_buff = new float[cl_ni*cl_nj];
  vcl_fill(vis_buff, vis_buff + cl_ni*cl_nj, 1.0f); 
  bocl_mem_sptr vis_image = new bocl_mem(device->context(), vis_buff, cl_ni*cl_nj*sizeof(float), "vis image (single float) buffer");
  vis_image->create_buffer(CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR);

  //: run expected image function
  render_expected_image(scene, device, opencl_cache, queue,
                        cam, exp_color, vis_image, exp_img_dim,
                        data_type, kernels[identifier][0], lthreads, cl_ni, cl_nj);  

  //: normalize and write image to GL buffer
  {
    vcl_size_t gThreads[] = {cl_ni,cl_nj};
    bocl_kernel* norm_rgb_gl = kernels[identifier][1]; 
    norm_rgb_gl->set_arg( exp_color.ptr() );
    norm_rgb_gl->set_arg( vis_image.ptr() );
    norm_rgb_gl->set_arg( exp_img_dim.ptr());
    norm_rgb_gl->set_arg( exp_image.ptr() ); 
    norm_rgb_gl->execute( queue, 2, lthreads, gThreads);
    clFinish(queue);
    norm_rgb_gl->clear_args();
  }
  
  //delete visibilty image
  delete[] vis_buff; 
  
  //: read out expected image
  clReleaseCommandQueue(queue);
  return true;
}
