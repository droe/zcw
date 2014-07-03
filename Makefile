
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

# change this to your installation path    
INSTALLPATH     = "/usr/local/"
# we will use INSTALLPATH/bin/ and INSTALLPATH/share/zcw/

CC        = g++
CFLAGS    = -Wall -O2 -L/usr/local/lib -I/usr/local/include
SOLARIS   = `uname -a | sed 's/ //g' | grep -i sparc | grep -i sunos | sed 's/$$/  -lsocket -lnsl -lresolv -R\/usr\/local\/lib/g' | cut -f2- -d" "`
WINDOWS   = `echo ${TERM} | grep cygwin | sed 's/cygwin/.exe/g'`

all:
	@echo
	@echo ' You should read the INSTALL file.'
	@echo

tld:
	@echo
	@echo ' Updating tld include files'
	@exec ./src/scripts/make_tld.sh
	@echo

classic:
	@echo
	@echo ' Building The CyberAbuse Whois (ZCW)'
	@exec ${CC} ${CFLAGS} -o zcw src/zcw.cpp ${SOLARIS}
	@exec ./src/scripts/strip.sh zcw${WINDOWS}
	@echo
	@echo ' Now you can type "make install" as root.'
	@echo

cache:
	@echo
	@echo ' Building ARIN_cache'
	@exec ./src/cache/create_db.sh
	@echo

sqlite-spam:
	@echo
	@echo ' Building The CyberAbuse Whois (ZCW)'
	@exec grep -v DBPATH ./src/includes/includes.hpp > ./src/includes/includes.hpp.new
	@exec cat ./src/includes/includes.hpp.new > ./src/includes/includes.hpp
	@exec rm -f ./src/includes/includes.hpp.new
	@exec echo '#define DBPATH "'${INSTALLPATH}share/zcw/'"' >> ./src/includes/includes.hpp
	@exec ${CC} -DSQLITE3 -DSPAM -DMTA ${CFLAGS} -lsqlite3 -o zcw src/zcw.cpp ${SOLARIS}
	@exec ./src/scripts/strip.sh zcw${WINDOWS}
	@echo
	@echo ' Now you can type "make install" as root.'
	@echo

mta:
	@echo
	@echo ' Building The CyberAbuse Whois (ZCW)'
	@exec grep -v DBPATH ./src/includes/includes.hpp > ./src/includes/includes.hpp.new
	@exec cat ./src/includes/includes.hpp.new > ./src/includes/includes.hpp
	@exec rm -f ./src/includes/includes.hpp.new
	@exec echo '#define DBPATH "'${INSTALLPATH}share/zcw/'"' >> ./src/includes/includes.hpp
	@exec ${CC} -DSQLITE3 -DSPAM -DMTA ${CFLAGS} -lsqlite3 -o zcw src/zcw.cpp ${SOLARIS}
	@exec ./src/scripts/strip.sh zcw${WINDOWS}
	@echo
	@echo ' Now you can type "make install" as root.'
	@echo

sqlite:
	@echo
	@echo ' Building The CyberAbuse Whois (ZCW)'
	@exec grep -v DBPATH ./src/includes/includes.hpp > ./src/includes/includes.hpp.new
	@exec cat ./src/includes/includes.hpp.new > ./src/includes/includes.hpp
	@exec rm -f ./src/includes/includes.hpp.new
	@exec echo '#define DBPATH "'${INSTALLPATH}share/zcw/'"' >> ./src/includes/includes.hpp
	@exec ${CC} -DSQLITE3 ${CFLAGS} -lsqlite3 -o zcw src/zcw.cpp ${SOLARIS}
	@exec ./src/scripts/strip.sh zcw${WINDOWS}
	@echo
	@echo ' Now you can type "make install" as root.'
	@echo

win32:
	@echo
	@echo ' Building The CyberAbuse Whois (ZCW)'
	@exec grep -v DBPATH ./src/includes/includes.hpp > ./src/includes/includes.hpp.new
	@exec cat ./src/includes/includes.hpp.new > ./src/includes/includes.hpp
	@exec rm -f ./src/includes/includes.hpp.new
	@exec echo '#define DBPATH "./"' >> ./src/includes/includes.hpp
	@exec ./src/scripts/get_sqlite3dll.sh
	@exec dlltool --def sqlite3.def --dllname sqlite3.dll --output-lib libsqlite3.a
	@exec ${CC} -static -L./ -DSQLITE3 ${CFLAGS} -lsqlite3 -o zcw src/zcw.cpp ./sqlite3.dll
	@exec ${CC} -static -L./ -DSQLITE3 -DSERVER ${CFLAGS} -lsqlite3 -o zcw-server src/zcw.cpp ./sqlite3.dll
	@exec ./src/scripts/tcpserver.sh
	@exec ./src/scripts/strip.sh zcw${WINDOWS}
	@exec ./src/scripts/strip.sh zcw-server${WINDOWS}
	@echo

debug:
	@echo
	@echo ' Building The CyberAbuse Whois (ZCW)'
	@exec ${CC} -DGODEBUG ${CFLAGS} -o zcw src/zcw.cpp ${SOLARIS}
	@echo
	@echo ' Now you can type "make install" as root.'
	@echo

debug-sqlite:
	@echo
	@echo ' Building The CyberAbuse Whois (ZCW)'
	@exec ${CC} -DSQLITE3 -DGODEBUG ${CFLAGS} -lsqlite3 -o zcw src/zcw.cpp ${SOLARIS}
	@echo
	@echo ' Now you can type "make install" as root.'
	@echo

server:
	@echo
	@echo ' Building The CyberAbuse Whois server (ZCW)'
	@exec ${CC} -DSQLITE3 -DSERVER ${CFLAGS} -lsqlite3 -o zcw-server src/zcw.cpp
	@exec ./src/scripts/strip.sh zcw-server${WINDOWS}
	@echo
	@exec ./src/scripts/tcpserver.sh
	@echo
	@echo ' Now you can type "make install" as root.'
	@echo

clean:
	@exec rm -f core *.core zcw zcw-server tcpserver zcw.exe zcw-server.exe tcpserver.exe
	@echo '*** Ignore the next error, if any.'
	@exec rm -i ARIN_cache.db
	@echo ' Done'

install:
	@echo
	@echo ' Installing executables in '${INSTALLPATH}bin/
	@exec cp -f zcw* ${INSTALLPATH}bin/ &> /dev/null
	@exec mkdir -p ${INSTALLPATH}share/zcw
	@echo
	@echo 'If you used "make server", you should type the following command:'
	@echo 'chown -R nobody ${INSTALLPATH}share/zcw (or equivalent)'
	@echo
	@echo '*** Ignore the next error, if any.'
	@exec cp -f ./ARIN_cache.db ${INSTALLPATH}share/zcw/ &> /dev/null
	@echo

uninstall:
	@echo
	@echo ' Removing zcw executables from '${INSTALLPATH}bin/
	@echo
	@exec rm -i ${INSTALLPATH}bin/zcw*
	@echo '*** Ignore the next error, if any.'
	@exec rm -i ${INSTALLPATH}share/ARIN_cache.db
	@echo

