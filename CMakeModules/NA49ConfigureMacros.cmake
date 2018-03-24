# This file contains macros for doing 'configure-style' substitutions at cmake time and creating rules to do it at make time.
#
#  NA49_CREATE_GLOBAL_REVISION
#  NA49_CREATE_CONFIGURE
#  NA49_CONFIG_FILES
#  NA49_CREATE_CONFIG_RULE
#  NA49_CREATE_CONFIG_RULES


# NA49_CREATE_CONFIG_RULE
# - Macro to create a make rule for configuring a single file
#
# This is the macro that creates the rule so a file is configured when make is run.
# If the OUT parameter is not given then it expects the file name to end in '.in' and 
# the new file will have the same name without the '.in' extension (foo.in -> foo).
# If the OUT parameter is present, then its value will be taken as the name of the new file.
# VERBOSE does what you expect (hopefully)
#
# The idea of the TARGET parameter is that we can specify a TARGET that depends on this file but
# now that I write this I see it is not used.
#
# Usage example:
#  NA49_CREATE_CONFIG_RULE(${FILE} [OUT ${OUT_FILE}] [TARGET ${TARGET}] [VERBOSE])

MACRO (NA49_CREATE_CONFIG_RULE )
  PARSE_ARGUMENTS (_NA49_CONFIG_RULE
    "OUT;TARGET"
    "VERBOSE"
    ${ARGN}
  )
  IF (_NA49_CONFIG_RULE_DEFAULT_ARGS)
    SET (_relative_in_file ${_NA49_CONFIG_RULE_DEFAULT_ARGS})
  ELSE (_NA49_CONFIG_RULE_DEFAULT_ARGS)
    MESSAGE (STATUS "No file name provided")
  ENDIF (_NA49_CONFIG_RULE_DEFAULT_ARGS)

  SET (_out_file ${_NA49_CONFIG_RULE_OUT})

  GET_FILENAME_COMPONENT (_in_file ${_relative_in_file} ABSOLUTE)
  IF (NOT _NA49_CONFIG_RULE_OUT)
    STRING (REGEX REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}" _out_file ${_in_file})
    STRING (REGEX REPLACE "\\.in" "" _out_file ${_out_file})
  ENDIF (NOT _NA49_CONFIG_RULE_OUT)

  GET_FILENAME_COMPONENT (_out_file_dir ${_out_file} PATH) 

  IF (_NA49_CONFIG_RULE_VERBOSE)
    MESSAGE("${NA49_CONFIGURE} ${_in_file} -> ${_out_file}")
  ENDIF (_NA49_CONFIG_RULE_VERBOSE)

  LIST (APPEND NA49_CONFIGURE_FILE_LIST  ${_in_file})
  STRING (REGEX REPLACE "/" "_" _target_name ${_out_file})
  ADD_CUSTOM_TARGET (${_target_name} ALL DEPENDS ${_out_file})
  LIST (APPEND NA49_CONFIGURE_TARGET_LIST  ${_target_name})
  ADD_CUSTOM_COMMAND (OUTPUT ${_out_file}
    COMMAND ${NA49_CONFIGURE} ${_in_file} ${_out_file} ${_out_file_dir}
    DEPENDS ${_in_file}
  )

ENDMACRO (NA49_CREATE_CONFIG_RULE)



# NA49_CREATE_CONFIG_RULES
# - Macro to create a make rule for configuring a list of files
#
# This macro just calls NA49_CREATE_CONFIG_RULE on a list of files.
#
# Usage example:
#  NA49_CREATE_CONFIG_RULES(${FILES} [TARGET ${TARGET}] [VERBOSE])

MACRO (NA49_CREATE_CONFIG_RULES)
  PARSE_ARGUMENTS (_NA49_CONFIG_RULES
    "TARGET"
    "VERBOSE"
    ${ARGN}
  )
  SET (_arguments)
  IF (_NA49_CONFIG_RULES_VERBOSE)
    LIST (APPEND _arguments VERBOSE)
  ENDIF (_NA49_CONFIG_RULES_VERBOSE)
  IF (_NA49_CONFIG_RULES_TARGET)
    LIST (APPEND _arguments TARGET ${_NA49_CONFIG_RULES_TARGET})
  ENDIF (_NA49_CONFIG_RULES_TARGET)

  FOREACH (_in_file ${_NA49_CONFIG_RULES_DEFAULT_ARGS})
    NA49_CREATE_CONFIG_RULE (${_in_file} ${_arguments})
  ENDFOREACH (_in_file)
ENDMACRO (NA49_CREATE_CONFIG_RULES)
