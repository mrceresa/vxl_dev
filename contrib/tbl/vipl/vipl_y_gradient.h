#ifndef vipl_y_gradient_h_
#define vipl_y_gradient_h_

// .NAME vipl_y_gradient - Convolve image with vertical [-1 1] filter
// .LIBRARY vipl
// .HEADER vxl package
// .INCLUDE vipl/vipl_y_gradient.h
// .FILE vipl_y_gradient.txx
//
// .SECTION Description
//   This Image Processing class is implemented using the vipl filters,
//   which means that it can be used with any image class (IUE or not,
//   TargetJr or not, vil or not) of any pixel data type.
//
//   Note that the user has to make sure that the output values fall into
//   the output pixel range, by specifying appropriate scale and shift;
//   wrap-around will occur if it does not.
//
//   Note also the implicit use of DataOut::DataOut(DataIn),
//   which you probably will have to provide when DataIn and DataOut
//   are not the same type.
//
// .SECTION Author
//   Peter Vanroose, K.U.Leuven (ESAT/PSI), 1 June 1998.
//
// .SECTION Modifications
//   Peter Vanroose, Aug.2000 - adapted to vxl
//

#include <vipl/filter/vipl_filter_2d.h> // parent class

template <class ImgIn,class ImgOut,class DataIn,class DataOut, VCL_DFL_TYPE_PARAM_STLDECL(PixelItr, vipl_trivial_pixeliter) >
class vipl_y_gradient : public vipl_filter_2d<ImgIn,ImgOut,DataIn,DataOut,PixelItr> {
// -+-+- data members: -+-+-
  // term to add to the gradient result:
private: DataOut shift_;
public: DataOut shift() const { return shift_; }
  // scale factor to multiply the shifted gradient with:
private: double scale_;
public: double scale() const { return scale_; }

// -+-+- constructors/destructors: -+-+-
public:
  inline vipl_y_gradient(double s=1.0, DataOut h=DataOut())
           : vipl_filter_2d<ImgIn,ImgOut,DataIn,DataOut,PixelItr>(),
             shift_(h), scale_(s) {}
  inline vipl_y_gradient(vipl_y_gradient const& A)
           : vipl_filter_2d<ImgIn,ImgOut,DataIn,DataOut,PixelItr>(A),
             shift_(A.shift()), scale_(A.scale()) {}
  inline ~vipl_y_gradient() {}

// -+-+- required method for filters: -+-+-
  bool section_applyop();
};

#ifdef INSTANTIATE_TEMPLATES
#include "vipl_y_gradient.txx"
#endif

#endif // vipl_y_gradient_h_
