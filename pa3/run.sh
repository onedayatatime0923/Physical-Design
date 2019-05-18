#!/bin/bash

BASEDIR=$(dirname "$0")
EXE=$BASEDIR/bstar

APHA=0.1

for var in "$@"
do
    FILE=$(basename $var .block)
    INPUTBLOCK=$var
    INPUTNET=$BASEDIR/input/$FILE.nets

    OUTPUTDIR=$BASEDIR/output
    mkdir -p $OUTPUTDIR
    OUTPUT=$OUTPUTDIR/$FILE.rpt

    echo -e "\e[31m"$var"\e[0m"
    time ($EXE $APHA $INPUTBLOCK $INPUTNET $OUTPUT)
    #gdb --args $EXE $INPUT $OUTPUT
done
