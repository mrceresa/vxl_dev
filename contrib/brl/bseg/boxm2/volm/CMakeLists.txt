# contrib/brl/bseg/boxm2/volm/CMakeLists.txt
INCLUDE_DIRECTORIES( ${GEL_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( ${GEL_INCLUDE_DIR}/mrc )

INCLUDE_DIRECTORIES( ${BRL_INCLUDE_DIR}/bbas )
INCLUDE_DIRECTORIES( ${BRL_INCLUDE_DIR}/bseg )

SET(boxm2_volm_sources
    boxm2_volm_matcher.h   boxm2_volm_matcher.cxx
    boxm2_volm_wr3db_index.h  boxm2_volm_wr3db_index.cxx
    boxm2_volm_wr3db_index_sptr.h
    boxm2_volm_locations.h boxm2_volm_locations.cxx
    boxm2_volm_locations_sptr.h
   )

AUX_SOURCE_DIRECTORY(Templates boxm2_volm_sources)

ADD_LIBRARY(boxm2_volm  ${boxm2_volm_sources})
TARGET_LINK_LIBRARIES(boxm2_volm boxm2 boxm2_io brip baio vpgl volm vnl vidl vil_algo vil vgl_algo vgl_xio vgl vbl vul vsl vcl)

#install the .h .txx and libs
INSTALL_TARGETS(/lib boxm2_volm)
INSTALL_NOBASE_HEADER_FILES(/include/vxl/contrib/brl/bseg/boxm2_volm/ ${boxm2_volm_sources})

IF( BUILD_TESTING )
  SUBDIRS(tests)
ENDIF( BUILD_TESTING )

SUBDIRS(exe)

#pro
subdirs(pro)

