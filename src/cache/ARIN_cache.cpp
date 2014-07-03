
/*

 ARIN_cache.cpp
   
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
#include <iostream>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

using std::string;
using std::cout;
using std::endl;

#define STRRET "\n"

u_long ip2long(string ip_str) {
const char *ip_chr=ip_str.c_str();
struct in_addr addr;
if(inet_aton(ip_chr, &addr)) return (u_long)ntohl((u_long)addr.s_addr);
else return 0;
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

string php_trim(string str, string in="\n\r\t\v ") {
        if(str=="") return "";
        size_t a = str.find_first_not_of(in);
        size_t b = str.find_last_not_of(in)+1;
if(a!=b) return str.substr(a,b-a);
else return "";
}

int php_substr_count(string str, string needle) {

string::size_type lenstr=str.size();
string::size_type len=needle.size();

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

bool bigerx(string line) {
if(line.find("-0-0-0-0)")<line.size() && line.find("-ERX-")<line.size()) return true;
return false;
}

string do_whois(string whois_q) {

        int s;
        string whois="whois.arin.net";
        const char *host=whois.c_str();
        FILE *sfi, *sfo;
        char buf[1024];
        size_t len;
        string result;
        string tmp;

        struct sockaddr_in server;
        struct sockaddr_in local;
        struct hostent *hostinfo;

        int port = 43;

        whois_q="! > "+whois_q;
        const char *query=whois_q.c_str();

        if((hostinfo=gethostbyname(host))==NULL) exit(1);

        memset((char *) &local, 0, sizeof(struct sockaddr));

        local.sin_family = AF_INET;
        local.sin_port = htons(0);
        memset((char *) &server, 0, sizeof(struct sockaddr));
        memcpy((char *) &server.sin_addr, hostinfo->h_addr, hostinfo->h_length);
        server.sin_family = AF_INET;
        server.sin_port = htons(port);

        if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) exit(1);
        if(bind(s, (struct sockaddr *) &local, sizeof(struct sockaddr)) < 0) exit(1);
        if (connect(s, (struct sockaddr *) &server, sizeof(struct sockaddr)) < 0) exit(1);

        sfi = fdopen(s, "r");
        sfo = fdopen(s, "w");

        if (sfi == NULL || sfo == NULL) exit(1);

        fprintf(sfo, "%s\r\n", query);
        fflush(sfo);

        while(fgets(buf, 1024, sfi)) {
                len = strlen(buf);
                while (len && isspace((int)buf[len-1])) buf[--len] = '\0';
                tmp = string(buf);
                if (tmp!="" && tmp.find("NET-")<tmp.size() && !bigerx(tmp)) result += php_trim(tmp.substr(tmp.find(")")+1)) + STRRET;
        }

return result;

}

void sqlmaker(string whoisres,string IR) {
  size_t where;
  int max=php_substr_count(whoisres,STRRET);
  string line="";
  string ipstart="";
  string ipstop="";
        for(int i=0;i<max;i++) {
		line=php_strtok(whoisres,STRRET);
                where=line.find_first_of("-",0);
                if(where>0){
                        ipstart=php_trim(line.substr(0,(where-1)));
                        ipstop =php_trim(line.substr((where+2),(line.size()-1)));
                        cout << "insert into ARIN_cache values ('" << IR << "', '" << ip2long(ipstart) << "', '" << ip2long(ipstop) << "');" << endl;
                }
        }
}

int main() {

	sqlmaker(do_whois("RIPE"),"whois.ripe.net");
	sqlmaker(do_whois("IANA"),"IANA");
	sqlmaker(do_whois("APNIC"),"whois.apnic.net");
	sqlmaker(do_whois("JAPANI"),"whois.v6nic.net");
	sqlmaker(do_whois("LACNIC"),"whois.lacnic.net");
	sqlmaker(do_whois("JNIC"),"whois.nic.ad.jp");
	sqlmaker(do_whois("AFRINIC"),"whois.afrinic.net");

return(0);

}

