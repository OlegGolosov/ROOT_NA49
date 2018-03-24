#!/bin/sh
###############################################################################

###
# If requested, use the NoDS branch
###
if [ ! -z "$T61NODS" ]; then
        T61NODS="noDS"
fi

###
# Define the official T61 analysis setting
###
T61=${T61:="$NA49/$NA49_LEVEL/Reconstruction/T61$T61NODS"}

###
# For the .root61dsts database file
###
ROOT61=${ROOT61:="/afs/cern.ch/na61/Data"}

###
PATH="$T61/bin:$PATH"
LD_LIBRARY_PATH="$T61/T61DST/lib:$T61/T61ANA/lib:$T61/RootDS/lib:$LD_LIBRARY_PATH"

###
export T61 ROOT61 PATH LD_LIBRARY_PATH

###############################################################################
