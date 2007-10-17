# modified by Kongbin to add support for volume rendering

SET (HAS_BGUI3D "NO")
IF (BUILD_BGUI3D)

  # Find the Coin3D library
  INCLUDE( ${vxl_SOURCE_DIR}/contrib/brl/bmods/find_coin.cmake )
  
  IF (COIN_FOUND)
    INCLUDE_DIRECTORIES( ${COIN_INCLUDE_DIR} )
    SET( HAS_BGUI3D "YES" )
    ADD_DEFINITIONS( -DHAS_BGUI3D )
    LINK_LIBRARIES( ${COIN_LIBRARY} )
  ENDIF (COIN_FOUND)

  # Find the SimVoleon library
  INCLUDE( ${vxl_SOURCE_DIR}/contrib/brl/bmods/find_SIMVoleon.cmake )

  IF (SIMVOLEON_FOUND)
    INCLUDE_DIRECTORIES( ${SIMVOLEON_INCLUDE_DIR} )
    LINK_LIBRARIES ( ${SIMVOLEON_LIBRARY} )
  ENDIF (SIMVOLEON_FOUND)

ENDIF (BUILD_BGUI3D)
