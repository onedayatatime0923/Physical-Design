#!/bin/bash

BASEDIR=$(dirname "$0")
EXE=$BASEDIR/fm
INPUT=$BASEDIR/input/input_$1.dat

OUTPUTDIR=$BASEDIR/output
mkdir -p $OUTPUTDIR
OUTPUT=$OUTPUTDIR/output_$1.dat

time ($EXE $INPUT $OUTPUT)
#gdb --args $EXE $INPUT $OUTPUT
