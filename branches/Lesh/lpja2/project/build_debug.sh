#!/bin/sh

mkdir -p ../bin/debug
make  compile_flags="-O0 -g" \
	defines="-DJA2 -DENGLISH" \
	--directory=../bin/debug \
	--makefile=../../project/Makefile
