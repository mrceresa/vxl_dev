#ifndef vidl_image_list_codec_h
#define vidl_image_list_codec_h

//:
// \file 
// \author Nicolas Dano, september 1999
// Modifications
// \verbatim
// Julien ESTEVE, June 2000
//     Ported from TargetJr
// 10/4/2001 Ian Scott (Manchester) Coverted perceps header to doxygen
// \endverbatim


#include <vcl_string.h>
#include <vbl/vbl_ref_count.h>
#include <vidl/vidl_image_list_codec_sptr.h>
#include <vidl/vidl_codec.h>
#include <vil/vil_image.h>
#include <vcl_list.h>
#include <vcl_vector.h>
//: Coder/decoder for a list of images
//   A vidl_image_list_codec is the video I/O access for list of images.
class vidl_image_list_codec :  public vidl_codec
{
  // PUBLIC INTERFACE----------------------------------------------------------
public:

  // Constructors
  vidl_image_list_codec();
  vidl_image_list_codec(vcl_list<vil_image>& images);
  vidl_image_list_codec(vcl_vector<vil_image>& images);
  // Destructor
  ~vidl_image_list_codec();

  // Operators-----------------------------------------------------------------
  // Safe cast to a parent from Image
  virtual vidl_image_list_codec* casttovidl_image_list_codec() { return this; }


  virtual bool get_section(int position, void* ib, int x0, int y0, int w, int h) const;
  virtual int put_section(int position, void* ib, int x0, int y0, int w, int h);
  // IO
  virtual vidl_codec_sptr load(const char* fname, char mode = 'r' );
  virtual vidl_codec_sptr load(const vcl_list<vcl_string> &fnames, char mode = 'r');
  virtual vidl_codec_sptr load(const vcl_vector<vcl_string> &fnames, char mode = 'r');
  virtual bool save(vidl_movie* movie, const char* fname);
  virtual bool save(vidl_movie* movie, const char* fname, const char* type);
  virtual bool probe(const char* fname);

  virtual const char* type() {return "ImageList";}

  // Specific to vidl_image_list_codec
  static void set_default_image_type(const char* type)
        {strcpy(default_initialization_image_type_,type);}

  virtual void set_image_type(const char* type)
        {strcpy(default_image_type_,type);}

  virtual const char* get_image_type() const {return default_image_type_;}
  vcl_vector<vil_image> get_images() const {return images_;}

  // Register image loaders
  //virtual void register_image_loaders ();

protected:
  virtual bool init();

  // Data Members--------------------------------------------------------------
  vcl_vector<vil_image> images_;

private:
  char* default_image_type_;
  static char* default_initialization_image_type_;

};

#endif // vidl_image_list_codec_h
