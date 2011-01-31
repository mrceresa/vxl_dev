
#include <vpgl/vpgl_perspective_camera.h>
#include <vil/vil_convert.h>
#include <vil/vil_image_view_base.h>
#include <vil/vil_image_view.h>
#include <vil/vil_math.h>
#include <vil/vil_load.h>
#include <vil/vil_save.h>

//executable args
#include <vul/vul_arg.h>
#include <vul/vul_file.h>
#include <vul/vul_file_iterator.h>
#include <vul/vul_timer.h>

//boxm2 scene stuff
#include <boxm2/io/boxm2_nn_cache.h>
#include <boxm2/boxm2_scene.h>
#include <boxm2/basic/boxm2_block_id.h>
#include <boxm2/cpp/boxm2_cpp_processor.h>
#include <boxm2/cpp/pro/boxm2_cpp_change_detection_process.h>
#include <boxm2/cpp/pro/boxm2_cpp_change_detection_uncertainity_process.h>

//brdb stuff
#include <brdb/brdb_value.h>

int main(int argc,  char** argv)
{
  vcl_cout<<"Boxm2 CPP Change Detection "<<vcl_endl;
  vul_arg<vcl_string> cam("-cam", "camera filename", "");
  vul_arg<vcl_string> scene_file("-scene", "scene filename", "");
  vul_arg<vcl_string> img("-img", "input image", "");
  vul_arg<vcl_string> outimg("-outimg", "change image", "");
  vul_arg<bool> use_uncertainity("-uncertain", "Use Uncertainity", true);

  // need this on some toolkit implementations to get the window up.
  vul_arg_parse(argc, argv);

  boxm2_scene_sptr scene = new boxm2_scene(scene_file());
  brdb_value_sptr brdb_scene_sptr = new brdb_value_t<boxm2_scene_sptr>(scene);
  // get relevant blocks
  boxm2_nn_cache cache( scene.ptr() );
  boxm2_cpp_processor cpp_pro;
  cpp_pro.init();
  cpp_pro.set_scene(scene.ptr());

  vcl_cout<<"Using uncertainity "<<use_uncertainity()<<vcl_endl;
  boxm2_cpp_process_base * cpp_change;
  if(use_uncertainity())
    cpp_change=new boxm2_cpp_change_detection_uncertainity_process();
  else
    cpp_change=new boxm2_cpp_change_detection_process();

  cpp_change->init();
  cpp_change->set_cache(&cache);

  {
      //load camera from file
      vcl_ifstream ifs(cam().c_str());
      vpgl_perspective_camera<double>* pcam =new vpgl_perspective_camera<double>;
      if (!ifs.is_open()) {
          vcl_cerr << "Failed to open file " <<cam() << '\n';
          return -1;
      }
      else  {
          ifs >> *pcam;
      }
      vpgl_camera_double_sptr cam = pcam;
      brdb_value_sptr brdb_cam = new brdb_value_t<vpgl_camera_double_sptr>(cam);

      //load image from file
      vil_image_view_base_sptr loaded_image = vil_load(img().c_str());
      vil_image_view<float>* floatimg = new vil_image_view<float>(loaded_image->ni(), loaded_image->nj(), 1);
      if (vil_image_view<vxl_byte> *img_byte = dynamic_cast<vil_image_view<vxl_byte>*>(loaded_image.ptr()))
      {
          vil_convert_stretch_range_limited(*img_byte, *floatimg, vxl_byte(0), vxl_byte(255), 0.0f, 1.0f);
      }
      else {
          vcl_cerr << "Failed to load image " << img()<< vcl_endl;
          return -1;
      }

      //create input image buffer
      brdb_value_sptr brdb_inimg = new brdb_value_t<vil_image_view_base_sptr>(floatimg);
      vil_image_view<float>* expimg = new vil_image_view<float>(loaded_image->ni(), loaded_image->nj(), 1);
      brdb_value_sptr brdb_expimg = new brdb_value_t<vil_image_view_base_sptr>(expimg);

      vcl_vector<brdb_value_sptr> input;
      input.push_back(brdb_scene_sptr);
      input.push_back(brdb_cam);
      input.push_back(brdb_inimg);
      input.push_back(brdb_expimg);

      //init output vector
      vcl_vector<brdb_value_sptr> output;

      vul_timer t;
      t.mark();
      cpp_pro.run(cpp_change, input, output);
      vil_save(*expimg,outimg().c_str());
      vcl_cout<<"Time taken is :" <<t.all()<<vcl_endl;
  }  
  cpp_pro.finish();

  return 0;
}