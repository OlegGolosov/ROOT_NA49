# - Try to find NA49 software
# Once done, this will define
#
#  NA49_FOUND - system has NA49 software
#  NA49_INCDIR - location of public NA49 header files
#  NA49_LIBDIR - location of NA49 libraries
#  NA49_hepdb_LIBRARY - location and full name of libdb
#  NA49_padtrans_LIBRARY - location and full name of libpadtrans
#  NA49_trkstep_LIBRARY - location and full name of libtrk
#  NA49_unpack_mbrd_LIBRARY - location and full name of libunpack_mbrd

macro(is_undefined_symbol_in_library _returnVar)
  parse_arguments(isinlib
    "LIBRARY;SYMBOL"
    ""
    ${ARGN}
  )
  execute_process(COMMAND nm ${isinlib_LIBRARY}
                  COMMAND grep -q "U ${isinlib_SYMBOL}"
                  RESULT_VARIABLE _grepResult)
  if(${_grepResult} EQUAL 0)
    set(${_returnVar} TRUE)
  else(${_grepResult} EQUAL 0)
    set(${_returnVar} FALSE)
  endif(${_grepResult} EQUAL 0)
endmacro(is_undefined_symbol_in_library)


set(NA49_DIR $ENV{NA49_ROOT})
if(NA49_DIR AND EXISTS ${NA49_DIR})
  set(NA49_FOUND TRUE)
endif(NA49_DIR AND EXISTS ${NA49_DIR})

if(NA49_FOUND)
  message(STATUS "NA49 software-chain location: ${NA49_DIR}")
else(NA49_FOUND)
  message(STATUS "Couldn't locate NA49 software chain")
endif(NA49_FOUND)

if (NA49_FOUND)
  find_package(DSPACK)
  find_package(PGIFortran)
  set(NA49_INCDIR ${NA49_DIR}/inc)
  set(NA49_unpack_mbrd_INCDIR ${NA49_DIR}/Utilities/UnpackMBRD)
  set(NA49_LIBDIR ${NA49_DIR}/lib)
  find_library(NA49_hepdb_LIBRARY NAMES db PATHS ${NA49_LIBDIR} ${NA49_DIR}/Reconstruction/HEPDB/lib NO_DEFAULT_PATH)
  find_library(NA49_padtrans_LIBRARY NAMES padtrans PATHS ${NA49_LIBDIR} ${NA49_DIR}/Reconstruction/PADTRANS/lib NO_DEFAULT_PATH)
  find_library(NA49_trkstep_LIBRARY NAMES trk PATHS ${NA49_LIBDIR} ${NA49_DIR}/Reconstruction/TRKSTEP/lib NO_DEFAULT_PATH)
  find_library(NA49_unpack_mbrd_LIBRARY NAMES unpack_mbrd PATHS ${NA49_LIBDIR} ${NA49_DIR}/Utilities/UnpackMBRD/Linux NO_DEFAULT_PATH)
  is_undefined_symbol_in_library(_hepdb_needs_pgf LIBRARY ${NA49_hepdb_LIBRARY} SYMBOL "ftn_strcmp")
  is_undefined_symbol_in_library(_trkstep_needs_pgf LIBRARY ${NA49_trkstep_LIBRARY} SYMBOL "ftn_i_sign")
  # Define link dependencies for braindead/static libraries. Warning: watch the order!!!
  list(APPEND NA49_hepdb_LIBRARY
    ${DSPACK_LIBRARY}
  )
  if(_hepdb_needs_pgf OR _trkstep_needs_pgf)
    message(STATUS "Adding PGF libs to link dependencies")
    if(_hepdb_needs_pgf)
      list(APPEND NA49_hepdb_LIBRARY ${PGIFortran_LIBRARIES})
    endif(_hepdb_needs_pgf)
    if(_trkstep_needs_pgf)
      list(APPEND NA49_trkstep_LIBRARY ${PGIFortran_LIBRARIES})
    endif(_trkstep_needs_pgf)
  endif(_hepdb_needs_pgf OR _trkstep_needs_pgf)
endif (NA49_FOUND)
