#!/bin/csh
###############################################################################

###
# If requested, use the NoDS branch
###
if ( $?T61NODS ) then
        set T61NODS = "noDS"
else
        set T61NODS = ""
endif

###
# Define the official T61 analysis setting
###
if ( ! $?T61 ) then
	setenv T61        "$NA49/$NA49_LEVEL/Reconstruction/T61$T61NODS"
endif

###
# For the .root61dsts database file
###
if ( ! $?ROOT61 ) then
	setenv ROOT61     "/afs/cern.ch/na61/Data"
endif

###
# define PATH and LD_LIBRARY_PATH
###
setenv PATH "$T61/bin":$PATH
setenv LD_LIBRARY_PATH "$T61/T61DST/lib":"$T61/T61ANA/lib":"$T61/RootDS/lib":"$LD_LIBRARY_PATH"

###############################################################################
