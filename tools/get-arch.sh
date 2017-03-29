#!/bin/bash

if [[ $1 != *"-"* ]];
then
    echo $1
else
    STR="$1"
    LEN="`expr index $STR -`"
    RES=`expr ${STR:0:$LEN-1}`
    echo $RES
fi
