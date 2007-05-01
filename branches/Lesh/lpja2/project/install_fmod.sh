#!/bin/sh

# This script will install fmod library to your system
# It will need root privilegies (via sudo)

echo Installing FMOD sound library to /usr/lib...

echo In order to install FMOD sound library to your system
echo you need enter a root password.

sudo cp ../lib/libfmod-3.75.so /usr/lib
sudo chmod 0644 /usr/lib/libfmod-3.75.so
sudo cp -s /usr/lib/libfmod-3.75.so /usr/lib/libfmod.so
