#!/bin/bash
# build.sh
# build the test files for valgrind testing
# 0xca7

set -xe

CC=gcc
CFLAGS="-Wall -pedantic"

LIBPATH=../../cydonia

mkdir out

${CC} ${CFLAGS} -o out/test_file_reader test_file_reader.c $LIBPATH/cydonia.c -I$LIBPATH
valgrind ./out/test_file_reader

