#ifndef vcl_egcs_vector_txx_
#define vcl_egcs_vector_txx_

#include <vcl/vcl_vector.h>
#include <vcl/vcl_compiler.h>

#define VCL_VECTOR_STLINST_uninitialized_copy(Inp, Fwd, Size) \
template Fwd __uninitialized_copy_aux(Inp, Inp, Fwd, __false_type);\
template pair<Inp, Fwd> __uninitialized_copy_n(Inp, Size, Fwd, input_iterator_tag);\
VCL_INSTANTIATE_INLINE(Fwd uninitialized_copy(Inp, Inp, Fwd))


#undef VCL_VECTOR_INSTANTIATE
#define VCL_VECTOR_INSTANTIATE(T) \
template vector<T >::iterator __uninitialized_fill_n_aux(vector<T >::iterator, vector<T >::size_type, T const &, __false_type); \
template void fill(vector<T >::iterator, vector<T >::iterator, T const &); \
template vector<T >::iterator fill_n(vector<T >::iterator, vector<T >::size_type, T const &);\
INSTANTIATE_COPY(vector<T >::const_iterator, vector<T >::iterator);\
STLINST_uninitialized_copy(vector<T >::const_iterator, vector<T >::iterator, vector<T >::size_type);\
template class vector<T >

#endif
