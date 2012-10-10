# gel/vgel/kl/CMakeLists.txt

INCLUDE(${MODULE_PATH}/FindKL.cmake)

IF(NOT VXL_USING_NATIVE_KL)
    INCLUDE(${GEL_MODULE_PATH}/FindKL.cmake)

    INCLUDE_DIRECTORIES(${KL_INCLUDE_DIR})

    SET( vgel_kl_sources
                          base.h
     convolve.c           convolve.h
     klt.c                klt.h
     pnmio.c              pnmio.h
     selectGoodFeatures.c
     trackFeatures.c
     error.c              error.h
     klt_util.c           klt_util.h
     pyramid.c            pyramid.h
     storeFeatures.c
     writeFeatures.c
    )

    ADD_LIBRARY(vgel_kl ${vgel_kl_sources})
    INSTALL_TARGETS(/lib vgel_kl)
    INSTALL_NOBASE_HEADER_FILES(/include/vxl/contrib/gel/vgel/kl ${vgel_kl_sources})

    IF(UNIX)
      TARGET_LINK_LIBRARIES(vgel_kl m)
    ENDIF(UNIX)
ENDIF(NOT VXL_USING_NATIVE_KL)

IF( BUILD_TESTING )
  SUBDIRS(tests)
ENDIF( BUILD_TESTING )
