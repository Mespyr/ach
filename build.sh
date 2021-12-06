#!/bin/sh
set -x
./clean.sh

SOURCES=""
EXEC="ilu"

for f in src/* 
do
    SOURCES="$SOURCES $f"
done

g++ $SOURCES -o $EXEC
