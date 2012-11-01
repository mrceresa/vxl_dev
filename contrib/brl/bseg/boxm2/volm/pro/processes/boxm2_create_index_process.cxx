// This is brl/bseg/boxm2/volm/pro/processes/boxm2_create_index_process.cxx
//:
// \file
// \brief  A process for creating an index for each location hypothesis of a scene
//
// \author Ozge C. Ozcanli
// \date Oct 21, 2012

#include <bprb/bprb_func_process.h>

#include <vcl_fstream.h>
#include <boxm2/boxm2_scene.h>
#include <boxm2/volm/boxm2_volm_wr3db_index.h>
#include <boxm2/volm/boxm2_volm_wr3db_index_sptr.h>
#include <boxm2/volm/boxm2_volm_locations.h>
//#include <bbas/volm/volm_spherical_container.h>
#include <bbas/volm/volm_spherical_shell_container.h>
#include <bbas/volm/volm_spherical_shell_container_sptr.h>

#include <vcl_fstream.h>
#include <boxm2/ocl/boxm2_opencl_cache.h>
#include <boxm2/boxm2_block.h>
#include <boxm2/boxm2_data_base.h>
#include <boxm2/ocl/boxm2_ocl_util.h>
//directory utility
#include <vul/vul_timer.h>
#include <bocl/bocl_device.h>
#include <bocl/bocl_kernel.h>

namespace boxm2_create_index_process_globals
{
  const unsigned n_inputs_ = 12;
  const unsigned n_outputs_ = 0;
  
  void compile_kernel(bocl_device_sptr device,vcl_vector<bocl_kernel*> & vec_kernels)
  {
    //gather all render sources... seems like a lot for rendering...
    vcl_vector<vcl_string> src_paths;
    vcl_string source_dir = boxm2_ocl_util::ocl_src_root();
    src_paths.push_back(source_dir + "scene_info.cl");
    src_paths.push_back(source_dir + "cell_utils.cl");
    src_paths.push_back(source_dir + "bit/bit_tree_library_functions.cl");
    src_paths.push_back(source_dir + "basic/sort_vector.cl");
    src_paths.push_back(source_dir + "backproject.cl");
    src_paths.push_back(source_dir + "statistics_library_functions.cl");
    src_paths.push_back(source_dir + "ray_bundle_library_opt.cl");
    src_paths.push_back(source_dir + "bit/compute_index.cl");
    src_paths.push_back(source_dir + "bit/cast_ray_bit.cl");

    //set kernel options
    //vcl_string options = " -D RENDER_DEPTH -D COMPINDEX ";
    vcl_string options = " -D COMPINDEX ";
    options += " -D STEP_CELL=step_cell_compute_index(tblock,aux_args.alpha,data_ptr,d*linfo->block_len,aux_args.vis,aux_args.expdepth,aux_args.expdepthsqr,aux_args.probsum,aux_args.t)";

    bocl_kernel* compute_index = new bocl_kernel();
    
    compute_index->create_kernel(&device->context(),device->device_id(), src_paths, "compute_loc_index", options, "compute_loc_index");
    vec_kernels.push_back(compute_index);
    
    //create normalize image kernel
    bocl_kernel * norm_kernel=new bocl_kernel();
    if (!norm_kernel) {
      vcl_cout << " cannot allocate kernel object!\n" << vcl_endl; vcl_cout.flush(); }
      else vcl_cout << " got a new kernel object!\n";

    norm_kernel->create_kernel(&device->context(),device->device_id(), src_paths, "normalize_index_depth_kernel", options,
                                            "normalize_index_depth_kernel"); //kernel identifier (for error checking)


    vec_kernels.push_back(norm_kernel);
    
    return ;
  }
  static vcl_map<vcl_string,vcl_vector<bocl_kernel*> > kernels;
}
bool boxm2_create_index_process_cons(bprb_func_process& pro)
{
  using namespace boxm2_create_index_process_globals;

  vcl_vector<vcl_string> input_types_(n_inputs_);
  input_types_[0] = "bocl_device_sptr";
  input_types_[1] = "boxm2_scene_sptr";
  input_types_[2] = "boxm2_opencl_cache_sptr";
  input_types_[3] = "boxm2_volm_loc_hypotheses_sptr";
  input_types_[4] = "float"; // minimum voxel resolution to create spherical container
  input_types_[5] = "float"; // maximum distance in the world that the spherical container will cover
  input_types_[6] = "float"; // the solid angle for the spherical container, the resolution of the voxels will get coarser based on this angle
  input_types_[7] = "float"; // cap angle to create the spherical shell container  -- 180 for full sphere, 90 for half sphere
  input_types_[8] = "float"; // point angle to create the spherical shell container
  input_types_[9] = "vcl_string"; // name of output file to save the index
  input_types_[10] = "float"; // visibility threshold to declare a ray a sky ray, it's strictly very small if occupied, so could be as small as 0.3f
  input_types_[11] = "float"; // buffer capacity on CPU RAM for the indices to be cached before being written to disc in chunks

  vcl_vector<vcl_string>  output_types_(n_outputs_);

  return pro.set_input_types(input_types_) && pro.set_output_types(output_types_);
}

bool boxm2_create_index_process(bprb_func_process& pro)
{
  using namespace boxm2_create_index_process_globals;
  
  vcl_size_t local_threads[2]={8,1};
  vcl_size_t global_threads[2]={0,1};  // global threads size is to be determined later

  //sanity check inputs
  if ( pro.n_inputs() < n_inputs_ ) {
    vcl_cout << pro.name() << ": The input number should be " << n_inputs_<< vcl_endl;
    return false;
  }
  float transfer_time=0.0f;
  float gpu_time=0.0f;

  if ( pro.n_inputs() < n_inputs_ ){
    vcl_cout << pro.name() << ": The input number should be " << n_inputs_<< vcl_endl;
    return false;
  }
  //get the inputs
  unsigned i = 0;
  bocl_device_sptr device = pro.get_input<bocl_device_sptr>(i++);
  vcl_cout << " device: " << device->info().device_name_ << " total memory: " << device->info().total_global_memory_ << "\n";
  vcl_cout << " max allowed work items in a group: " << device->info().max_work_group_size_ << "\n";
  vcl_cout << " max work item sizes in each dimensions: " << device->info().max_work_item_sizes_ << "\n";
  boxm2_scene_sptr scene = pro.get_input<boxm2_scene_sptr>(i++);
  boxm2_opencl_cache_sptr  opencl_cache = pro.get_input<boxm2_opencl_cache_sptr>(i++);
  boxm2_volm_loc_hypotheses_sptr hyp = pro.get_input<boxm2_volm_loc_hypotheses_sptr>(i++);
  float vmin = pro.get_input<float>(i++);
  float dmax = pro.get_input<float>(i++);
  float solid_angle = pro.get_input<float>(i++);
  float cap_angle = pro.get_input<float>(i++);
  float point_angle = pro.get_input<float>(i++); 
  vcl_string index_file = pro.get_input<vcl_string>(i++);
  float vis_thres = pro.get_input<float>(i++);
  float buffer_capacity = pro.get_input<float>(i++);
  
  volm_spherical_container_sptr sph2 = new volm_spherical_container(solid_angle,vmin,dmax);
  vcl_cout << "number of voxels in container: " << sph2->get_voxels().size() << vcl_endl;
  
  // construct spherical shell container, radius is always 1 cause points will be used to compute ray directions
  double radius = 1;
  volm_spherical_shell_container_sptr sph_shell = new volm_spherical_shell_container(radius, (double)cap_angle, (double)point_angle);
  int layer_size = (int)(sph_shell->get_container_size());
  boxm2_volm_wr3db_index_sptr ind = new boxm2_volm_wr3db_index(layer_size, buffer_capacity);
  ind->initialize_write(index_file);
  
  global_threads[0] = RoundUp(layer_size, (int)local_threads[0]);
  vcl_cout << "layer_size: " << layer_size << ", # of global threads: " << global_threads[0] << "\n";
  unsigned int thread_cnt = (unsigned)global_threads[0];

  //cache size sanity check
  long binCache = (long)(opencl_cache.ptr()->bytes_in_cache());
  vcl_cout<<"Update MBs in cache: "<<binCache/(1024.0*1024.0)<<vcl_endl;

  // create a command queue.
  int status=0;
  cl_command_queue queue = clCreateCommandQueue( device->context(),
                                                 *(device->device_id()),
                                                 CL_QUEUE_PROFILING_ENABLE,
                                                 &status);
  if (status!=0)
    return false;

  // compile the kernel if not already compiled
  vcl_string identifier=device->device_identifier();
  if (kernels.find(identifier)==kernels.end()) {
    vcl_cout<<"===========Compiling kernels==========="<<vcl_endl;
    vcl_vector<bocl_kernel*> ks;
    compile_kernel(device,ks);
    kernels[identifier]=ks;
  }

  // bit lookup buffer
  cl_uchar lookup_arr[256];
  boxm2_ocl_util::set_bit_lookup(lookup_arr);
  bocl_mem* lookup=new bocl_mem(device->context(), lookup_arr, sizeof(cl_uchar)*256, "bit lookup buffer");
  lookup->create_buffer(CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR);

  // create directions buffer
  cl_float* ray_dirs = new cl_float[4*layer_size];
  vcl_vector<vgl_point_3d<double>>& cart_points = sph_shell->cart_points();
  int cnt = 0;
  for (int i = 0; i < layer_size; i++) {
    ray_dirs[4*i  ] = (cl_float)cart_points[i].x();
    ray_dirs[4*i+1] = (cl_float)cart_points[i].y();
    ray_dirs[4*i+2] = (cl_float)cart_points[i].z();
    ray_dirs[4*i+3] = 0.0f;
    if (cnt++ < 5)
      vcl_cout << "ray dir: " << i << " " << cart_points[i].x() << " " << cart_points[i].y() << " " << cart_points[i].z() << vcl_endl;
  }
  
  bocl_mem* ray_dir_buffer = new bocl_mem(device->context(), ray_dirs, sizeof(cl_float4)*layer_size, "ray directions buffer");
  ray_dir_buffer->create_buffer(CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR);
  
  bocl_mem* ray_dim_mem = new bocl_mem(device->context(), &(layer_size), sizeof(int), "ray directions size");
  ray_dim_mem->create_buffer(CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR );

  vcl_map<boxm2_block_id, boxm2_block_metadata> blocks = scene->blocks();
  vcl_cout << "number of blocks: " << blocks.size() << vcl_endl;
  vcl_cout.flush();
  //: get subblk dimension
  boxm2_block_metadata mdata = scene->get_block_metadata(blocks.begin()->first);
  float subblk_dim = (float)mdata.sub_block_dim_.x();
  bocl_mem*  subblk_dim_mem=new bocl_mem(device->context(), &(subblk_dim), sizeof(float), "sub block dim buffer");
  subblk_dim_mem->create_buffer(CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR );
  
  //zip through each location hypothesis
  for (unsigned hi = 0; hi < hyp->locs_.size(); hi++)
  {
    vcl_cout << "Processing hypothesis: " << hi << " x: " << hyp->locs_[hi].x() << " y: " << hyp->locs_[hi].y() << " z: " << hyp->locs_[hi].z() << vcl_endl;
    
    cl_float loc_arr[4];
    loc_arr[0] = hyp->locs_[hi].x(); 
    loc_arr[1] = hyp->locs_[hi].y();
    loc_arr[2] = hyp->locs_[hi].z();
    loc_arr[3] = 1.0f;
    bocl_mem* hypo_location = new bocl_mem(device->context(), loc_arr, sizeof(cl_float4), "location buffer");
    hypo_location->create_buffer(CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR);
    
    // Output Arrays
    float* buff = new float[layer_size];
    for (int i=0;i<layer_size;i++) buff[i]=0.0f;
    float* vis_buff = new float[layer_size];
    for (int i=0;i<layer_size;i++) vis_buff[i]=1.0f;
    float* prob_buff = new float[layer_size];
    for (int i=0;i<layer_size;i++) prob_buff[i]=0.0f;
    float* t_infinity_buff = new float[layer_size];
    for (int i=0;i<layer_size;i++) t_infinity_buff[i]=0.0f;

    bocl_mem* exp_depth=new bocl_mem(device->context(),buff,layer_size*sizeof(float),"exp depth buffer");
    exp_depth->create_buffer(CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR);

    bocl_mem* vis=new bocl_mem(device->context(),vis_buff,layer_size*sizeof(float),"visibility buffer");
    vis->create_buffer(CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR);

    bocl_mem* probs=new bocl_mem(device->context(),prob_buff,layer_size*sizeof(float),"vis x omega buffer");
    probs->create_buffer(CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR);

    bocl_mem* t_infinity=new bocl_mem(device->context(),t_infinity_buff,layer_size*sizeof(float),"t infinity buffer");
    t_infinity->create_buffer(CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR);
    
    //zip through each block
    vcl_map<boxm2_block_id, boxm2_block_metadata>::iterator blk_iter_inner;
    for (blk_iter_inner = blocks.begin(); blk_iter_inner != blocks.end(); ++blk_iter_inner) {
      boxm2_block_id id_inner = blk_iter_inner->first;
      
      //load tree and alpha
      boxm2_block_metadata mdata = blk_iter_inner->second;
      bocl_kernel* kern =  kernels[identifier][0];
      
      vul_timer transfer;
      
      bocl_mem* blk       = opencl_cache->get_block(blk_iter_inner->first);
      bocl_mem* blk_info  = opencl_cache->loaded_block_info();
      //bocl_mem* alpha     = opencl_cache->get_data<BOXM2_ALPHA>(blk_iter_inner->first,0,false);
      bocl_mem* alpha = opencl_cache->get_data<BOXM2_ALPHA>(id_inner);
      
      transfer_time += (float) transfer.all();
    
      kern->set_arg( blk_info );
      kern->set_arg( ray_dir_buffer);
      kern->set_arg( ray_dim_mem);
      kern->set_arg( blk );
      kern->set_arg( lookup  );
      kern->set_arg( alpha  );
      kern->set_arg( hypo_location );
      kern->set_arg( exp_depth );
      kern->set_arg( vis );
      kern->set_arg( probs );
      kern->set_arg( t_infinity );
      kern->set_local_arg( local_threads[0]*local_threads[1]*sizeof(cl_uchar16) );//local tree,
      kern->set_local_arg( local_threads[0]*local_threads[1]*10*sizeof(cl_uchar) ); //cumsum buffer, imindex buffer

      //execute kernel
      if (!kern->execute(queue, 2, local_threads, global_threads)) {
        vcl_cerr << "INDEX KERNEL EXECUTE FAILED!\n";
        return false;
      }
      int status = clFinish(queue);
      if (status != 0) {
        vcl_cerr << "status: " << status << "  INDEX EXECUTE FAILED: " + error_to_string(status) << vcl_endl;
        return false;
      }
      gpu_time += kern->exec_time();

      //clear render kernel args so it can reset em on next execution
      kern->clear_args();
      
      //remove from device memory unnecessary items
      opencl_cache->shallow_remove_data(id_inner,boxm2_data_traits<BOXM2_ALPHA>::prefix());
      opencl_cache->shallow_remove_block(id_inner); // also remove blk_info
      //opencl_cache->clear_cache();
      
      status = clFinish(queue);
      check_val(status, MEM_FAILURE, "opencl clear cache FAILED: " + error_to_string(status)); 
    }
    
    // normalize
    {
      bocl_kernel* normalize_kern= kernels[identifier][1];
      normalize_kern->set_arg( exp_depth );
      normalize_kern->set_arg( probs );
      normalize_kern->set_arg( t_infinity );
      normalize_kern->set_arg( subblk_dim_mem );
      normalize_kern->execute( queue, 2, local_threads, global_threads);
      clFinish(queue);
      gpu_time += normalize_kern->exec_time();
  
      //clear render kernel args so it can reset em on next execution
      normalize_kern->clear_args();
    }

    //read from gpu
    exp_depth->read_to_buffer(queue);
    vis->read_to_buffer(queue);
    
    status = clFinish(queue);
    check_val(status, MEM_FAILURE, "read to output buffers FAILED: " + error_to_string(status));
#if 0
    vcl_cout << "exp depths after normalization: \n";
    for (unsigned i = 0; i < layer_size; i++) {
      vcl_cout << buff[i] << " (" << vis_buff[i] << ") ";
    }
    vcl_cout << vcl_endl;
#endif
    // find each depth interval using spherical container
    vcl_vector<char> values;
    for (int i = 0; i < layer_size; i++) {
      // check if sky
      if (vis_buff[i] > vis_thres)
        values.push_back((char)254);
      else
        values.push_back(sph2->get_depth_interval((double)buff[i]));
    }
#if 0
    sph_shell->draw_template("./test.vrml", values, 254);
#endif
    // add to index 
    ind->add_to_index(values);
  
    // release the device and host memories  
    delete exp_depth;  // calls release_memory() which enqueues a mem delete event, call clFinish to make sure it is executed
    delete vis;
    delete probs;
    delete t_infinity;
    delete hypo_location;
    
    status = clFinish(queue);
    check_val(status, MEM_FAILURE, "release memory FAILED: " + error_to_string(status));
    
    if (!buff)
      vcl_cout << "buff is zero after release mem!\n"; vcl_cout.flush();
    delete [] buff;
    delete [] vis_buff;
    delete [] prob_buff;
    delete [] t_infinity_buff;
  }
  
  delete ray_dir_buffer;
  delete ray_dim_mem;  
  delete lookup;
//  delete subblk_dim_mem;
  
  status = clFinish(queue);
  check_val(status, MEM_FAILURE, "release memory FAILED: " + error_to_string(status));
  
  delete [] ray_dirs;

  vcl_cout<<"Gpu time "<<gpu_time<<" transfer time "<<transfer_time<< " micro seconds. " << vcl_endl;
  clReleaseCommandQueue(queue);
  
  //cache size sanity check
  binCache = (long)(opencl_cache.ptr()->bytes_in_cache());
  vcl_cout<<"At the end of process MBs in cache: "<<binCache/(1024.0*1024.0)<<vcl_endl;
  
  ind->finalize();
  
  return true;
}
