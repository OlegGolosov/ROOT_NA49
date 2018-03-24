# - Try to find PGI Fortran
# Once done, this will define
#
#  PGIFortran_FOUND - system has PGIFortran software
#  PGIFortran_INCDIR - location of public PGIFortran header files
#  PGIFortran_LIBDIR - location of PGIFortran libraries
#  PGIFortran_LIBRARIES - libraries to link against
##  PGIFortran_pgc_LIBRARY - location and full name of libpgc
##  PGIFortran_pgftnrtl_LIBRARY - location and full name of libpgftnrtl

set(PGIFortran_DIR $ENV{PGI})
if(PGIFortran_DIR AND EXISTS ${PGIFortran_DIR})
  set(PGIFortran_FOUND TRUE)
endif(PGIFortran_DIR AND EXISTS ${PGIFortran_DIR})

if (PGIFortran_FOUND)
  set(PGIFortran_INCDIR ${PGIFortran_DIR}/include)
  set(PGIFortran_LIBDIR ${PGIFortran_DIR}/lib)
  find_library(PGIFortran_pgc_LIBRARY NAMES pgc PATHS ${PGIFortran_LIBDIR} NO_DEFAULT_PATH)
  find_library(PGIFortran_pgftnrtl_LIBRARY NAMES pgftnrtl PATHS ${PGIFortran_LIBDIR} NO_DEFAULT_PATH)
  mark_as_internal(${PGIFortran_pgc_LIBRARY})
  mark_as_internal(${PGIFortran_pgftnrtl_LIBRARY})
  set(PGIFortran_LIBRARIES ${PGIFortran_pgc_LIBRARY} ${PGIFortran_pgftnrtl_LIBRARY})
endif (PGIFortran_FOUND)


if(PGIFortran_FOUND)
  message(STATUS "PGI Fortran found in ${PGIFortran_DIR}")
else(PGIFortran_FOUND)
  message(STATUS "Couldn't locate PGI Fortran")
endif(PGIFortran_FOUND)
