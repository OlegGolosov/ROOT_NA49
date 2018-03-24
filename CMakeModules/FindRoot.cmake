################################################################################
# Module to find Root                                                          #
#                                                                              #
# This sets the following variables:                                           #
#   - ROOT_FOUND                                                               #
#   - ROOT_LIBRARIES                                                           #
#   - ROOT_INCLUDE_DIR                                                         #
#   - ROOT_BIN_DIR                                                             #
#   - ROOTSYS                                                                  #
#   - ROOTCINT                                                                 #
#   - ROOT_CXXFLAGS                                                            #
#   - ROOT_AUXCFLAGS                                                           #
#   - ROOT_LDFLAGS                                                             #
#   - ROOT_VERSION                                                             #
#                                                                              #
# And these might be needed as well... we'll see:                              #
#   - ROOT_LIBS                                                                #
#   - ROOT_GLIBS                                                               #
#   - ROOT_LIBDIR                                                              #
################################################################################

include(LibFindMacros)

# the following disables all default paths (either from cmake, from environment)
FIND_PATH(ROOT_BIN_DIR root-config 
          PATHS $ENV{ROOTSYS}/bin
          NO_DEFAULT_PATH)

# now defaults are allowed but if nothing is found it is not overwritten
# (because it is cached)
FIND_PATH(ROOT_BIN_DIR root-config)

GET_FILENAME_COMPONENT(ROOTSYS ${ROOT_BIN_DIR} PATH)

IF(NOT ENV{ROOTSYS})
  SET (ENV{ROOTSYS} ${ROOTSYS})
ENDIF(NOT ENV{ROOTSYS})

IF(ROOTSYS)
  SET (ROOT_FOUND TRUE)
  SET (HAVE_ROOT 1)

  EXECUTE_PROCESS (COMMAND ${ROOT_BIN_DIR}/root-config --version
    OUTPUT_VARIABLE ROOT_VERSION
  )
  STRING (REGEX REPLACE "[ \t\r\n]+" "" ROOT_VERSION ${ROOT_VERSION})
  STRING (REGEX REPLACE "/" "." ROOT_VERSION ${ROOT_VERSION})

  EXECUTE_PROCESS (COMMAND ${ROOT_BIN_DIR}/root-config --ldflags
    OUTPUT_VARIABLE ROOT_LDFLAGS
  )
  STRING (REGEX REPLACE "[ \t\r\n]+" " " ROOT_LDFLAGS ${ROOT_LDFLAGS})

  EXECUTE_PROCESS (COMMAND ${ROOT_BIN_DIR}/root-config --libs
    OUTPUT_VARIABLE ROOT_LIBS
  )
  STRING (REGEX REPLACE "[\t\r\n]+" "" ROOT_LIBS ${ROOT_LIBS})

  EXECUTE_PROCESS (COMMAND ${ROOT_BIN_DIR}/root-config --glibs
    OUTPUT_VARIABLE ROOT_GLIBS
  )
  STRING (REGEX REPLACE "[\t\r\n]+" "" ROOT_GLIBS ${ROOT_GLIBS})

  EXECUTE_PROCESS (COMMAND ${ROOT_BIN_DIR}/root-config --libdir
    OUTPUT_VARIABLE _ROOT_LIBDIR
  )
  STRING (REGEX REPLACE "[ \t\r\n]+" "" ROOT_LIBDIR ${_ROOT_LIBDIR})

  EXECUTE_PROCESS (COMMAND ${ROOT_BIN_DIR}/root-config --incdir
    OUTPUT_VARIABLE ROOT_INCLUDE_DIR
  )
  STRING (REGEX REPLACE "[ \t\r\n]+" "" ROOT_INCLUDE_DIR ${ROOT_INCLUDE_DIR})

  EXECUTE_PROCESS (COMMAND ${ROOT_BIN_DIR}/root-config --cflags
    OUTPUT_VARIABLE ROOT_CXXFLAGS
  )
  STRING (REGEX REPLACE "[\t\r\n]+" "" ROOT_CXXFLAGS ${ROOT_CXXFLAGS})

  EXECUTE_PROCESS (COMMAND ${ROOT_BIN_DIR}/root-config --auxcflags
    OUTPUT_VARIABLE ROOT_AUXCFLAGS
  )
  STRING (REGEX REPLACE "[\t\r\n]+" "" ROOT_AUXCFLAGS ${ROOT_AUXCFLAGS})

  SET (ROOTCINT ${ROOT_BIN_DIR}/rootcint)

  find_library(ROOT_Minuit_LIBRARY NAMES Minuit PATHS ${ROOT_LIBDIR})

  STRING (REGEX REPLACE "-l" " " CRAP ${ROOT_LIBS})
  STRING (REGEX REPLACE "-" " " CRAP ${CRAP})
  SEPARATE_ARGUMENTS (CRAP)
  FOREACH (_file ${CRAP})
    FIND_LIBRARY (_LIBY${_file} ${_file} ${ROOT_LIBDIR} NO_DEFAULT_PATH)
    IF (_LIBY${_file})
      LIST (APPEND ROOT_LIBRARIES ${_LIBY${_file}})
    ENDIF (_LIBY${_file})
    MARK_AS_INTERNAL (_LIBY${_file})
  ENDFOREACH (_file)

ENDIF (ROOTSYS)

SET (_DIRECTORIES)

IF (NOT DL_LIBRARY)
  FIND_LIBRARY (DL_LIBRARY dl)
  MARK_AS_ADVANCED (DL_LIBRARY)
ENDIF (NOT DL_LIBRARY)
SET (ROOT_LIBRARIES ${ROOT_LIBRARIES} ${DL_LIBRARY})

IF (ROOT_FOUND)
  #add minuit
  IF (NOT Root_FIND_QUIETLY)	
    MESSAGE (STATUS "found Root: ${ROOTSYS}")
    #MESSAGE("             : ${ROOT_LIBRARIES}")
  ENDIF (NOT Root_FIND_QUIETLY)
ELSE (ROOT_FOUND)
  IF (Root_FIND_REQUIRED)
    MESSAGE (FATAL_ERROR "Could not find Root")
  ENDIF (Root_FIND_REQUIRED)
ENDIF (ROOT_FOUND)


###########################################
#
#       Macros for building ROOT dictionary
#
###########################################

# The following macro creates rules for a dictionary
# If the LinkDef file is called SomeLinkDef.h
# it will create the files SomeDictionary.cc and SomeDictionary.h
# it defines the variables $Some_DICT_DOURCE and $Some_DICT_HEADER
# $Some_INFILES $Some_INCLUDE_DIRS $Some_VERBOSE

# local variables: _LINKDEF _INCLUDE_DIRS _NAME _current_FILE

MACRO (ROOT_GENERATE_DICTIONARY _NAME)
  PARSE_ARGUMENTS (${_NAME}
    "LINKDEF;INFILES;INCLUDE_DIRS"
    "VERBOSE"
    ${ARGN}
  )
  SET (_INCLUDE_DIRS)
  SET (${_NAME}_DICT_SOURCE ${CMAKE_CURRENT_BINARY_DIR}/${_NAME}Dictionary.cc)
  SET (${_NAME}_DICT_HEADER ${CMAKE_CURRENT_BINARY_DIR}/${_NAME}Dictionary.h)

  FOREACH (_current_FILE ${${_NAME}_INCLUDE_DIRS})
    SET (_INCLUDE_DIRS ${_INCLUDE_DIRS} -I${_current_FILE})   
  ENDFOREACH (_current_FILE ${${_NAME}_INCLUDE_DIRS})

  IF (${_NAME}_VERBOSE)
    MESSAGE (STATUS "Root dictionary:\n   ${_NAME}_INFILES: ${${_NAME}_INFILES}")
    MESSAGE ("   OutFILEs: ${${_NAME}_DICT_SOURCE} ${${_NAME}_DICT_HEADER}")
    MESSAGE ("   LINKDEF: ${${_NAME}_LINKDEF}")
  ENDIF (${_NAME}_VERBOSE)

  ADD_CUSTOM_COMMAND (OUTPUT ${${_NAME}_DICT_SOURCE} ${${_NAME}_DICT_HEADER}
    COMMAND ${ROOTCINT} -f ${${_NAME}_DICT_SOURCE} -c ${_INCLUDE_DIRS} ${${_NAME}_INFILES} ${${_NAME}_LINKDEF}
    DEPENDS ${${_NAME}_INFILES}
  )

ENDMACRO (ROOT_GENERATE_DICTIONARY)
