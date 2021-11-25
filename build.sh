#!/bin/sh

SOURCES=""
EXEC="ilu"

set -xe
for f in src/* 
do
    SOURCES="$SOURCES $f"
done

g++ $SOURCES -o $EXEC
