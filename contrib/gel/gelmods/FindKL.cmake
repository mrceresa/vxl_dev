# - Find KL library
# Find the native KL includes and library
# This module defines
#  KL_INCLUDE_DIR, where to find klt.h, etc.
#  KL_LIBRARIES, libraries to link against to use KL.
#  KL_FOUND, If false, do not try to use KL.
# also defined, but not for general use are
#  KL_LIBRARY, where to find the KL library.

FIND_PATH( KL_INCLUDE_DIR klt.h
  /usr/include
  /usr/include/klt
  /usr/local/include
)

SET( KL_NAMES ${KL_NAMES} klt )
FIND_LIBRARY( KL_LIBRARY
  NAMES ${KL_NAMES}
  PATHS /usr/lib
    /usr/lib64
    /usr/local/lib
)

SET( KL_FOUND "NO" )
IF( KL_INCLUDE_DIR )
    IF( KL_LIBRARY )
        SET( KL_FOUND "YES" )
        SET( KL_LIBRARIES ${KL_LIBRARY} )
    ENDIF( KL_LIBRARY )
ENDIF( KL_INCLUDE_DIR )
