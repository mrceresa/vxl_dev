// This is mul/mil/mil_byte_image_2d_io.cxx
#ifdef VCL_NEEDS_PRAGMA_INTERFACE
#pragma implementation
#endif
//:
// \file
// \brief Load and save mil_image_2d_of<vil_byte> from named files.
// \author Tim Cootes

#include "mil_byte_image_2d_io.h"

#include <mil/mil_byte_image_2d_io_plugin.h>

#include <vcl_string.h>
#include <vcl_cassert.h>

#include <vil/vil_load.h>
#include <vil/vil_save.h>
#include <vil/vil_memory_image_of.h>
#include <vil/vil_rgb_byte.h>
#include <mil/mil_convert_vil.h>

//=======================================================================

mil_byte_image_2d_io::mil_byte_image_2d_io()
: colour_code_( mil_byte_image_2d_io_depth_image )
{
}

//=======================================================================

mil_byte_image_2d_io::~mil_byte_image_2d_io()
{
}

//: Define whether to load images as colour or grey-scale
//  Options are '' (i.e. rely on image), 'Grey' or 'RGB'
void mil_byte_image_2d_io::setColour(const vcl_string& c)
{
//  colour_ = c;
  if ( c == "" )
  {
    colour_code_ = mil_byte_image_2d_io_depth_image;
    return;
  }
  else if ( c == "Grey" )
  {
    colour_code_ = mil_byte_image_2d_io_depth_grey;
    return;
  }
  else if ( c == "RGB" )
  {
    colour_code_ = mil_byte_image_2d_io_depth_rgb;
    return;
  }

  vcl_cerr << " mil_byte_image_2d_io::set_colour(int); colour depth string "
           << c << " not supported\n";
}

//: Set the colour by the depth or using the standard code in the header
//  Only colour (3-plane) and greyscale (1-plane) currently supported
//  Returns false if the depth is not supported
bool mil_byte_image_2d_io::set_colour_depth( int new_depth )
{
  if ( new_depth == mil_byte_image_2d_io_depth_image ||
       new_depth == mil_byte_image_2d_io_depth_grey  ||
       new_depth == mil_byte_image_2d_io_depth_rgb )
  {
    colour_code_ = (mil_byte_image_2d_io_std_depths &)new_depth;
    return true;
  }

  vcl_cerr << " mil_byte_image_2d_io::set_colour(int); colour depth code "
           << new_depth << " not supported - NON FATAL ERROR\n";
  return false;
}

//: Whether to load images as RGB, Grey-scale or leave to image format
vcl_string mil_byte_image_2d_io::colour() const
{
  if ( colour_code_ == mil_byte_image_2d_io_depth_image )
  {
    return vcl_string("");
  }
  else if ( colour_code_ == mil_byte_image_2d_io_depth_grey )
  {
    return vcl_string("Grey");
  }
  else if ( colour_code_ == mil_byte_image_2d_io_depth_rgb )
  {
    return vcl_string("RGB");
  }

  vcl_cerr << " mil_byte_image_2d_io::set_colour(int); colour code " << (int)colour_code_
           << " not found in standard list - NON FATAL ERROR\n";

  return vcl_string("Bugger");
}

//: Return the colour depth using the standard code in the header
//  Only colour (3-plane) and greyscale (1-plane) currently supported
int mil_byte_image_2d_io::colour_depth()
{
  return colour_code_;
}

//: Current image
//  The one generated by last call to loadImage() or loadTheImage()
const mil_image& mil_byte_image_2d_io::image() const
{
  return image_;
}

//: Attempt to load image from named file
// The image can be found by calling image()
// \param filetype  String hinting at what image format is
// \return true if successful
bool mil_byte_image_2d_io::loadImage(const vcl_string& path,
              const vcl_string& filetype)
{
  return loadTheImage(image_,path,filetype);
}

//: Attempt to save image to named file
// \param filetype  String defining what format to save in
// \return true if successful
bool mil_byte_image_2d_io::saveImage(const mil_image& image,
                                     const vcl_string& path,
                                     const vcl_string& filetype) const
{
  assert(image.is_class("mil_image_2d_of<vil_byte>"));
  return saveTheImage((const mil_image_2d_of<vil_byte>&) image,path,filetype);
}


vcl_string mil_byte_image_2d_io::guessFileType(const vcl_string& path)
{
  int n = path.size();
  if (n<=4) return "Unknown";

  const char* ext = path.c_str()+n-4;
  vcl_string s_ext= ext;

  if (s_ext == ".BMP" || s_ext==".bmp")
    return "bmp";
  else
  if (s_ext == ".ras" || s_ext==".RAS")
    return "sunras";
  else
  if (s_ext == ".rad")
    return "radial";
  else
  if (s_ext == ".gif" || s_ext==".GIF")
    return "gif";
  else
  if (s_ext == ".jpg" || s_ext==".JPG")
    return "jpeg";

  return "Unknown";
}


//: Attempt to load image from named file
// \param filetype  String hinting at what image format is
// \return true if successful
// \retval image The loaded image. The object also stores the image
// which can be found by subsequent calls to image()
bool mil_byte_image_2d_io::loadTheImage(mil_image_2d_of<vil_byte>& image,
              const vcl_string& path,
              const vcl_string& f_type)
{
  mil_byte_image_2d_io_plugin plugin;
  if ( plugin.loadTheImage(image,path,f_type,colour()) )
  {
    return true;
  }

  vil_image img = vil_load(path.c_str());  // ie f_type is ignored here !!
  int nx = img.width();
  int ny = img.height();
  if (nx==0 || ny==0)
  {
    image_.resize(0,0);
    image = image_;
    return false;
  }

  bool img_is_grey = (img.get_size_bytes()==(nx*ny));

  if ( colour_depth() == mil_byte_image_2d_io_depth_grey )// colour_=="Grey")   // ie make a grey image whatever image is loaded
  {
    // wish to load a grey image
    if (img_is_grey)
    {
      // convert grey to grey
      vil_memory_image_of<vil_byte> buf(img);
      mil_convert_vil_gv2gm(image_,buf);
    }
    else
    {
      // convert colour image to grey
      vil_memory_image_of<vil_rgb_byte> buf(img);
      mil_convert_vil_cv2gm(image_,buf);
    }
  }
  else if ( colour_depth() == mil_byte_image_2d_io_depth_rgb ) // colour_=="RGB")
  {
    // wish to load a colour image
    if (img_is_grey)
    {
      //problem
      vcl_cerr<<"require a colour image, image attempting to load is grey\n";
      image_.resize(0,0);
      image = image_;
      return false;
    }
    else
    {
      //i.e. copy colour image as colour
      vil_memory_image_of<vil_rgb_byte> buf(img);
      mil_convert_vil_cv2cm(image_,buf);
    }
  }
  else if ( colour_depth() == mil_byte_image_2d_io_depth_image ) // i.e. colour_="" => rely on image
  {
    // i.e. colour_="" => rely on image
    if (img_is_grey)
    {
      //copy grey image as grey
      vil_memory_image_of<vil_byte> buf(img);
      mil_convert_vil_gv2gm(image_,buf);
    }
    else
    {
      //i.e. copy colour image as colour
      vil_memory_image_of<vil_rgb_byte> buf(img);
      mil_convert_vil_cv2cm(image_,buf);
    }
  }
  else
  {
    vcl_cerr << "mil_byte_image_2d_io::loadTheImage(); colour_depth() "
             << colour_depth() << " not supported - NON FATAL ERROR\n";
    return false;
  }

  image=image_;
  return true;
}


//: Attempt to save image to named file
// \param filetype  String defining what format to save in
bool mil_byte_image_2d_io::saveTheImage(const mil_image_2d_of<vil_byte>& image,
            const vcl_string& path,
            const vcl_string& f_type)
{
  vcl_string filetype = f_type;
  if (f_type=="")
    filetype=guessFileType(path);

  if (filetype=="Unknown")
  {
    vcl_cerr<<"File type for "<<path<<" unknown\n";
    return false;
  }

  //convert mil_image to vil_image
  //if colour=>vil_memory_image_of<vil_rgb_byte>
  //if grey=>vil_memory_image_of<vil_byte>
  //then use vil_save with appropriate "filetype"

  bool image_is_grey=false,image_is_colour=false;
  if (image.n_planes()==1)
    image_is_grey=true;
  else if (image.n_planes()==3)
    image_is_colour=true;
  else
  {
    vcl_cerr<<"Failed to save: number of planes = "<<image.n_planes()<<" ??, require 1 or 3\n";
    return false;
  }

  if (image_is_grey)
  {
    vil_memory_image_of<vil_byte> buf;
    mil_convert_vil_gm2gv(buf,image);
    vil_save(buf,path.c_str(),filetype.c_str());
    return true;
  }

  else if (image_is_colour)
  {
    vil_memory_image_of<vil_rgb_byte> buf;
    mil_convert_vil_cm2cv(buf,image);
    vil_save(buf,path.c_str(),filetype.c_str());
    return true;
  }
  else
  return false;
}


//=======================================================================
// Method: is_a
//=======================================================================

vcl_string mil_byte_image_2d_io::is_a() const
{
  return vcl_string("mil_byte_image_2d_io");
}

//=======================================================================
// Method: is_class
//=======================================================================

bool mil_byte_image_2d_io::is_class(vcl_string const& s) const
{
  return s==mil_byte_image_2d_io::is_a() || mil_image_io::is_class(s);
}

//=======================================================================
// Method: version_no
//=======================================================================

short mil_byte_image_2d_io::version_no() const
{
  return 1;
}

//=======================================================================
// Method: clone
//=======================================================================

mil_image_io* mil_byte_image_2d_io::clone() const
{
  return new mil_byte_image_2d_io(*this);
}

//=======================================================================
// Method: print
//=======================================================================

void mil_byte_image_2d_io::print_summary(vcl_ostream&) const
{
}

//=======================================================================
// Method: save
//=======================================================================

void mil_byte_image_2d_io::b_write(vsl_b_ostream& bfs) const
{
  vsl_b_write(bfs,version_no());
}

//=======================================================================
// Method: load
//=======================================================================

void mil_byte_image_2d_io::b_read(vsl_b_istream& bfs)
{
  if (!bfs) return;

  short version;
  vsl_b_read(bfs,version);
  switch (version)
  {
  case (1):
    break;
  default:
    vcl_cerr << "I/O ERROR: mil_byte_image_2d_io::b_read(vsl_b_istream&)\n"
             << "           Unknown version number "<< version << "\n";
    bfs.is().clear(vcl_ios::badbit); // Set an unrecoverable IO error on stream
    return;
  }
}
