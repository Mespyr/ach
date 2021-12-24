#!/bin/sh
./clean.sh

SOURCES=""
EXEC="ilu"

for f in src/* 
do
    SOURCES="$SOURCES $f"
done

set -xe
g++ $SOURCES -o $EXEC $@
