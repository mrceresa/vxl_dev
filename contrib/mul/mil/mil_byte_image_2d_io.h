#ifndef mil_byte_image_2d_io_h_
#define mil_byte_image_2d_io_h_
#ifdef __GNUC__
#pragma interface
#endif

//: \file
//  \brief Load and save mil_image_2d_of<vil_byte> from named files.
//  \author Tim Cootes

#include <mil/mil_image_io.h>
#include <mil/mil_image_2d_of.h>
#include <vil/vil_byte.h>

class vil_image;

class mil_image;

//: Load and save mil_image_2d_of<vil_byte> from named files.
class mil_byte_image_2d_io : public mil_image_io {
private:
  //: Current image object
  //  image() returns a reference to this
  mil_image_2d_of<vil_byte> image_;

  //: Define whether to load images as colour or grey-scale
  //  Options are '' (ie rely on image), 'Grey' or 'RGB'
  vcl_string colour_;

  vcl_string guessFileType(const vcl_string& path) const;

  void copyAsGrey(vil_image&);
  void copyGreyToRGB(vil_image&);
  void copyAsRGB(vil_image&);
public:

  //: Dflt ctor
  mil_byte_image_2d_io();

  //: Destructor
  virtual ~mil_byte_image_2d_io();

  //: Define whether to load images as colour or grey-scale
  //  Options are '' (ie rely on image), 'Grey' or 'RGB'
  void setColour(const vcl_string&);

  //: Whether to load images as RGB, Grey-scale or leave to image format
  //  Returns "RGB","Grey" or ""
  const vcl_string& colour() const;

  //: Current image
  //  (The one generated by last call to b_read())
  virtual const mil_image& image() const;

  //: Attempt to load image from named file
  //!in: filetype: String hinting at what image format is
  //!ret: true if successfull
  virtual bool loadImage(const vcl_string& path,
                         const vcl_string& filetype);

	//: Attempt to save image to named file
  //!in: filetype: String defining what format to save in
  //!ret: true if successfull
  virtual bool saveImage(const vcl_string& path,
                         const vcl_string& filetype) const;

 
  //: Attempt to load image from named file
  //!in: filetype: String hinting at what image format is
  //  If filetype=="" ,then guess the format from the path extension
  //  Return true if successfull
  bool loadTheImage(mil_image_2d_of<vil_byte>& image,
                    const vcl_string& path,
                    const vcl_string& filetype);

	//: Attempt to save image to named file
  //!in: filetype: String defining what format to save in
  bool saveTheImage(const mil_image_2d_of<vil_byte>& image,
                         const vcl_string& path,
                         const vcl_string& filetype) const;

  //: Version number for I/O
  short version_no() const;

  //: Name of the class
  virtual vcl_string is_a() const;

  //: Does the name of the class match the argument?
  virtual bool is_class(vcl_string const& s) const;

  //: Create a copy on the heap and return base class pointer
  virtual mil_image_io* clone() const;

  //: Print class to os
  virtual void print_summary(vcl_ostream& os) const;

  //: Save class to binary file stream
  //!in: bfs: Target binary file stream
  virtual void b_write(vsl_b_ostream& bfs) const;

  //: Load class from binary file stream
  //!out: bfs: Target binary file stream
  virtual void b_read(vsl_b_istream& bfs);
};

#endif // mil_byte_image_2d_io_h_
