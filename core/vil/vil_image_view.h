// This is mul/vil2/vil2_image_view.h
#ifndef vil2_image_view_h_
#define vil2_image_view_h_
#ifdef VCL_NEEDS_PRAGMA_INTERFACE
#pragma interface
#endif
//:
// \file
// \brief A base class reference-counting view of some image data.
// \author Ian Scott - Manchester

#include <vcl_iosfwd.h>
#include <vcl_string.h>
#include <vcl_cassert.h>
#include <vil2/vil2_image_view_base.h>
#include <vil2/vil2_smart_ptr.h>
#include <vil2/vil2_memory_chunk.h>
#include <vil2/vil2_pixel_format.h>


//: Concrete view of image data of type T held in memory
//  Views nplanes() planes of data each of size ni() x nj().
//  The with the (i,j) element of the p'th plane is given by
//  im.top_left_ptr()[i*im.istep() + j*im.jstep() + p*im.planestep]
//  The actual image data is either allocated by the class
//  (using resize), in which case it is deleted
//  only when it has no views observing it, or is allocated outside (and is not deleted on
//  destruction).  This allows external images to be accessed
//  without a deep copy.
//
//  Note that copying one vil2_image_view<T> to another takes a shallow
//  copy by default - it copies the view, not the raw image data.
//  Use the explicit deep_copy() call to take a deep copy.

template <class T>
class vil2_image_view : public vil2_image_view_base
{
protected:
  //: Pointer to pixel at origin.
  T * top_left_;
  //: Add this to a pixel pointer to move one column left.
  int istep_;
  //: Add this to a pixel pointer to move one row down.
  int jstep_;
  //: Add this to a pixel pointer to move one plane back.
  int planestep_;

  //: Reference to actual image data.
  vil2_memory_chunk_sptr ptr_;

  //: Disconnect this view from the underlying data,
  void release_memory()
  { ptr_ = 0; }



public:

    //: Dflt ctor
    //  Creates an empty one plane image.
  vil2_image_view();

    //: Create a n_plane plane image of ni x nj pixels
  vil2_image_view(unsigned ni, unsigned nj, unsigned n_planes=1);

    //: Set this view to look at someone else's memory data.
    //  If the data goes out of scope then this view could be invalid, and
    //  there's no way of knowing until its too late - so take care!
  vil2_image_view(const T* top_left, unsigned ni, unsigned nj, unsigned nplanes,
                  int istep, int jstep, int planestep);

    //: Set this view to look at another view's data
    //  Typically used by functions which generate a manipulated view of
    //  another's image data.
    //  Need to pass the memory chunk to set up the internal smart ptr appropriately
  vil2_image_view(const vil2_smart_ptr<vil2_memory_chunk>& mem_chunk,
                  const T* top_left, unsigned ni, unsigned nj, unsigned nplanes,
                  int istep, int jstep, int planestep);

    //: Construct from various vil2_image_view types.
    // The new object will point to the same underlying image as the rhs
    // You can assign a vil2_image_view<compound_type<T>> to a vil2_image_view<T>
    // in all reasonable cases - the lhs will have as many planes as the rhs has
    // components. You can assign a vil2_image_view<T> to a vil2_image_view<compound_type<T>>
    // when the underlying data is formatted appropriately and the lhs has
    // as many components as the rhs has planes. O(1).
    // If the view types are not compatible this object will be set to empty.
  vil2_image_view(const vil2_image_view_base& rhs);

    //: Construct from various vil2_image_view types.
    // The new object will point to the same underlying image as the rhs.
    //
    // You can assign a vil2_image_view<compound_type<T>> to a vil2_image_view<T>
    // in all reasonable cases - the lhs will have as many planes as the rhs has
    // components. You can assign a vil2_image_view<T> to a vil2_image_view<compound_type<T>>
    // when the underlying data is formatted appropriately and the lhs has
    // as many components as the rhs has planes. O(1).
    // If the view types are not compatible this object will be set to empty.
  vil2_image_view(const vil2_image_view_base_sptr& rhs);

    //  Destructor
  virtual ~vil2_image_view();

  // Standard container stuff
  // This assumes that the data is arranged contiguously.
  // Is this assumption good?

  //: The pixel type of this image
  typedef T pixel_type;

  //: True if data all in one unbroken block
  bool is_contiguous() const;

  // iterators
  typedef T *iterator;
  inline iterator begin() { assert(is_contiguous()); return top_left_; }
  inline iterator end  () { assert(is_contiguous()); return top_left_ + size(); }

  typedef T const *const_iterator;
  inline const_iterator begin() const { assert(is_contiguous()); return top_left_; }
  inline const_iterator end  () const { assert(is_contiguous()); return top_left_ + size(); }

  // aritmetic indexing stuff

  //: Pointer to the first (top left in plane 0) pixel;
  T * top_left_ptr() { return top_left_; }  // Make origin explicit
  //: Pointer to the first (top left in plane 0) pixel;
  const T * top_left_ptr() const { return top_left_; }

  //: Add this to your pixel pointer to get next i pixel
  int istep() const { return istep_; }
  //: Add this to your pixel pointer to get next j pixel
  int jstep() const { return jstep_; }
  //: Add this to your pixel pointer to get pixel on next plane
  int planestep() const { return planestep_; }

  //: Cast to bool is true if pointing at some data.
  operator bool () const { return top_left_ != (T*)0; }

  //: The number of bytes in the data
  inline unsigned size_bytes() const { return size() * sizeof(T); }

    //: Smart pointer to the object holding the data for this view
    // Will be a null pointer if this view looks at `third-party' data,
    // eg using set_to_memory.
    //
    // Typically used when creating new views of the data
  const vil2_memory_chunk_sptr& memory_chunk() const { return ptr_; }

    //: Smart pointer to the object holding the data for this view
    // Will be a null pointer if this view looks at `third-party' data,
    // eg using set_to_memory
    //
    // Typically used when creating new views of the data
  vil2_memory_chunk_sptr& memory_chunk() { return ptr_; }

  // Ordinary image indexing stuff.

  //: Return read-only reference to pixel at (i,j) in plane 0.
  const T& operator()(unsigned i, unsigned j) const {
    assert(i<ni_); assert(j<nj_);
    return top_left_[jstep_*j+i*istep_]; }

  //: Return read/write reference to pixel at (i,j) in plane 0.
  T&       operator()(unsigned i, unsigned j) {
    assert(i<ni_); assert(j<nj_);
    return top_left_[istep_*i+j*jstep_]; }

  //: Return read-only reference to pixel at (i,j) in plane p.
  const T& operator()(unsigned i, unsigned j, unsigned p) const {
  assert(i<ni_); assert(j<nj_);
   return top_left_[planestep_*p + jstep_*j + i*istep_]; }

  //: Return read-only reference to pixel at (i,j) in plane p.
  T&       operator()(unsigned i, unsigned j, unsigned p) {
  assert(i<ni_); assert(j<nj_);
   return top_left_[planestep_*p + jstep_*j + i*istep_]; }


// image stuff

  //: resize current planes to ni x nj
  // If already correct size, this function returns quickly
  virtual void resize(unsigned ni, unsigned nj);

  //: resize to ni x ni x nplanes
  // If already correct size, this function returns quickly
  virtual void resize(unsigned ni, unsigned nj, unsigned nplanes);

  //: Make a copy of the data in src and set this to view it
  void deep_copy(const vil2_image_view<T>& src);

  //: Make empty.
  // Disconnects view from underlying data.
  void clear()
  {
    release_memory();
    ni_=nj_=nplanes_=0;
    top_left_=0;
  }

  //: Set this view to look at someone else's memory data.
  //  If the data goes out of scope then this view could be invalid, and
  //  there's no way of knowing until its too late - so take care!
  //
  //  Note that though top_left is passed in as const, the data may be manipulated
  //  through the view.
  void set_to_memory(const T* top_left, unsigned ni, unsigned nj, unsigned nplanes,
              int istep, int jstep, int planestep);

  //: Arrange that this is window on some planes of given image.
  //  i.e. plane(i) points to im.plane(i+p0) + offset
  void set_to_window(const vil2_image_view& im,
                     unsigned i0, unsigned ni, unsigned j0,
                     unsigned nj, unsigned p0, unsigned np);

  //: Arrange that this is window on all planes of given image.
  void set_to_window(const vil2_image_view& im,
                     unsigned i0, unsigned ni, unsigned j0, unsigned nj);

  //: Fill view with given value
  void fill(T value);

  //: Print a 1-line summary of contents
  virtual void print(vcl_ostream&) const;

    //: print all image data to os in a grid (rounds output to int)
  virtual void print_all(vcl_ostream& os) const;

    //: Return class name
  virtual vcl_string is_a() const;

    //: True if this is (or is derived from) class s
  virtual bool is_class(vcl_string const& s) const;

    //: Return a description of the concrete data pixel type.
    // The value corresponds directly to pixel_type.
  virtual enum vil2_pixel_format pixel_format() const
  { return vil2_pixel_format_of(T()); }

    //: True if they share same view of same image data.
    //  This does not do a deep equality on image data. If the images point
    //  to different image data objects that contain identical images, then
    //  the result will still be false.
  bool operator==(const vil2_image_view<T> &other) const;

  //: Copy a view. The rhs and lhs will point to the same image data.
  // You can assign a vil2_image_view<compound_type<T>> to a vil2_image_view<T>
  // in all reasonable cases - the lhs will have as many planes as the rhs has
  // components. You can assign a vil2_image_view<T> to a vil2_image_view<compound_type<T>>
  // when the underlying data is formatted appropriately and the lhs has
  // as many components as the rhs has planes. O(1).
  // If the view types are not compatible this object will be set to empty.
  const vil2_image_view<T> & operator = (const vil2_image_view_base & rhs);

  //: Copy a view. The rhs and lhs will point to the same image data.
  // You can assign a vil2_image_view<compound_type<T>> to a vil2_image_view<T>
  // in all reasonable cases - the lhs will have as many planes as the rhs has
  // components. You can assign a vil2_image_view<T> to a vil2_image_view<compound_type<T>>
  // when the underlying data is formatted appropriately and the lhs has
  // as many components as the rhs has planes. O(1).
  // If the view types are not compatible this object will be set to empty.
  const vil2_image_view<T> & operator = (const vil2_image_view_base_sptr & rhs)
  {
    if (!rhs) clear();
    else *this = *rhs;
    return *this;
  }

};

//: Print a 1-line summary of contents
template <class T>
inline
vcl_ostream& operator<<(vcl_ostream& s, vil2_image_view<T> const& im) {
  im.print(s); return s; }



//: True if the actual images are identical.
// $\bigwedge_{i,j,p} {\textstyle src}(i,j,p) == {\textstyle dest}(i,j,p)$
// The data may be formatted differently in each memory chunk.
//  O(size).
// \relates vil2_image_view
template<class T>
bool vil2_deep_equality(const vil2_image_view<T> &lhs, const vil2_image_view<T> &rhs);


#endif // vil_image_view_h_
