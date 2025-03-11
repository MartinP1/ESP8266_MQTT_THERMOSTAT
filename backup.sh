#!/bin/bash

#  create backup subdirectory
echo "create backup subdirectory" $1

mkdir "./backup/"$1"/"

mv *.bin "./backup/"$1"/"
