#ifndef vcl_egcs_list_txx_
#define vcl_egcs_list_txx_

#include <vcl/vcl_list.h>

// MT: Member template

#define VCL_LIST_INSTANTIATE_MT_InputIterator(T, InputIterator) \
template list<T >::list(InputIterator first, InputIterator last); \
template void list<T >::insert(list<T >::iterator, InputIterator, InputIterator); \

#define VCL_LIST_INSTANTIATE(T) \
template class list<T >;\
VCL_LIST_INSTANTIATE_MT_InputIterator(T, list<T >::iterator);\
VCL_LIST_INSTANTIATE_MT_InputIterator(T, list<T >::const_iterator);


// #ifdef __STL_MEMBER_TEMPLATES
// #define INSTANTIATE_LIST_MT_InputIterator(T, InputIterator)
// template list<T >::list(InputIterator first, InputIterator last);
// template void list<T >::insert(list<T >::iterator position, InputIterator first, InputIterator last);
// template void list<T >::range_initialize(InputIterator first, InputIterator last)
// #else
// #define INSTANTIATE_LIST_MT_InputIterator(T, InputIterator) /* no-op */
// #endif


#endif
