################################################################################
# Module to find DSPACK                                                        #
#                                                                              #
# This sets the following variables:                                           #
#   - DSPACK_FOUND                                                             #
#   - DSPACK_LIBRARIES                                                         #
#   - DSPACK_INCLUDE_DIR                                                       #
#   - DSPACK_CXXFLAGS                                                          #
#                                                                              #
################################################################################

# the following disables all default paths (either from cmake, from environment)
FIND_PATH (DSPACK_ROOT inc/dspack.h
  PATHS /usr /usr/local /opt/local $ENV{NA49}/DSPACK $ENV{NA49}/DSPACK
  $ENV{NA49}/Reconstruction/DSPACK $ENV{NA49}/Reconstruction/DSPACK
  $ENV{DSPACK_HOME}
  NO_DEFAULT_PATH)

IF(DSPACK_ROOT)
  SET (DSPACK_FOUND TRUE)
  SET(DSPACK_INCLUDE_DIR ${DSPACK_ROOT}/inc)
  SET(DSPACK_CXXFLAGS -I${DSPACK_ROOT}/inc)
  FIND_LIBRARY (DSPACK_LIBRARY dspack ${DSPACK_ROOT}/lib NO_DEFAULT_PATH)
  IF(DSPACK_LIBRARY)
    MARK_AS_INTERNAL(DSPACK_LIBRARY)
  ELSE(DSPACK_LIBRARY)
    MESSAGE(STATUS dspack library not found)
  ENDIF(DSPACK_LIBRARY)
ELSE(DSPACK_ROOT)
  MESSAGE(STATUS dspack not found)
ENDIF(DSPACK_ROOT)



