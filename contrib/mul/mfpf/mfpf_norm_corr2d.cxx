//:
// \file
// \brief Searches over a grid using normalised correllation
// \author Tim Cootes


#include <mfpf/mfpf_norm_corr2d.h>
#include <vsl/vsl_indent.h>
#include <vsl/vsl_binary_loader.h>
#include <vul/vul_string.h>
#include <vcl_cmath.h>
#include <vcl_algorithm.h>

#include <mbl/mbl_parse_block.h>
#include <mbl/mbl_read_props.h>
#include <mbl/mbl_cloneables_factory.h>

#include <vil/vil_resample_bilin.h>
#include <vil/io/vil_io_image_view.h>
#include <vil/vil_math.h>
#include <vil/vil_print.h>

//=======================================================================
// Dflt ctor
//=======================================================================

mfpf_norm_corr2d::mfpf_norm_corr2d()
{
  set_defaults();
}

//: Define default values
void mfpf_norm_corr2d::set_defaults()
{
  step_size_=1.0;
  search_ni_=5;
  search_nj_=5;
}

//=======================================================================
// Destructor
//=======================================================================

mfpf_norm_corr2d::~mfpf_norm_corr2d()
{
}

void mfpf_norm_corr2d::set_step_size(double s)
{
  step_size_=s;
}

//: Define filter kernel to search with
void mfpf_norm_corr2d::set(const vil_image_view<double>& k, 
                           double ref_x, double ref_y)
{
  // Copy, ensuring istep==1
  unsigned ni=k.ni(),nj=k.nj();
  kernel_.set_size(ni,nj);
  double sum=0.0,ss=0.0;
  for (unsigned j=0;j<nj;++j)
    for (unsigned i=0;i<ni;++i)
    {
      kernel_(i,j)=k(i,j);
      sum+=k(i,j); ss+=k(i,j)*k(i,j);
    }

  // Normalise so that kernel_ has zero mean and unit sum of squares.
  double mean=sum/(ni*nj);
  ss-=(mean*mean*ni*nj);
  double s=1.0;
  if (ss>0) s = vcl_sqrt(1.0/ss);
  vil_math_scale_and_offset_values(kernel_,s,-s*mean);

  ref_x_ = ref_x;
  ref_y_ = ref_y;
}

//: Define filter kernel to search with
void mfpf_norm_corr2d::set(const vil_image_view<double>& k)
{
  set(k, 0.5*(k.ni()-1.0), 0.5*(k.nj()-1.0));
}

void mfpf_norm_corr2d::set_search_area(unsigned ni, unsigned nj)
{
  search_ni_=ni;
  search_nj_=nj;
}

// Assumes im2[i] has zero mean and unit length as a vector
// Assumes element (i,j) is im1[i+j*jstep1] etc
inline double norm_corr(const float* im1, const double* im2, 
                        vcl_ptrdiff_t jstep1, vcl_ptrdiff_t jstep2, 
                        unsigned ni, unsigned nj)
{
  double sum1=0.0,sum2=0.0,sum_sq=0.0;
  for (unsigned j=0;j<nj;++j,im1+=jstep1,im2+=jstep2)
    for (unsigned i=0;i<ni;++i)
    {
      sum1+=im1[i]*im2[i];
      sum2+=im1[i];
      sum_sq+=im1[i]*im1[i];
    }
  unsigned n=ni*nj;
  double mean = sum2/n;
  double ss = vcl_max(1e-6,sum_sq-n*mean*mean);
  double s = vcl_sqrt(ss);
  return sum1/s;
}

//: Evaluate match at p, using u to define scale and orientation 
// Returns -1*edge strength at p along direction u
double mfpf_norm_corr2d::evaluate(const vimt_image_2d_of<float>& image,
                        const vgl_point_2d<double>& p,
                        const vgl_vector_2d<double>& u)
{
  vgl_vector_2d<double> u1=step_size_*u;
  vgl_vector_2d<double> v1(-u1.y(),u1.x());

  vil_image_view<float> sample;

  const vgl_point_2d<double> p0 = p-ref_x_*u1-ref_y_*v1;

  const vimt_transform_2d& s_w2i = image.world2im();
  vgl_point_2d<double> im_p0 = s_w2i(p0);
  vgl_vector_2d<double> im_u = s_w2i.delta(p0, u1);
  vgl_vector_2d<double> im_v = s_w2i.delta(p0, v1);

  vil_resample_bilin(image.image(),sample,
                      im_p0.x(),im_p0.y(),  im_u.x(),im_u.y(),
                      im_v.x(),im_v.y(), 
                      kernel_.ni(),kernel_.nj());

  return -1*norm_corr(sample.top_left_ptr(),kernel_.top_left_ptr(),
                      sample.jstep(),kernel_.jstep(),
                      kernel_.ni(),kernel_.nj());
}

//: Evaluate match at in a region around p 
// Returns a qualtity of fit at a set of positions.
// response image (whose size and transform is set inside the
// function), indicates the points at which the function was
// evaluated.  response(i,j) is the fit at the point
// response.world2im().inverse()(i,j).  The world2im() transformation
// may be affine.
void mfpf_norm_corr2d::evaluate_region(
                        const vimt_image_2d_of<float>& image,
                        const vgl_point_2d<double>& p,
                        const vgl_vector_2d<double>& u,
                        vimt_image_2d_of<double>& response)
{
  assert(image.image().nplanes()==1);
  int ni=1+2*search_ni_;
  int nj=1+2*search_nj_;
  int nsi = 2*search_ni_ + kernel_.ni();
  int nsj = 2*search_nj_ + kernel_.nj();

  vil_image_view<float> sample;
  vgl_vector_2d<double> u1=step_size_*u;
  vgl_vector_2d<double> v1(-u1.y(),u1.x());
  const vgl_point_2d<double> p0 = p-(search_ni_+ref_x_)*u1
                                   -(search_nj_+ref_y_)*v1;

  const vimt_transform_2d& s_w2i = image.world2im();
  vgl_point_2d<double> im_p0 = s_w2i(p0);
  vgl_vector_2d<double> im_u = s_w2i.delta(p0, u1);
  vgl_vector_2d<double> im_v = s_w2i.delta(p0, v1);

  vil_resample_bilin(image.image(),sample,
                      im_p0.x(),im_p0.y(),  im_u.x(),im_u.y(),
                      im_v.x(),im_v.y(), 
                      nsi,nsj);

  response.image().set_size(ni,nj);
  double* r = response.image().top_left_ptr();
  const double* k = kernel_.top_left_ptr();
  const float* s = sample.top_left_ptr();
  vcl_ptrdiff_t r_jstep = response.image().jstep();
  vcl_ptrdiff_t s_jstep = sample.jstep();
  vcl_ptrdiff_t k_jstep = kernel_.jstep();

  for (unsigned j=0;j<nj;++j,r+=r_jstep,s+=s_jstep)
  {
    for (int i=0;i<ni;++i)
    {
      r[i] = -1*norm_corr(s+i,k,s_jstep,k_jstep,
                          kernel_.ni(),kernel_.nj());
    }
  }

  // Set up transformation parameters

  // Point (i,j) in dest corresponds to p1+i.u+j.v,
  // an affine transformation for image to world
  const vgl_point_2d<double> p1 = p-search_ni_*u1-search_nj_*v1;

  vimt_transform_2d i2w;
  i2w.set_similarity(vgl_point_2d<double>(u1.x(),u1.y()),p1);
  response.set_world2im(i2w.inverse());
}

   //: Search given image around p, using u to define scale and orientation 
   //  On exit, new_p and new_u define position, scale and orientation of 
   //  the best nearby match.  Returns a qualtity of fit measure at that
   //  point (the smaller the better).
double mfpf_norm_corr2d::search(const vimt_image_2d_of<float>& image,
                        const vgl_point_2d<double>& p,
                        const vgl_vector_2d<double>& u,
                        vgl_point_2d<double>& new_p,
                        vgl_vector_2d<double>& new_u)
{
  int ni=1+2*search_ni_;
  int nj=1+2*search_nj_;
  int nsi = 2*search_ni_ + kernel_.ni();
  int nsj = 2*search_nj_ + kernel_.nj();

  vil_image_view<float> sample;
  vgl_vector_2d<double> u1=step_size_*u;
  vgl_vector_2d<double> v1(-u1.y(),u1.x());
  const vgl_point_2d<double> p0 = p-(search_ni_+ref_x_)*u1
                                   -(search_nj_+ref_y_)*v1;

  const vimt_transform_2d& s_w2i = image.world2im();
  vgl_point_2d<double> im_p0 = s_w2i(p0);
  vgl_vector_2d<double> im_u = s_w2i.delta(p0, u1);
  vgl_vector_2d<double> im_v = s_w2i.delta(p0, v1);

  vil_resample_bilin(image.image(),sample,
                      im_p0.x(),im_p0.y(),  im_u.x(),im_u.y(),
                      im_v.x(),im_v.y(), 
                      nsi,nsj);

  const double* k = kernel_.top_left_ptr();
  const float* s = sample.top_left_ptr();
  vcl_ptrdiff_t s_jstep = sample.jstep();
  vcl_ptrdiff_t k_jstep = kernel_.jstep();

  double best_r=-9.99e9;
  int best_i,best_j;
  for (unsigned j=0;j<nj;++j,s+=s_jstep)
  {
    for (int i=0;i<ni;++i)
    {
      double r = norm_corr(s+i,k,s_jstep,k_jstep,
                           kernel_.ni(),kernel_.nj());
      if (r>best_r) { best_r=r; best_i=i; best_j=j; }
    }
  }

  // Compute position of best point
  new_p = p+(best_i-search_ni_)*u1+(best_j-search_nj_)*v1;
  new_u = u;
  return -1.0 * best_r;
}

//=======================================================================
// Method: set_from_stream
//=======================================================================
//: Initialise from a string stream
bool mfpf_norm_corr2d::set_from_stream(vcl_istream &is)
{
  // Cycle through string and produce a map of properties
  vcl_string s = mbl_parse_block(is);
  vcl_istringstream ss(s);
  mbl_read_props_type props = mbl_read_props_ws(ss);

  set_defaults();

  // Extract the properties
  if (props.find("step_size")!=props.end())
  { 
    step_size_=vul_string_atof(props["step_size"]); 
    props.erase("step_size"); 
  }
  if (props.find("search_ni")!=props.end())
  { 
    search_ni_=vul_string_atoi(props["search_ni"]); 
    props.erase("search_ni"); 
  }
  if (props.find("search_nj")!=props.end())
  { 
    search_nj_=vul_string_atoi(props["search_nj"]); 
    props.erase("search_nj"); 
  }

  // Check for unused props
  mbl_read_props_look_for_unused_props(
      "mfpf_norm_corr2d::set_from_stream", props, mbl_read_props_type());
  return true;
}

//=======================================================================
// Method: is_a
//=======================================================================

vcl_string mfpf_norm_corr2d::is_a() const
{
  return vcl_string("mfpf_norm_corr2d");
}

//: Create a copy on the heap and return base class pointer
mfpf_point_finder* mfpf_norm_corr2d::clone() const
{
  return new mfpf_norm_corr2d(*this);
}

//=======================================================================
// Method: print
//=======================================================================

void mfpf_norm_corr2d::print_summary(vcl_ostream& os) const
{
  os<<"{ step_size: "<<step_size_;
  os<<" size: "<<kernel_.ni()<<" x "<<kernel_.nj();
  os<<" search_ni: "<<search_ni_;
  os<<" search_nj: "<<search_nj_;
  os<<" }";
}

void mfpf_norm_corr2d::b_write(vsl_b_ostream& bfs) const
{
  vsl_b_write(bfs,version_no());
  vsl_b_write(bfs,step_size_); 
  vsl_b_write(bfs,kernel_); 
  vsl_b_write(bfs,ref_x_); 
  vsl_b_write(bfs,ref_y_); 
  vsl_b_write(bfs,search_ni_); 
  vsl_b_write(bfs,search_nj_); 
}

//=======================================================================
// Method: load
//=======================================================================

void mfpf_norm_corr2d::b_read(vsl_b_istream& bfs)
{
  if (!bfs) return;
  short version;
  vsl_b_read(bfs,version);
  switch (version)
  {
    case (1):
      vsl_b_read(bfs,step_size_);
      vsl_b_read(bfs,kernel_);
      vsl_b_read(bfs,ref_x_);
      vsl_b_read(bfs,ref_y_);
      vsl_b_read(bfs,search_ni_);
      vsl_b_read(bfs,search_nj_);
      break;
    default:
      vcl_cerr << "I/O ERROR: vsl_b_read(vsl_b_istream&) \n";
      vcl_cerr << "           Unknown version number "<< version << vcl_endl;
      bfs.is().clear(vcl_ios::badbit); // Set an unrecoverable IO error on stream
      return;
  }
}

//: Test equality
bool mfpf_norm_corr2d::operator==(const mfpf_norm_corr2d& nc) const
{
  if (kernel_.ni()!=nc.kernel_.ni()) return false;
  if (kernel_.nj()!=nc.kernel_.nj()) return false;
  if (search_ni_!=nc.search_ni_) return false;
  if (search_nj_!=nc.search_nj_) return false;
  if (vcl_fabs(ref_x_-nc.ref_x_)>1e-6) return false;
  if (vcl_fabs(ref_y_-nc.ref_y_)>1e-6) return false;
  if (vcl_fabs(step_size_-nc.step_size_)>1e-6) return false;
  if (kernel_.size()!=nc.kernel_.size()) return false;
  if (kernel_.size()==0) return true;  // ssd fails on empty 
  return (vil_math_ssd(kernel_,nc.kernel_,double(0))<1e-4);
}


