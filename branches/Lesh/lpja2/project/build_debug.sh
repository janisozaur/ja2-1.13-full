#!/bin/sh

mkdir -p ../bin/debug
make  compile_flags="-O0 -g" \
	defines="-DJA2 -DENGLISH -DJA2EDITOR" \
	--directory=../bin/debug \
	--makefile=../../project/Makefile
