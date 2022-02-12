#!/bin/sh
./clean.sh

SOURCES=""
EXEC="ach"

for f in src/* 
do
	SOURCES="$SOURCES $f"
done

set -xe
g++ $SOURCES -o $EXEC -Wall -Wextra -pedantic $@
