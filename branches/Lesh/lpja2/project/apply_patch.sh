#!/bin/sh

patch --directory=/home/lesh/projects/lpja2 --input=$1 --posix -p0 --no-backup-if-mismatch $2
