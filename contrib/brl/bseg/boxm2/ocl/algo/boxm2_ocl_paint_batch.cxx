#include "boxm2_ocl_paint_batch.h"
//:
// \file
#include <boxm2/ocl/boxm2_opencl_cache.h>
#include <boxm2/boxm2_scene.h>
#include <boxm2/boxm2_block.h>
#include <boxm2/boxm2_data_base.h>
#include <boxm2/ocl/boxm2_ocl_util.h>
#include <boxm2/boxm2_util.h>
#include <boxm2/boxm2_data_traits.h>
#include <boxm2/cpp/algo/boxm2_mog3_grey_processor.h>

//brdb stuff
#include <brdb/brdb_value.h>

//directory utility
#include <vcl_fstream.h>
#include <vcl_where_root_dir.h>
#include <bocl/bocl_device.h>
#include <bocl/bocl_kernel.h>
#include <vul/vul_timer.h>
#include <vcl_algorithm.h>

//: Declare kernels
vcl_map<vcl_string, bocl_kernel*> boxm2_ocl_paint_batch::kernels_;

//paint block
void boxm2_ocl_paint_batch::paint_block( boxm2_scene_sptr           scene,
                                         bocl_device_sptr           device,
                                         boxm2_stream_cache_sptr    str_cache,
                                         boxm2_opencl_cache_sptr    opencl_cache,
                                         cl_command_queue           &queue,
                                         vcl_string                 data_type,
                                         boxm2_block_id             id,
                                         bsta_sigma_normalizer_sptr n_table )
{
  typedef boxm2_data_traits<BOXM2_AUX0>::datatype aux0_datatype;
  typedef boxm2_data_traits<BOXM2_AUX1>::datatype aux1_datatype;
  typedef boxm2_data_traits<BOXM2_AUX2>::datatype aux2_datatype;

  //grab the device appropriate kernel
  vcl_string opts = boxm2_ocl_util::mog_options( data_type );
  bocl_kernel* kern = compile_kernels(device, opts);

  vul_timer t;
  float stream_time   = 0.0f;
  float transfer_time = 0.0f;
  float gpu_time      = 0.0f;

  // reads from disc if not already in memory
  boxm2_cache_sptr cache  = opencl_cache->get_cpu_cache();
  boxm2_data_base* alph   = cache->get_data_base(id,boxm2_data_traits<BOXM2_ALPHA>::prefix(),0,false);
  int alphaTypeSize       = (int) boxm2_data_info::datasize(boxm2_data_traits<BOXM2_ALPHA>::prefix());
  int data_buff_length    = (int) (alph->buffer_length()/alphaTypeSize);

  //debuggers--------
  int mogSize = (int) boxm2_data_info::datasize(boxm2_data_traits<BOXM2_MOG3_GREY>::prefix());
  boxm2_data_base* mog_base = cache->get_data_base(id,boxm2_data_traits<BOXM2_MOG3_GREY>::prefix(), mogSize*data_buff_length, false);
  boxm2_data<BOXM2_MOG3_GREY>* mog_data_ = new boxm2_data<BOXM2_MOG3_GREY>(mog_base->data_buffer(), mog_base->buffer_length(), mog_base->block_id());
  //----------------

  //int buffer indicating starting location for each sample
  cl_uint* sampleIndex = new cl_uint[data_buff_length];
  cl_uint  currIdx     = 0;
  vcl_vector<vxl_byte> all_obs, all_vis;

  //go through each cell, get stats, store
  vcl_cout<<"  NumIndexes: "<<data_buff_length<<'\n'
          <<"  currIndex: ";
  for (int index = 0; index < data_buff_length; ++index) {
    if (index%10000==0)
      vcl_cout<<index<<"..."<<vcl_flush;

    //make sure to store the currIDX;
    sampleIndex[index] = currIdx;

    //initial data
    vul_timer st;
    vcl_vector<aux0_datatype> cell_len = str_cache->get_next<BOXM2_AUX0>(id, index);
    vcl_vector<aux1_datatype> cell_obs = str_cache->get_next<BOXM2_AUX1>(id, index);
    vcl_vector<aux2_datatype> cell_vis = str_cache->get_next<BOXM2_AUX2>(id, index);
    stream_time += st.all();

    //push all samples into block
    unsigned nimgs = (unsigned) cell_len.size();
    for (unsigned m = 0; m < nimgs; m++) {

      float obs_seg_len = cell_len[m];
      if (obs_seg_len > 1e-16f) {

        //store obs and vis only if this cell is visible
        float obs = cell_obs[m]/obs_seg_len;
        float vis = cell_vis[m]/obs_seg_len; // mean vis

        //store bytes
        vxl_byte obsByte = (vxl_byte) (obs*255.0f);
        vxl_byte visByte = (vxl_byte) (vis*255.0f);
        all_obs.push_back(obsByte);
        all_vis.push_back(visByte);
        ++currIdx;
      }
    }
  }
  vcl_cout<<vcl_endl;

  //report some buffer sizes
  long totalSize = sizeof(int)*data_buff_length + (all_obs.size()+all_vis.size())*sizeof(vxl_byte);
  unsigned int numSamps = (unsigned int) all_obs.size();
  float time = (float) t.all();
  vcl_cout<<"  num cells: "<<data_buff_length<<"; "
          <<"  num samps: "<<numSamps<<" = "<< (float)(numSamps/data_buff_length)<<" samples per cell (avg)\n"
          <<"  total size: "<<(float)totalSize/1024.0f/1024.0f<<" mb in "<<t.all()<<" ms ("<<stream_time<<" ms in stream cache)"<<vcl_endl;

  //-------------------------------------------------------------------------
  //set up input buffers
  bocl_mem_sptr index_mem = new bocl_mem(device->context(), sampleIndex, sizeof(cl_uint)*data_buff_length, "sample index buffer");
  index_mem->create_buffer(CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR);

  vxl_byte* intsBuff = &all_obs[0];
  bocl_mem_sptr sample_ints = new bocl_mem(device->context(), intsBuff, sizeof(char)*numSamps, "sample_intensities buffer");
  sample_ints->create_buffer(CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR);

  vxl_byte* visBuff = &all_vis[0];
  bocl_mem_sptr sample_vis  = new bocl_mem(device->context(), visBuff, sizeof(char)*numSamps, "sample_visibilities buffer");
  sample_vis->create_buffer(CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR);

  bocl_mem_sptr num_samples = new bocl_mem(device->context(), &numSamps, sizeof(unsigned int), "total number of samples buffer");
  num_samples->create_buffer(CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR);

   // Output Array
  float output_arr[100] = {0};
  bocl_mem_sptr  cl_output=new bocl_mem(device->context(), output_arr, sizeof(float)*100, "output buffer");
  cl_output->create_buffer(CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR);

  //write the image values to the buffer
  int mogTypeSize     = (int) boxm2_data_info::datasize(data_type);
  vul_timer transfer;
  bocl_mem* blk       = opencl_cache->get_block(id);
  bocl_mem* mog       = opencl_cache->get_data(id, data_type, data_buff_length*mogTypeSize, false);
  bocl_mem* blk_info  = opencl_cache->loaded_block_info();

  boxm2_scene_info* info_buffer = (boxm2_scene_info*) blk_info->cpu_buffer();
  info_buffer->data_buffer_length = data_buff_length;
  blk_info->write_to_buffer(queue);
  transfer_time += (float) transfer.all();
  vcl_size_t lThreads[] = {64, 1};
  vcl_size_t gThreads[] = {RoundUp(data_buff_length,lThreads[0]), 1};

  //set first kernel args
  kern->set_arg( blk_info );
  kern->set_arg( mog );
  kern->set_arg( index_mem.ptr() );
  kern->set_arg( sample_ints.ptr() );
  kern->set_arg( sample_vis.ptr() );
  kern->set_arg( num_samples.ptr() );
  kern->set_arg( cl_output.ptr() );

  //execute kernel
  kern->execute( queue, 2, lThreads, gThreads);
  clFinish(queue);
  kern->clear_args();

  //read back to buffer
  mog->read_to_buffer(queue);
  gpu_time += kern->exec_time();

  vcl_cout<<" Batch paint block GPU Time: "<<gpu_time<<", transfer time: "<<transfer_time<<vcl_endl;

  // clean up cmdqueue
  opencl_cache->clear_cache(); 
  vcl_cout<<"Opencl Cache: "<<opencl_cache->to_string()<<vcl_endl;
  clReleaseCommandQueue(queue);
  delete[] sampleIndex;
}


//: Keeps track of already compiled kernels, and returns matching ones
bocl_kernel* boxm2_ocl_paint_batch::compile_kernels( bocl_device_sptr device,
                                                     vcl_string       opts )
{
  //make id out of device
  vcl_string identifier = device->device_identifier();
  if ( kernels_.find(identifier) != kernels_.end() )
    return kernels_[identifier];

  vcl_cout<<"===========Compiling kernels==========="<<vcl_endl;
  //gather all render sources... seems like a lot for rendering...
  vcl_vector<vcl_string> src_paths;
  vcl_string source_dir = boxm2_ocl_util::ocl_src_root();
  src_paths.push_back(source_dir + "scene_info.cl");
  src_paths.push_back(source_dir + "cell_utils.cl");
  src_paths.push_back(source_dir + "statistics_library_functions.cl");
  src_paths.push_back(source_dir + "stat/weighted_em.cl");
  src_paths.push_back(source_dir + "batch/batch_update_appearance.cl");

  //compilation options
  bocl_kernel* kernel = new bocl_kernel();
  kernel->create_kernel(&device->context(),device->device_id(), src_paths, "batch_update_appearance", opts, "batch_paint::paint_kernel");

  //store in map
  kernels_[identifier] = kernel;
  return kernel;
}


void boxm2_ocl_paint_batch::weighted_mean_var(float* obs, float* vis, int numSamples, float& mean, float& var)
{
  var = 0.0f, mean = 0.0f;
  if (numSamples == 0)
    return;

  if (numSamples == 1) {
    mean = obs[0];
    var  = .06f;
    return;
  }

  float sample_mean     = 0.0f;
  float sum_weights     = 0.0f;
  float sum_sqr_weights = 0.0f;
  for (int i=0; i<numSamples; ++i) {
    float w = vis[i];
    float o = obs[i];
    sample_mean     += o*w;
    sum_weights     += w;
    sum_sqr_weights += w*w;
  }

  //divide the weighted mean
  if (sum_weights > 0.0f)
    sample_mean /= sum_weights;
  else
    return;

  //sum square differences for unbiased variance estimation
  float sum_sqr_diffs = 0.0f;
  for (int i=0; i<numSamples; ++i) {
    float w = vis[i];
    float o = obs[i];
    sum_sqr_diffs += w * (o-sample_mean)*(o-sample_mean);
  }

  float sample_var = (sum_weights / (sum_weights*sum_weights - sum_sqr_weights) ) * sum_sqr_diffs;

  var = sample_var;
  mean = sample_mean;
  return;
}

