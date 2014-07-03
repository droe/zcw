
/*

 includes/includes.hpp
   
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

#include <string>
#include <string.h>
#include <ctime>
#include <iostream>

#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#if defined(SQLITE3)
#include <sqlite3.h>
#endif

#if defined(SERVER)
using std::cin;
#define ALARM_TIMEOUT_STDIN 2
#define ALARM_TIMEOUT_ZCW 30
#endif
#define ALARM_TIMEOUT_WHOIS 12

#if defined(GODEBUG)
#define DEBUG "OK"
#else
#define DEBUG "NO"
#endif

using std::string;
using std::cout;
using std::endl;

#define STRRET "\n"

#define VERSION "5.6"

#define IP_START "0.0.0.0"
#define IP_END "0.0.0.0"


#define DBPATH "/usr/local/share/zcw/"
