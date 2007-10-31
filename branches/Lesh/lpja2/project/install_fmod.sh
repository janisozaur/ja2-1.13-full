#!/bin/sh

# Revised by JAPH, 18 september, 2007
# Revised by Arikon, 19 september, 2007

# This script will install the fmod library to your system.
# It will check that it was started with sudo, and if not,
# it will exit with instructions to run under sudo.

if [ $USER = "root" ]
then 
    echo "Running as root ..."
    echo "That's dangerous, but I'll assume you know what you're doing."
elif [ $USERNAME = "root" ]
then 
    echo "Running as root ..."
    echo "That's dangerous, but I'll assume you know what you're doing."
elif [ "x$SUDO_USER" = "x" ]
then
    echo "You must run this script under sudo!"
    echo "Try 'sudo sh install_fmod.sh'"
    exit 1
fi

if [ -f /usr/lib/libfmod-3.75.so ]
then
    echo "FMOD sound library version 3.75 already installed, exiting"
    exit 0
fi

echo "Installing FMOD sound library to /usr/lib..."

cp ../lib/libfmod-3.75.so /usr/lib
chmod 0644 /usr/lib/libfmod-3.75.so
ln -s /usr/lib/libfmod-3.75.so /usr/lib/libfmod.so
ldconfig

echo "All done!"
