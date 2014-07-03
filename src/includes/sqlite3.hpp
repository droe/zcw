
/*

 includes/sqlite3.hpp
   
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

string which_IR_cached(string whois) {
	sqlite3 *db=NULL;
	sqlite3_stmt *stmt=0;
	int result=0;
	string zres="";
	string filename=DBPATH;
	char buf[32];
	string query="";
	u_long ip=ip2long(whois);
	struct stat sts;

	filename+="ARIN_cache.db";
	if(stat(filename.c_str(),&sts)!=0) filename="./ARIN_cache.db";

	sprintf(buf,"%lu",ip);
	query="SELECT IR FROM ARIN_cache WHERE start<="+(string)buf+" AND "+(string)buf+"<=stop ORDER BY stop-start LIMIT 1;";
	if((string)DEBUG=="OK") cout << query << endl;

	sqlite3_open(filename.c_str(),&db);
	if(!db) showerror(6);
	result=sqlite3_prepare(db,query.c_str(),-1,&stmt,NULL);
	if(result!=SQLITE_OK) showerror(6);
	result=sqlite3_step(stmt);
	if(result==SQLITE_ROW) zres=string((char * )sqlite3_column_text(stmt,0));
	result=sqlite3_finalize(stmt);
	if(result!=SQLITE_OK) showerror(6);
	sqlite3_close(db);
	if((string)DEBUG=="OK") cout << zres << endl;
	return zres;
}

