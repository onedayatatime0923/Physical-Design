#!/bin/bash

BASEDIR=$(dirname "$0")
EXE=$BASEDIR/tool/eval.pl


INPUTDIR=$BASEDIR/input
INPUT=$1
case "$1" in
    "example") 
        INPUT=$INPUTDIR/example.gr;;
    "a1") 
        INPUT=$INPUTDIR/adaptec1.capo70.2d.35.50.90.gr;;
    "a2") 
        INPUT=$INPUTDIR/adaptec2.mpl60.2d.35.20.100.gr;;
    "a3") 
        INPUT=$INPUTDIR/adaptec3.dragon70.2d.30.50.90.gr;;
    "a4") 
        INPUT=$INPUTDIR/adaptec4.aplace60.2d.30.50.90.gr;;
    "a5") 
        INPUT=$INPUTDIR/adaptec5.mfar50.2d.50.20.100.gr;;
    "n1") 
        INPUT=$INPUTDIR/newblue1.ntup50.2d.30.50.90.gr;;
    "n2") 
        INPUT=$INPUTDIR/newblue2.fastplace90.2d.50.20.100.gr;;
    "n3") 
        INPUT=$INPUTDIR/newblue3.kraftwerk80.2d.40.50.90.gr;;
esac

OUTPUTDIR=$BASEDIR/output
mkdir -p $OUTPUTDIR
OUTPUT=$OUTPUTDIR/$1.out

time ($EXE $INPUT $OUTPUT)
#gdb --args $EXE $INPUT $OUTPUT

