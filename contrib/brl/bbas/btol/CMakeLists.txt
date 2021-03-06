# segmentation detection library
# brl/bbas/btol/CMakeLists.txt

doxygen_add_library(contrib/brl/bbas/btol
  DEPENDS contrib/brl/bbas/bsol contrib/gel/vtol core/vbl
  PACKAGE contrib/brl
  )

INCLUDE_DIRECTORIES(${BRL_INCLUDE_DIR} )
INCLUDE_DIRECTORIES(${BRL_INCLUDE_DIR}/bbas )
INCLUDE_DIRECTORIES(${GEL_INCLUDE_DIR} )

SET(btol_sources
    dll.h

    btol_edge_algs.cxx     btol_edge_algs.h
    btol_vertex_algs.cxx   btol_vertex_algs.h
    btol_face_algs.cxx     btol_face_algs.h
)

ADD_LIBRARY(btol ${btol_sources})
SET_TARGET_PROPERTIES(btol PROPERTIES ${VXL_LIBRARY_PROPERTIES})
INSTALL_TARGETS(/lib btol)
INSTALL_NOBASE_HEADER_FILES(/include/vxl/contrib/btol ${btol_sources})

TARGET_LINK_LIBRARIES(btol bsol vtol vdgl vsol vgl vnl)

IF (BUILD_TESTING)
  SUBDIRS(tests)
ENDIF (BUILD_TESTING)
