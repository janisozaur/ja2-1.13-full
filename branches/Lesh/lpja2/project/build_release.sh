#!/bin/sh

mkdir -p ../bin/release
make compile_flags="-O3 -funroll-loops -fomit-frame-pointer -g" \
	defines="-DJA2 -DENGLISH" \
	--directory=../bin/release \
	--makefile=../../project/Makefile

