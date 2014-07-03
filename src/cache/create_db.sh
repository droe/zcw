#!/bin/sh

# This script will create the ARIN_cache.db database

# filename : ARIN_cache.db
# tablename : ARIN_cache
# fields : IR / start / stop

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

WINDOWS=`echo ${TERM} | grep cygwin | sed 's/cygwin/.exe/g'`

nwhich=`echo $zwhich | wc -l | awk '{ print $0 }'`

if [ $nwhich -eq 0 ]; then
echo 
echo " There's no sqlite3 binary in your path."
echo 
exit 1
fi 

g++ -Wall -o ARIN_cache ./src/cache/ARIN_cache.cpp

rm -f ARIN_cache.db

cat > create_db.sql << EOF
create table ARIN_cache (IR, start INTEGER, stop INTEGER);
begin;
EOF

./ARIN_cache >> create_db.sql

cat >> create_db.sql << EOF
commit;
CREATE INDEX idx_ARIN_cache ON ARIN_cache(start,stop);
EOF

sqlite3 ARIN_cache.db < create_db.sql
rm create_db.sql
rm ARIN_cache${WINDOWS}

