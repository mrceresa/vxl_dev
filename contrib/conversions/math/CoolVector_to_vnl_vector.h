#include <cool/Vector.h>
#include <vnl/vnl_vector.h>

template <class T>
inline vnl_vector<T > CoolVector_to_vnl_vector(CoolVector<T > const& v)
{
  T const* data = v.data_block();
  return vnl_vector<T >(data, v.size());
}
