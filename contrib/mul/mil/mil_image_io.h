#ifndef mil_image_io_h_
#define mil_image_io_h_
#ifdef __GNUC__
#pragma interface
#endif

//: \file
//  \brief Class to load and save images from named files
//  \author Tim Cootes

#include <vsl/vsl_binary_io.h>
#include <vcl_string.h>


class mil_image;

//: Class to load and save images from named files
//  Base class for objects which load and save images of arbitrary type.
//  This allows one to write classes which load in images without
//  knowing in advance what type they are going to be.
//
//  Derived classes will store the image which has been loaded by a
//  call to loadImage(filename,filetype).  It can then be accessed
//  by a call to image()
class mil_image_io {

public:

    //: Dflt ctor
    mil_image_io();

    //: Destructor
    virtual ~mil_image_io();

    //: Current image
    //  (The one generated by last call to b_read())
    virtual const mil_image& image() const =0;

    //: Attempt to load image from named file
    //!in: filetype: String hinting at what image format is  (valid options depend on derived class)
    //!ret: true if successfull
    //  If successful, image() will return the image which was loaded.
    virtual bool loadImage(const vcl_string& path,
                           const vcl_string& filetype) =0;

    //: Attempt to save image to named file
    //!in: filetype: String defining what format to save in (valid options depend on derived class)
    virtual bool saveImage(const vcl_string& path,
                           const vcl_string& filetype) const =0;

    //: Version number for I/O
    short version_no() const;

    //: Name of the class
    virtual vcl_string is_a() const;

    //: Does the name of the class match the argument?
    virtual bool is_class(vcl_string const& s) const;

    //: Create a copy on the heap and return base class pointer
    virtual mil_image_io* clone() const = 0;

    //: Print class to os
    virtual void print_summary(vcl_ostream& os) const = 0;

    //: Save class to binary file stream
    //!in: bfs: Target binary file stream
    virtual void b_write(vsl_b_ostream& bfs) const = 0;

    //: Load class from binary file stream
    //!out: bfs: Target binary file stream
    virtual void b_read(vsl_b_istream& bfs) = 0;
};

    //: Allows derived class to be loaded by base-class pointer
    //  A loader object exists which is invoked by calls
    //  of the form "vsl_b_read(bfs,base_ptr);".  This loads derived class
    //  objects from the disk, places them on the heap and
    //  returns a base class pointer.
    //  In order to work the loader object requires
    //  an instance of each derived class that might be
    //  found.  This function gives the model class to
    //  the appropriate loader.
void vsl_add_to_binary_loader(const mil_image_io& b);

    //: Binary file stream output operator for class reference
void vsl_b_write(vsl_b_ostream& bfs, const mil_image_io& b);

    //: Binary file stream input operator for class reference
void vsl_b_read(vsl_b_istream& bfs, mil_image_io& b);

    //: Stream output operator for class reference
vcl_ostream& operator<<(vcl_ostream& os,const mil_image_io& b);

    //: Stream output operator for class pointer
vcl_ostream& operator<<(vcl_ostream& os,const mil_image_io* b);

#endif // mil_image_io_h_
