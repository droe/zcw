
/*

 includes/phplib.hpp
   
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

string php_strtolower(string str) {
	for(string::size_type i=0,end=str.size();i<end;i++) {
        	str[i] = tolower(str[i]);
	}
return str;
}

string php_strtoupper(string str) {
	for(string::size_type i=0,end=str.size();i<end;i++) {
        	str[i] = toupper(str[i]);
	}
return str;
}

string php_strtok(string &str, string tok) {
        size_t len=str.find_first_of(tok,0);
        string tmpstr="";

        if (len>0) {
                tmpstr = str.substr(0,len);
                if (len<str.size()) str=str.substr(len+1,str.size()-len+1);
                else str="";
        }
        else {
                tmpstr = str;
                str ="";
        }
return tmpstr;
}

string php_strstr(string str, string needle, int offset=0) {
	if(needle==php_strtolower(needle)) str=php_strtolower(str);
	if(str==needle) return str;
	if(str.size()<(needle.size()+offset)) return "";
	int start=str.find(needle)+offset;
	if(start==offset) start=needle.size()+offset;
if(start<offset) return "";
else return str.substr(start);
}

string php_strrstr(string str, string needle, int offset=0) {
	int start=str.rfind(needle)+offset;
if(start<1) return "";
else return str.substr(start);
}

string php_strenclose(string str, string start, string stop, int offset=0) {
	int begn=str.find(start)+offset+1;
	str=str.substr(begn);
	int end=str.find(stop);
return str.substr(0,end);
}

string php_trim(string str, string in="\n\r\t\v ") {
	if(str=="") return "";
	size_t a = str.find_first_not_of(in);
	size_t b = str.find_last_not_of(in)+1;
if(a!=b) return str.substr(a,b-a);
else return "";
}

string php_substr(string str, int start=0, int length=0) {
	if (length==0) { length=str.size(); }
	if (start<0) { start=str.size()+start; }
	if (length<0) { length=str.size()-start+length; }
return str.substr(start,length);
}

int php_substr_count(string str, string needle) {

string::size_type lenstr=str.size();
string::size_type len=needle.size();

php_strtolower(str);
php_strtolower(needle);

int count=0;
int old = 0;
int n = 0;


if (len==0 || lenstr==0) { return(count); }

if (len==1) {
	for(size_t i=0;i<lenstr;i++) {
		if (str[i]==needle[0]) { count++; }
	} 
}
else {

string p = str.substr(0,len);

	for(int i=0,max=lenstr+len;i<max;i+=len) {
		n = p.rfind(needle);
		p = str.substr(0,i);
		if (old!=n && n>=0) count++;
		old = n;
	}
}

return(count);

}


