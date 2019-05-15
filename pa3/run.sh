#!/bin/bash

BASEDIR=$(dirname "$0")
EXE=$BASEDIR/bstar

APHA=0.5

for var in "$@"
do
    FILE=$(basename $var .block)
    INPUTBLOCK=$var
    INPUTNET=$BASEDIR/input/$FILE.nets

    OUTPUTDIR=$BASEDIR/output
    mkdir -p $OUTPUTDIR
    OUTPUT=$OUTPUTDIR/$FILE.out

    time ($EXE $APHA $INPUTBLOCK $INPUTNET $OUTPUT)
    #gdb --args $EXE $INPUT $OUTPUT
done
