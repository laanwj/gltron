#!/bin/sh

aclocal
autoconf -f
automake --foreign --add-missing

echo you are ready to run ./configure now
