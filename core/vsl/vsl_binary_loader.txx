#ifndef vsl_binary_loader_txx_
#define vsl_binary_loader_txx_

#include <vsl/vsl_binary_loader.h>
#include <vcl_vector.txx>

template<class BaseClass>
vsl_binary_loader<BaseClass>& vsl_binary_loader<BaseClass>::instance()
{
  if (instance_ == 0)
  {
    instance_ = new vsl_binary_loader<BaseClass>;

    // Register for deletion by vsl_delete_all_loaders()
    instance_->register_this();
  }
  return *instance_;
}

template<class BaseClass>
void vsl_binary_loader<BaseClass>::make_empty()
{
    for (int i=0; i<object_.size(); ++i)
      delete object_[i];
    object_.resize(0);
}

template<class BaseClass>
vsl_binary_loader<BaseClass>::~vsl_binary_loader()
{
    make_empty();
    instance_=0;
}

// IO for  pointers to BaseClass:
template<class BaseClass>
void vsl_binary_loader<BaseClass>::load_object( vsl_b_istream& is, BaseClass*& b)
{

  // HELP ON RUN-TIME ERROR HERE
  // If you get a run-time error here it is most-likely because you called
  // vsl_b_read with an uninitialised null base_class pointer. The base class
  // pointer should either point to a real object, or be set to 0 - IMS.
  delete b;   // Delete old object pointed to by b

  vcl_string name;
  vsl_b_read(is,name);

  if (name=="VSL_NULL_PTR")
  {
    // Zero pointer
    b=0;
    return;
  }

  int i;
  for (i=0; (i<object_.size()) && !(object_[i]->is_a()==name); i++);

  if (i<object_.size())
  {
    b = object_[i]->clone(); // If you get a compile error here you are probably trying to load into a non-base class pointer.
    b->b_read(is);
  }
  else
  {
    vcl_cerr << "ERROR: vsl_binary_loader<BaseClass>::load_object: ";
    vcl_cerr << "class name <" << name << "> not in list of loaders"<<vcl_endl;
    vcl_cerr << object_.size()<<" valid loaders: "<<vcl_endl;
    for (int j=0; j<object_.size(); ++j)
      vcl_cerr << object_[j]->is_a() << vcl_endl;
    vcl_abort();
  }
}

template <class BaseClass>
vsl_binary_loader<BaseClass>* vsl_binary_loader<BaseClass>::instance_ = 0;

#undef VSL_BINARY_LOADER_INSTANTIATE
#define VSL_BINARY_LOADER_INSTANTIATE(T) \
template class vsl_binary_loader<T >; \
template void vsl_b_read( vsl_b_istream& bfs, T*& b); \
template void vsl_b_write(vsl_b_ostream& bfs, const T* b); \
VCL_VECTOR_INSTANTIATE(T*)


#endif // vsl_binary_loader_txx_
