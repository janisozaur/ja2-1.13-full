#!/bin/sh

input_files="../src/*.* ../src/sgp/*.* ../src/utils/*.* ../src/tile_engine/*.* ../src/editor/*.* ../src/laptop/*.* \
    ../src/strategic/*.* ../src/tactical/*.* ../src/tactical_ai/*.*"

echo Replacing $1 with $2...

sed -i "s/\"$1\"/\"$2\"/g" $input_files
