# vxl/vnl/xio/CMakeLists.txt

SET(vnl_xio_sources
  vnl_xio_matrix_fixed.txx   vnl_xio_matrix_fixed.h
  vnl_xio_matrix.txx         vnl_xio_matrix.h
  vnl_xio_vector_fixed.txx   vnl_xio_vector_fixed.h
  vnl_xio_vector.txx         vnl_xio_vector.h
  vnl_xio_quaternion.txx     vnl_xio_quaternion.h
)

AUX_SOURCE_DIRECTORY(Templates vnl_xio_sources)

ADD_LIBRARY(vnl_xio ${vnl_xio_sources} )
SET_TARGET_PROPERTIES(vnl_xio PROPERTIES ${VXL_LIBRARY_PROPERTIES})
TARGET_LINK_LIBRARIES( vnl_xio vnl vsl )

INSTALL_TARGETS(/lib vnl_xio)
INSTALL_NOBASE_HEADER_FILES(/include/vxl/core/vnl/xio ${vnl_xio_sources})

IF(BUILD_TESTING)
  SUBDIRS(tests)
ENDIF(BUILD_TESTING)
