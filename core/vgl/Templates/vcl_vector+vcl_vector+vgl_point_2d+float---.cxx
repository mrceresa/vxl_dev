#include <vgl/vgl_point_2d.h>
#include <vcl_vector.txx>

typedef vcl_vector<vgl_point_2d<float> > v;

VCL_VECTOR_INSTANTIATE(v);

#include <vcl_algorithm.txx>
VCL_CONTAINABLE_INSTANTIATE(v);
