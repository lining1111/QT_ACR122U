#!/bin/sh

#
# Copyright (C) 2011-2017 Advanced Card Systems Ltd. All Rights Reserved.
#
# This software is the confidential and proprietary information of Advanced
# Card Systems Ltd. ("Confidential Information").  You shall not disclose such
# Confidential Information and shall use it only in accordance with the terms
# of the license agreement you entered into with ACS.
#

#
# install.sh
# This script is to install the library.
#

USER=`whoami`
MACHINE=`uname -m`

if [ $USER != root ]; then
    echo "You must be root to run this script."
    exit 1
fi

echo "Installing libctacs..."

case $MACHINE in
    i?86)
        cp -av include/* /usr/include
        cp -av lib/i386/* /usr/lib
        ;;
    x86_64)
        cp -av include/* /usr/include
        cp -av lib/x86_64/* /usr/lib
        ;;
    *)
        echo "Error: The CPU architecture is unknown."
        exit 1
        ;;
esac

echo "Completed!"
