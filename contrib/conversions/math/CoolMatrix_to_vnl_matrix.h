#include <cool/Matrix.h>
#include <vnl/vnl_matrix.h>

template <class T>
inline vnl_matrix<T > CoolMatrix_to_vnl_matrix(CoolMatrix<T > const& m)
{
  T const* data = m.data_block();
  return vnl_matrix<T >(data, m.rows(), m.columns());
}
