
/*

 includes/update.hpp
   
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

void get_update() {
string tmp="";
string query="/whois/?check_updates=";
query+=VERSION;
tmp=php_trim(do_whois("www.cyberabuse.org",query));
if(tmp=="Content-Type: text/html") tmp="";
if(tmp!="") cout << "\n You are running version " << VERSION << ", while version " << tmp << " is available.\n Go to http://www.cyberabuse.org/whois/ to get it.\n" << endl;
else cout << "\n This version of zcw is up to date.\n" << endl;
exit(0);
}

