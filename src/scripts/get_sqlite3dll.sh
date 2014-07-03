#!/bin/sh

libversion=`wget -q -O- http://www.sqlite.org/download.html | grep sqlitedll-3 | grep zip | cut -f2 -d'"'`
wget http://www.sqlite.org/$libversion
unzip $libversion
rm $libversion

