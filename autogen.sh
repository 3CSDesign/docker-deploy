#!/bin/sh
# autogen.sh - script to bootstrap the build environment for an Autotools-based project

# Run necessary autotools commands
aclocal
autoheader
automake --add-missing --copy
autoconf
