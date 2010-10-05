//:
// \file
// \brief Tool to write eps files showing modes of model
// \author Tim Cootes

#include <mbl/mbl_parse_block.h>
#include <mbl/mbl_read_props.h>
#include <mbl/mbl_exception.h>
#include <mbl/mbl_parse_colon_pairs_list.h>
#include <vul/vul_arg.h>
#include <vul/vul_string.h>
#include <vcl_sstream.h>
#include <vcl_fstream.h>
#include <vcl_string.h>
#include <vcl_algorithm.h>
#include <vsl/vsl_quick_file.h>

#include <msm/msm_shape_model.h>
#include <msm/msm_shape_instance.h>
#include <msm/msm_curve.h>

#include <mbl/mbl_cloneables_factory.h>
#include <msm/msm_add_all_loaders.h>
#include <msm/utils/msm_draw_shape_to_eps.h>
#include <msm/utils/msm_shape_mode_view.h>

/*
Parameter file format:
<START FILE>
//: File to load model from
shape_model_path: shape_model.bfs

curves_path: face_front.crvs

// Maximum number of shape modes
max_modes: 99

//: Number of SDs to vary mode by
n_sds: 3.0

//: Current number of shapes per mode
n_per_mode: 3

//: When true, generate 2*n_per_mode frames of a movie for each mode
make_movie: false

//: When true, overlap all the shapes to highlight changes
overlap_shapes: false

//: Radius of points to display (if <0, then don't draw points)
point_radius: 2

line_colour: black
point_colour: red

// Approximate width of region to display shape
width: 100

base_name: mode
output_dir: ./

<END FILE>
*/

void print_usage()
{
  vcl_cout << "msm_draw_shape_modes -p param_file\n"
           << "Tool to write eps files showing modes of model.\n"
           << vcl_endl;

  vul_arg_display_usage_and_exit();
}

//: Structure to hold parameters
struct tool_params
{
  //: Path to shape model
  vcl_string shape_model_path;
  vcl_string curves_path;

  //: Maximum number of shape modes
  unsigned max_modes;

  //: Number of SDs to vary mode by
  double n_sds;

  //: Current number of shapes per mode
  unsigned n_per_mode;

  //: When true, generate 2*n_per_mode frames of a movie for each mode
  bool make_movie;

  //: When true, overlap all the shapes to highlight changes
  bool overlap_shapes;

  //: Approximate width of region to display shape
  double width;

  //: Base name for output
  vcl_string base_name;

  //: Directory to save eps files
  vcl_string output_dir;

  //: Line colour
  vcl_string line_colour;

  //: Point colour
  vcl_string point_colour;

  //: Radius of points to display (if <0, then don't draw points)
  double point_radius;


  //: Parse named text file to read in data
  //  Throws a mbl_exception_parse_error if fails
  void read_from_file(const vcl_string& path);
};

//: Parse named text file to read in data
//  Throws a mbl_exception_parse_error if fails
void tool_params::read_from_file(const vcl_string& path)
{
  vcl_ifstream ifs(path.c_str());
  if (!ifs)
  {
    vcl_string error_msg = "Failed to open file: "+path;
    throw (mbl_exception_parse_error(error_msg));
  }

  mbl_read_props_type props = mbl_read_props_ws(ifs);

  curves_path=props["curves_path"];
  n_per_mode=vul_string_atoi(props.get_optional_property("n_per_mode","3"));
  max_modes=vul_string_atoi(props.get_optional_property("max_modes","5"));
  overlap_shapes=vul_string_to_bool(props.get_optional_property("overlap_shapes","false"));
  make_movie=vul_string_to_bool(props.get_optional_property("make_movie","false"));
  n_sds=vul_string_atof(props.get_optional_property("n_sds","3.0"));
  point_radius=vul_string_atof(props.get_optional_property("point_radius","1.5"));
  width=vul_string_atof(props.get_optional_property("width","100"));
  line_colour=props.get_optional_property("line_colour","black");
  point_colour=props.get_optional_property("point_colour","red");
  base_name=props.get_optional_property("base_name","./");
  output_dir=props.get_optional_property("output_dir","./");
  shape_model_path=props.get_optional_property("shape_model_path",
                                       "shape_aam.bfs");

  if (make_movie) overlap_shapes=true;

  // Don't look for unused props so can use a single common parameter file.
}


void draw_mode(msm_shape_mode_view& mode_view,
               const msm_curves& curves,
               tool_params& params)
{
  mode_view.compute_shapes();
  unsigned m = mode_view.mode();
  vgl_box_2d<int> win_box = mode_view.display_window();

  vcl_stringstream ss;
  ss<<params.base_name<<"_s"<<m<<".eps";
  mbl_eps_writer writer(ss.str().c_str(),
                        win_box.width(),win_box.height());

  for (unsigned i=0;i<mode_view.points().size();++i)
  {
    writer.set_colour(params.point_colour);
    if (params.point_radius>0)
      msm_draw_points_to_eps(writer,mode_view.points()[i],
                              params.point_radius);
    writer.set_colour(params.line_colour);
    msm_draw_shape_to_eps(writer,mode_view.points()[i],curves);
  }
  writer.close();
  vcl_cout<<"Saved to "<<ss.str()<<vcl_endl;
}

//: Write a set of eps files defining a movie of the mode.
void draw_mode_frames(msm_shape_mode_view& mode_view,
               const msm_curves& curves,
               tool_params& params)
{
  mode_view.compute_shapes();
  unsigned m = mode_view.mode();
  vgl_box_2d<int> win_box = mode_view.display_window();

  unsigned n_shapes = mode_view.points().size();
  for (unsigned i=0;i<2*n_shapes-2;++i)
  {
    // f goes from 0 - (n_shapes-1), then back down to 1
    unsigned f=i;
    if (i>=n_shapes) f=2*n_shapes-2-i;

    vcl_stringstream ss;
    ss<<params.base_name<<"_s"<<m<<"_";
    if (i<10) ss<<"0";
    ss<<i<<".eps";
    mbl_eps_writer writer(ss.str().c_str(),
                        win_box.width(),win_box.height());

    writer.set_colour(params.point_colour);
    if (params.point_radius>0)
      msm_draw_points_to_eps(writer,mode_view.points()[f],
                              params.point_radius);
    writer.set_colour(params.line_colour);
    msm_draw_shape_to_eps(writer,mode_view.points()[f],curves);

    writer.close();
  }
  vcl_cout<<"Saved "<<2*n_shapes-2<<" frames to "
          <<params.base_name<<"_s"<<m<<"_XX.eps"<<vcl_endl;
}


int main(int argc, char** argv)
{
  vul_arg<vcl_string> param_path("-p","Parameter filename");
  vul_arg_parse(argc,argv);

  msm_add_all_loaders();

  if (param_path()=="")
  {
    print_usage();
    return 0;
  }

  tool_params params;
  try 
  { 
    params.read_from_file(param_path()); 
  }
  catch (mbl_exception_parse_error& e)
  {
    vcl_cerr<<"Error: "<<e.what()<<vcl_endl;
    return 1;
  }

  msm_shape_model shape_model;

  if (!vsl_quick_file_load(shape_model,params.shape_model_path))
  {
    vcl_cerr<<"Failed to load shape model from "
        <<params.shape_model_path<<vcl_endl;
    return 2;
  }

  vcl_cout<<"Model: "<<shape_model<<vcl_endl;

  msm_curves curves;
  if (!curves.read_text_file(params.curves_path))
    vcl_cerr<<"Failed to read in curves from "<<params.curves_path<<vcl_endl;

  msm_shape_mode_view mode_view;
  mode_view.set_shape_model(shape_model);
/*
  // Estimate aspect ratio
  vgl_box_2d<double> bounds = shape_model.mean_points().bounds();
  double asp=bounds.height()/(1e-3+bounds.width());

  int win_width;
  if (params.overlap_shapes)
    win_width=int(params.width);
  else
    win_width=int(params.width*params.n_per_mode);

  int win_height=int(asp*params.width);
  mode_view.set_display_window(vgl_box_2d<int>(0,win_width, 0,win_height));
*/
  mode_view.set_range(params.n_sds);
  mode_view.set_n_per_mode(params.n_per_mode);
  mode_view.set_overlap_shapes(params.overlap_shapes);
  mode_view.set_display_width(params.width);
  vgl_box_2d<int> win_box = mode_view.display_window();

  unsigned n_modes = vcl_min(params.max_modes,shape_model.n_modes());

  for (unsigned m=0;m<n_modes;++m)
  {
    mode_view.set_mode(1+m);

    if (params.make_movie)
      draw_mode_frames(mode_view,curves,params);
    else
      draw_mode(mode_view,curves,params);
  }
  return 0;
}