# Macros defined here are
#  MARK_AS_INTERNAL
#  LIST_CONTAINS
#  PARSE_ARGUMENTS


# MARK_AS_INTERNAL macro
# - Macro to hide a variable from the GUI
#

MACRO (MARK_AS_INTERNAL _var)
  SET (${_var} ${${_var}} CACHE INTERNAL "hide this!" FORCE)
ENDMACRO (MARK_AS_INTERNAL _var)


# LIST_CONTAINS macro
# - This macro checks if a list contains a given value
#
# The variable given as first argument will be set to either TRUE or FALSE.
#
# Usage:
#  LIST_CONTAINS(value ${list})
# value will be set to either TRUE or FALSE

MACRO  (LIST_CONTAINS var value)
  SET (${var})
  FOREACH (value2 ${ARGN})
    IF (${value} STREQUAL ${value2})
      SET (${var} TRUE)
    ENDIF (${value} STREQUAL ${value2})
  ENDFOREACH (value2)
ENDMACRO (LIST_CONTAINS)


# PARSE_ARGUMENTS macro
# - Macro to parse arguments given to macros
#
# This macro is copied from the one in http://www.cmake.org/Wiki/CMake_User_Contributed_Macros
#
# Usage examples:
#  PARSE_ARGUMENTS( ${prefix} ${arg_names} ${option_names})
#  PARSE_ARGUMENTS(  _PREFIX "A;B" "C" ${ARGN})
# will parse ${ARGN} and define the variables: _PREFIX_A, _PREFIX_B. It will set _PREFIX_C to either TRUE or FALSE.

MACRO (PARSE_ARGUMENTS prefix arg_names option_names)
  SET (DEFAULT_ARGS)
  FOREACH (arg_name ${arg_names})
    SET (${prefix}_${arg_name})
  ENDFOREACH (arg_name)
  FOREACH (option ${option_names})
    SET (${prefix}_${option} FALSE)
  ENDFOREACH (option)

  SET (current_arg_name DEFAULT_ARGS)
  SET (current_arg_list)
  FOREACH (arg ${ARGN})
    LIST_CONTAINS (is_arg_name ${arg} ${arg_names})
    IF (is_arg_name)
      SET (${prefix}_${current_arg_name} ${current_arg_list})
      SET (current_arg_name ${arg})
      SET (current_arg_list)
    ELSE (is_arg_name)
      LIST_CONTAINS (is_option ${arg} ${option_names})
      IF (is_option)
	SET (${prefix}_${arg} TRUE)
      ELSE (is_option)
	SET (current_arg_list ${current_arg_list} ${arg})
      ENDIF (is_option)
    ENDIF (is_arg_name)
  ENDFOREACH (arg)
  SET (${prefix}_${current_arg_name} ${current_arg_list})
ENDMACRO (PARSE_ARGUMENTS)
