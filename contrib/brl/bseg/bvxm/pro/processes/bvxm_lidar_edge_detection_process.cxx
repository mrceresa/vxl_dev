//This is brl/bseg/bvxm/pro/processes/bvxm_lidar_edge_detection_process.cxx

//:
// \file
// \brief A class for generating LiDAR edges given a LiDAR image pair.
//        -  Input:
//             - First return path (string)
//             - Second return path (string)
//
//        -  Output:
//             - clipped image area (first ret) "vil_image_view_base_sptr"
//             - clipped image area (second ret) "vil_image_view_base_sptr"
//             - mask "vil_image_view_base_sptr"
//
// \author  Ibrahim Eden
// \date    July 31, 2008
// \verbatim
//  Modifications
//   Isabel Restrepo - Jan 27, 2009 - converted process-class to functions which is the new design for bvxm_processes.
// \endverbatim

#include <bprb/bprb_func_process.h>
#include <bprb/bprb_parameters.h>
#include <vil/vil_image_view.h>
#include <vil/vil_pixel_format.h>
#include <vil/vil_convert.h>

//: global variables/functions
namespace bvxm_lidar_edge_detection_process_globals
{
  const unsigned n_inputs_ = 2;
  const unsigned n_outputs_ =3;

  //set parameter identifying strings
  const vcl_string param_thresh_diff_ =  "threshold_edge_difference";
}

//: set input and output types
bool bvxm_lidar_edge_detection_process_cons(bprb_func_process& pro)
{
  using namespace bvxm_lidar_edge_detection_process_globals;
  //this process takes 2 inputs:
  //the first and second return images
  vcl_vector<vcl_string> input_types_(n_inputs_);
  unsigned i = 0;
  input_types_[i++] = "vil_image_view_base_sptr";  // first ret image ROI
  input_types_[i++] = "vil_image_view_base_sptr";  // second ret image ROI
  if (!pro.set_input_types(input_types_))
    return false;

  //output
  //output
  vcl_vector<vcl_string> output_types_(n_outputs_);
  unsigned j =0;
  output_types_[j++]= "vil_image_view_base_sptr";  // lidar height image
  output_types_[j++]= "vil_image_view_base_sptr";  // lidar edge image
  output_types_[j++]= "vil_image_view_base_sptr";  // lidar edge probability image
  if (!pro.set_output_types(output_types_))
    return false;

  return true;
}

bool bvxm_lidar_edge_detection_process(bprb_func_process& pro)
{
  using namespace bvxm_lidar_edge_detection_process_globals;

  //check number of inputs
  if (pro.n_inputs()<n_inputs_)
  {
    vcl_cout << pro.name() << " The input number should be " << n_inputs_<< vcl_endl;
    return false;
  }

  // get the inputs:
  // image
  unsigned i = 0;
  vil_image_view_base_sptr image_first_return_base = pro.get_input<vil_image_view_base_sptr>(i++);
  vil_pixel_format input_pixel_format = image_first_return_base->pixel_format();
  vil_image_view_base_sptr image_second_return_base = pro.get_input<vil_image_view_base_sptr>(i++);

  //check input's validity
  if (image_first_return_base == 0) {
    vcl_cout << "first return image is NULL\n";
    return false;
  }

  if (image_second_return_base == 0) {
    vcl_cout << "last return image is NULL\n";
    return false;
  }

  bool is_input_float = true;
  if (input_pixel_format == VIL_PIXEL_FORMAT_BYTE){
    is_input_float = false;
  }
  else if (input_pixel_format == VIL_PIXEL_FORMAT_FLOAT){
    is_input_float = true;
  }
  else {
    vcl_cout << "this process only accepts vxl_float or vxl_byte pixel formats\n";
    return false;
  }

  vil_image_view<float> image_first_return = *(vil_convert_cast(float(), image_first_return_base));
  vil_image_view<float> image_second_return = *(vil_convert_cast(float(), image_second_return_base));

  //read in parameters
  float threshold_edge_difference = 10.0f;
  if (!pro.parameters()->get_value(param_thresh_diff_, threshold_edge_difference)) {
    vcl_cout << "problems in retrieving parameters\n";
    return false;
  }

  unsigned ni = image_first_return.ni();
  unsigned nj = image_first_return.nj();

  vil_image_view<float> lidar_height(ni,nj);
  lidar_height.fill(0.0f);
  vil_image_view<float> lidar_edges(ni,nj);
  lidar_edges.fill(0.0f);
  vil_image_view<float> lidar_edges_prob(ni,nj);
  lidar_edges_prob.fill(0.0f);

  for (unsigned i=0; i<ni; i++){
    for (unsigned j=0; j<nj; j++){
      float curr_difference = image_first_return(i,j)-image_second_return(i,j);
      lidar_edges_prob(i,j) = curr_difference/(1.0f+curr_difference);
      if (curr_difference > threshold_edge_difference){
        lidar_edges(i,j) = 1.0f;
        lidar_height(i,j) = image_first_return(i,j);
      }
      else{
        lidar_edges(i,j) = 0.0f;
        lidar_height(i,j) = 0.0f;
      }
    }
  }

  unsigned j = 0;
  // store image height
  pro.set_output_val<vil_image_view_base_sptr>(j++,(new vil_image_view<float>(lidar_height)));
  // store image edge
  pro.set_output_val<vil_image_view_base_sptr>(j++, (new vil_image_view<float>(lidar_edges)));
  // store image edge prob
  pro.set_output_val<vil_image_view_base_sptr>(j++, (new vil_image_view<float>(lidar_edges_prob)));

  return true;
}
