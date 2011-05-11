#ifndef boxm2_ocl_render_expected_image_function_h_
#define boxm2_ocl_render_expected_image_function_h_

#include <bocl/bocl_device.h>
#include <bocl/bocl_kernel.h>
#include <boxm2/boxm2_scene.h>
#include <boxm2/boxm2_block.h>
#include <boxm2/ocl/boxm2_opencl_cache.h>

float render_expected_image( boxm2_scene_sptr & scene,
                            bocl_device_sptr & device,
                            boxm2_opencl_cache_sptr & opencl_cache,
                            cl_command_queue & queue,
                            vpgl_camera_double_sptr & cam,
                            bocl_mem_sptr & exp_image,
                            bocl_mem_sptr & vis_image, 
                            bocl_mem_sptr & exp_img_dim,
                            vcl_string data_type,
                            bocl_kernel* kernel,
                            vcl_size_t * lthreads,
                            unsigned cl_ni,
                            unsigned cl_nj );
                            
float render_cone_expected_image( boxm2_scene_sptr & scene,
                                  bocl_device_sptr & device,
                                  boxm2_opencl_cache_sptr & opencl_cache,
                                  cl_command_queue & queue,
                                  vpgl_camera_double_sptr & cam,
                                  bocl_mem_sptr & exp_image,
                                  bocl_mem_sptr & vis_image, 
                                  bocl_mem_sptr & exp_img_dim,
                                  vcl_string data_type,
                                  bocl_kernel* kernel,
                                  vcl_size_t * lthreads,
                                  unsigned cl_ni,
                                  unsigned cl_nj );

#endif // boxm2_ocl_render_expected_image_function_h_
