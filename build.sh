#!/bin/sh
set -xe

SOURCES=""
EXEC="ilu"

for f in src/* 
do
    SOURCES="$SOURCES $f"
done

g++ $SOURCES -o $EXEC
