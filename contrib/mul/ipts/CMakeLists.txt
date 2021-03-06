# mul/ipts/CMakeLists.txt

doxygen_add_library(contrib/mul/ipts
  DEPENDS contrib/mul/vimt contrib/mul/mbl
  PACKAGE contrib/mul
  DESCRIPTION "Interesting Points - Find interesting/salient image points"
  )

SET(ipts_sources
    ipts_scale_space_peaks.h
    ipts_draw.h
    ipts_local_entropy.cxx ipts_local_entropy.h
    ipts_corner_pyramid.cxx ipts_corner_pyramid.h
    ipts_entropy_pyramid.cxx ipts_entropy_pyramid.h
    ipts_orientation_pyramid.cxx ipts_orientation_pyramid.h
)

ADD_LIBRARY(ipts ${ipts_sources})
SET_TARGET_PROPERTIES(ipts PROPERTIES ${VXL_LIBRARY_PROPERTIES})
INSTALL_TARGETS(/lib ipts)
INSTALL_NOBASE_HEADER_FILES(/include/vxl/contrib/mul/ipts ${ipts_sources})
TARGET_LINK_LIBRARIES(ipts vimt_algo vimt vgl vil_algo vil)

IF(BUILD_MUL_TOOLS)
  SUBDIRS(tools)
ENDIF(BUILD_MUL_TOOLS)


IF (BUILD_TESTING)
  SUBDIRS(tests)
ENDIF (BUILD_TESTING)
