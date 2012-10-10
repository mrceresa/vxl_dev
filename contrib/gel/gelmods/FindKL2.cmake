#
# Find a KL library
#
# This file is used to manage using either a natively provided KL library or the one in v3p if provided.
#
#
# As per the standard scheme the following definitions are used
# KL_INCLUDE_DIR - where to find geotiff.h
# KL_LIBRARIES   - the set of libraries to include to use KL.
# KL_FOUND       - TRUE, if available somewhere on the system.

# Additionally
# VXL_USING_NATIVE_KL  - True if we are using a KL library provided outside vxl (or v3p)


# If this FORCE variable is unset or is FALSE, try to find a native library.
IF( VXL_FORCE_V3P_KL )
ELSE( VXL_FORCE_V3P_KL )
  INCLUDE( ${MODULE_PATH}/NewCMake/FindKL.cmake )
ENDIF( VXL_FORCE_V3P_KL )

  
IF(KL_FOUND)

  SET(VXL_USING_NATIVE_KL "YES")

ELSE(KL_FOUND)

  #
  # At some point, in a "release" version, it is possible that someone
  # will not have the v3p geotiff library, so make sure the headers
  # exist.
  #
  
  IF(EXISTS ${vxl_SOURCE_DIR}/contrib/gel/vgel/kl/klt.h)
    # Use FIND_PATH here to allow the user to set the path to IGNORE
    # to disable geotiff support.
    FIND_PATH(KL_INCLUDE_DIR klt.h
      ${vxl_SOURCE_DIR}/v3p/geotiff
    )
    IF( KL_INCLUDE_DIR )
      SET( KL_FOUND "YES" )
      SET( KL_INSTALL_INCLUDE_DIR ${CMAKE_INSTALL_DIR}/include/vxl/v3p/geotiff)
      SET( KL_LIBRARIES geotiff )
    ENDIF( KL_INCLUDE_DIR )
  
  ENDIF(EXISTS ${vxl_SOURCE_DIR}/v3p/geotiff/geotiff.h)
  
ENDIF(KL_FOUND)
