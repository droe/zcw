#!/bin/sh

# This script will update (or no) the cctld and gtld includes

#   Copyright (c) 2003-2008 by Philippe Bourcier
#   
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
#

if [ ! -f `which wget` ]; then
echo " You need wget, to download the TLD lists."
echo
exit
fi

#
# This part updates cctld.hpp
#

INCPATH="./src/includes/"

cat << EOF > $INCPATH"cctld.hpp.new"

/*

 includes/cctld.hpp
   
   Copyright (c) 2003-2008 by Philippe Bourcier
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

*/

string get_countryname(string tld) {
tld=php_strtoupper(tld);
EOF

wget -qO - http://www.iana.org/domains/root/db/index.html | grep country-code | grep -v such.as | cut -f3- -d"." | sed 's/<\/a><\/td><td>country-code<\/td><td>/|/g' | cut -f-1 -d"<" | cut -f-1 -d"(" | while read line; do

country=`echo $line | cut -f2 -d"|"`
comma=`echo $country | cut -f2 -d","`
cctld=`echo $line | cut -f1 -d"|"`

if [ "x$comma" = "x$country" ]; then
	echo 'if(tld=="'$cctld'") return "'$country'";' >> $INCPATH"cctld.hpp.new"
else
	zx=`echo $country | cut -f1 -d","`
	echo 'if(tld=="'$cctld'") return "'`echo $comma` $zx'";' >> $INCPATH"cctld.hpp.new"
fi

done

cat << EOF >> $INCPATH"cctld.hpp.new"
return "Not Found";
}

EOF

#
# This part updates gtld.hpp
#

cat << EOF > $INCPATH"gtld.hpp.new"

/*

 includes/gtld.hpp
   
   Copyright (c) 2003-2008 by Philippe Bourcier
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

*/

bool is_gtld(string str) {
EOF

live=`wget -qO - http://www.iana.org/domains/root/db/index.html | egrep "sponsored|generic" | grep -v such.as | cut -f3- -d"." | cut -f-1 -d"<" | cut -f-1 -d"(" | xargs | sed 's/ /|/g'`

gtld='string needle="'$live'";'

cat << EOF >> $INCPATH"gtld.hpp.new"
$gtld
int max=php_substr_count(needle,"|")+1;
string tmp;
        for(int i=0;i<max;i++) {
                tmp=php_strtok(needle,"|");
                if(str==tmp) return true;
        }
return false;
}

EOF

oldg=`wc -c $INCPATH"gtld.hpp"      | awk '{ print $1 }'`
oldc=`wc -c $INCPATH"cctld.hpp"     | awk '{ print $1 }'`
newg=`wc -c $INCPATH"gtld.hpp.new"  | awk '{ print $1 }'`
newc=`wc -c $INCPATH"cctld.hpp.new" | awk '{ print $1 }'`

mode=""
if [ $oldg != $newg ]; then
	cat $INCPATH"gtld.hpp.new" > $INCPATH"gtld.hpp"
	mode="ok"
fi
if [ $oldc != $newc ]; then
	cat $INCPATH"cctld.hpp.new" > $INCPATH"cctld.hpp"
	mode="ok"
fi
if [ "x$mode" = "xok" ]; then
	echo " CCTLD/GTLD update done"
else
	echo " No update needed"
fi

rm -f $INCPATH"gtld.hpp.new"
rm -f $INCPATH"cctld.hpp.new"

# EOF
