#ifndef vcl_gcc_295_list_txx_
#define vcl_gcc_295_list_txx_

#include <vcl/vcl_list.h>

#define VCL_LIST_INSTANTIATE_ITERATOR(Inp, Fwd, Diff) \
template Fwd std::copy(Inp, Inp, Fwd);\
template Fwd std::copy_backward(Inp, Inp, Fwd);\
template void std::advance(Fwd&, Diff);\
template void std::__advance(Fwd&, int, std::bidirectional_iterator_tag);


#define VCL_LIST_INSTANTIATE(T) \
VCL_LIST_INSTANTIATE_ITERATOR(vcl_list<T >::iterator, vcl_list<T >::iterator, vcl_list<T >::difference_type) \
template class vcl_list<T >;

#endif
