export NA49_ARCH=Linux
absname=`readlink -f $BASH_SOURCE`
export T49=`dirname -- $absname`
export T49DST=$T49/T49DST
export T49DST_LIB=$T49/build/T49DST
export T49ANA=$T49/T49ANA
export T49ANA_LIB=$T49/build/T49ANA
export NEW_PATH=$T49DST_LIB:$T49ANA_LIB
export LD_LIBRARY_PATH=$NEW_PATH:$LD_LIBRARY_PATH
export NEW_PATH=$T49/build/src
export PATH=$NEW_PATH:$PATH

