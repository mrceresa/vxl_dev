#include <vcl_vector.txx>

#if defined(NO_STD_BOOL) || defined(VCL_SUNPRO_CC_50) || defined(VCL_GCC_295) || defined(GNU_LIBSTDCXX_V3) || defined(VCL_EGCS)
// SunPro 5.0 provides vector<bool> as a specialization of vector<T>.
// Ditto for GNU lib v3.
namespace {
  void tic(int nargs) {
    vcl_vector<bool> done_once(nargs, false);
  }
}
# ifdef VCL_GCC_295
#include <bvector.h>
template void fill<_Bit_iterator, bool>(_Bit_iterator, _Bit_iterator, bool const &);
template _Bit_iterator fill_n<_Bit_iterator, unsigned int, bool>(_Bit_iterator, unsigned int, bool const &);
# endif
# ifdef GNU_LIBSTDCXX_V3
namespace std {
  template void std::fill<std::_Bit_iterator, bool>(std::_Bit_iterator, std::_Bit_iterator, bool const &);
  template std::_Bit_iterator std::fill_n<std::_Bit_iterator, unsigned int, bool>(std::_Bit_iterator, unsigned int, bool const &);
}
namespace {
  void tickler(std::vector<bool> &uctive) {
    uctive.push_back(true);
  }
}
# endif
#else
VCL_VECTOR_INSTANTIATE(bool);
#endif
