#
# This open cl setup is based on the requirements for the AMD SDK.
# http://developer.amd.com/gpu/ATIStreamSDKBetaProgram/Pages/default.aspx
# Undoubtedly a different setup is required for the Nvidia SDK
#
# 3/14/2009 Support for both ATI and NVIDIA SDKs added by Octavian
# 4/06/2009 Force check of actual lib files in the path's provided using FIND_LIBRARY added by Octavian

SET(OPENCL_FOUND "NO")
SET(NVIDIA_FOUND "NO")
SET(ATI_FOUND "NO")


IF (WIN32)
  FIND_PATH( OPENCL_INCLUDE_PATH CL/cl_gl.h)

  IF (OPENCL_INCLUDE_PATH)

   FIND_PATH( OPENCL_NVIDIA_LIBRARY_PATH OpenCL.lib )
   FIND_PATH( OPENCL_ATI_LIBRARY_PATH aticalcl.lib )

  IF (OPENCL_NVIDIA_LIBRARY_PATH)

     FIND_LIBRARY(OPENCL_NVIDIA_LIBRARY NAMES OpenCL.lib PATHS ${OPENCL_NVIDIA_LIBRARY_PATH} ${OPENCL_NVIDIA_LIBRARY_PATH}/Win32)
     IF (OPENCL_NVIDIA_LIBRARY)
      SET(OPENCL_FOUND "YES")
      SET(NVIDIA_FOUND "YES")
      SET( OPENCL_LIBRARIES ${OPENCL_NVIDIA_LIBRARY})

      ENDIF (OPENCL_NVIDIA_LIBRARY)
   ENDIF (OPENCL_NVIDIA_LIBRARY_PATH)

  IF (OPENCL_ATI_LIBRARY_PATH)
     SET(OPENCL_CALC_LIBRARY "${OPENCL_ATI_LIBRARY_PATH}/aticalcl.lib")
     SET(OPENCL_ATICALRT_LIBRARY "${OPENCL_ATI_LIBRARY_PATH}/aticalrt.lib")
     SET(OPENCL_ATI_LIBRARY "${OPENCL_ATI_LIBRARY_PATH}/x86/OpenCL.lib")

     #IF (OPENCL_CALC_LIBRARY AND  OPENCL_ATICALRT_LIBRARY AND OPENCL_ATI_LIBRARY)
      SET(OPENCL_LIBRARIES ${OPENCL_CALC_LIBRARY}  ${OPENCL_ATICALRT_LIBRARY} ${OPENCL_ATI_LIBRARY})
      IF (OPENCL_LIBRARIES)
        SET(OPENCL_FOUND "YES")
        SET(ATI_FOUND "YES")
        ENDIF (OPENCL_LIBRARIES)
   ENDIF (OPENCL_ATI_LIBRARY_PATH)

   IF (ATI_FOUND AND NVIDIA_FOUND)

    OPTION (USE_NVIDIA_SDK "Default to NVIDIA SDK?" YES)

    IF (USE_NVIDIA_SDK)
       SET(OPENCL_FOUND "YES")
       SET( OPENCL_LIBRARIES OPENCL_NVIDIA_LIBRARY)
    ENDIF (USE_NVIDIA_SDK)

    IF (NOT USE_NVIDIA_SDK)
       SET(OPENCL_FOUND "YES")
       SET(OPENCL_LIBRARIES OPENCL_CALC_LIBRARY OPENCL_AMUABI_LIBRARY OPENCL_ATICALRT_LIBRARY OPENCL_ATI_LIBRARY)
    ENDIF (NOT USE_NVIDIA_SDK)

   ENDIF (ATI_FOUND AND NVIDIA_FOUND)

  ENDIF (OPENCL_INCLUDE_PATH)
ENDIF (WIN32)

IF (APPLE OR UNIX)
  FIND_PATH(OPENCL_INCLUDE_PATH CL/cl.h "Include for OpenCL on OSX")
  IF (OPENCL_INCLUDE_PATH)
    FIND_LIBRARY(OPENCL_LIBRARY OpenCL "OpenCL lib for OSX")
    IF (OPENCL_LIBRARY)
      SET(OPENCL_FOUND "YES")
      SET(OPENCL_LIBRARIES ${OPENCL_LIBRARY})
    ENDIF (OPENCL_LIBRARY)
  ENDIF (OPENCL_INCLUDE_PATH)
ENDIF (APPLE OR UNIX)

