#!/bin/sh

# This shell script will build and patch tcpserver

#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 2, or (at your option)
#   any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program; if not, write to the Free Software Foundation,
#   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

echo

tcpserverfile=`which tcpserver 2>&1 | cut -f1 -d" " | grep "/"`

if [ "x$tcpserverfile" != "x" ]; then
echo "It looks like tcpserver has already been installed."
echo
exit 0
fi

if [ ! -f `which wget` ]; then
echo " You need wget, to download the UCSPI-TCP archive."
echo
exit
fi

echo " Getting the UCSPI-TCP archive... (this can take some time)"
echo
wget -q http://cr.yp.to/ucspi-tcp/`wget -qO - http://cr.yp.to/ucspi-tcp/install.html | grep tar.gz | cut -f2 -d'"'`
tar -zxf ucspi-tcp-0.88.tar.gz
cd ucspi-tcp-0.88
echo " Building TCPSERVER (...and this too)"
wget -q http://www.wolfermann.org/tcpserver.patch
tail -`wc -l tcpserver.patch | awk '{ print $1" - 4" }' | bc` tcpserver.patch > tmp && cat tmp > tcpserver.patch
patch -p0 < tcpserver.patch >/dev/null 2>&1
sed 's/gcc /gcc -Wall /g' conf-ld > tmp && cat tmp > conf-ld
sed 's/gcc /gcc -Wall /g' conf-cc > tmp && cat tmp > conf-cc
head -4 error.h > tmp
echo "#include <errno.h>" >> tmp
tail -n +5 error.h >> tmp
cat tmp > error.h
make 1>debug.out 2>&1
if [ ! -f tcpserver ]; then
echo && cat debug.out
exit
fi
cp `/bin/ls -F | grep tcpserver | grep "*" | cut -f1 -d"*"` ../
cd ..
rm -rf ucspi-tcp-0.88
rm ucspi-tcp-0.88.tar.gz
echo
echo " It's all done..."
echo " You now have ./tcpserver"
echo

