
/*

 zcw.cpp
   
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

#include "includes/includes.hpp"
#include "includes/phplib.hpp"
#include "includes/net.hpp"
#include "includes/cctld.hpp"
#include "includes/gtld.hpp"
#include "includes/div.hpp"
#include "includes/do_whois.hpp"
#if defined(SQLITE3)
#include "includes/sqlite3.hpp"
#endif
#include "includes/parse.hpp"
#include "includes/update.hpp"
#include "includes/whois.hpp"

int main(int argc, char *argv[]) {

	string ip="";
	string host="";

	string default_email="";
	string default_error_email="none";

	string res_range="";
	string res_netname="";
	string res_as="";
	string res_infos="";
	string res_country="";
	string res_email="";
	string res_source="";
	string res_abusedotnet="";

	bool mode_c=false;
	bool mode_e=false;
	bool mode_s=false;
	bool mode_t=false;
	bool mode_f=false;
	bool mode_w=false;
	bool mode_h=false;

#if defined(SPAM)
	bool mode_a=true;
#else
	bool mode_a=false;
#endif

	string whois_text="";

     #if defined(SERVER)

	if(signal(SIGALRM,sigalrm_handler)==SIG_IGN) signal(SIGALRM,sigalrm_handler);
	alarm(ALARM_TIMEOUT_STDIN);
	cin >> ip;
	alarm(ALARM_TIMEOUT_ZCW);
	if(ip=="") showerror(3);

     #else

	if(argc<2) usage();
	ip=argv[(argc-1)];

	if(php_trim(argv[1])=="update") get_update();

	for(int i=1;i<(argc-1);i++) {
	 if(php_trim(argv[i])=="/c") mode_c=true;
	 if(php_trim(argv[i])=="/e") mode_e=true;
	 if(php_trim(argv[i])=="/s") mode_s=true;
	 if(php_trim(argv[i])=="/a") mode_a=true;
	 if(php_trim(argv[i])=="+a") mode_a=true;
	 if(php_trim(argv[i])=="+f") mode_f=true;
	 if(php_trim(argv[i])=="+w") mode_w=true;
	 if(php_trim(argv[i])=="-t") mode_t=true;
	 if(php_trim(argv[i])=="-h") mode_h=true;
	}

	if(mode_e) default_email=default_error_email;

     #endif

if(!is_ip(ip)) {
	host=ip;
	if(long2ip(ip2long(ip))!=ip) ip=getip(ip);
	if(long2ip(ip2long(ip))!=ip || !is_ip(ip)) showerror(3);
	if(is_ip(host)) host="";
}

// ##### WHOIS PARSING #####

// ##### WHOIS QUERY #####
if(whois_text=="") {
	whois_text=whois(ip);
	for(u_int i=0;i<whois_text.size();i++) if(whois_text[i]=='"') whois_text[i]='\'';
}

if(mode_c) res_country=get_country(whois_text,res_source,mode_t);
if(mode_e) res_email=get_email(whois_text,res_source,default_email,mode_a);
if(mode_a) res_as=get_as(do_whois("whois.cymru.com","-f "+ip));

if(!mode_c && !mode_e && !mode_s) {
	res_source=set_mode(php_strtok(whois_text,STRRET));
	res_range=get_network(whois_text,res_source);
	res_netname=get_netname(whois_text,res_source);
	res_infos=get_infos(whois_text,res_source);
	res_country=get_country(whois_text,res_source,mode_t);
	res_email=get_email(whois_text,res_source,default_email,mode_a);
}
else if(!mode_s) res_source="";

// ##### MODE +W #####

if(res_email!="") {
        if(mode_w && php_strstr(res_email,"abuse@")=="" && php_strstr(res_email,"security")=="" && php_strstr(res_email,"cert")=="" && php_strstr(res_email,"sirt")=="") {
                if(host=="" && gethost(ip)!="") res_abusedotnet=do_whois("whois.abuse.net",gethost(ip));
                else res_abusedotnet=do_whois("whois.abuse.net",host);
                if(res_abusedotnet=="") res_abusedotnet=do_whois("whois.abuse.net",res_email);
        }
} else {
	if(mode_w && (host!="" || gethost(ip)!="")) {
		if(host=="" && gethost(ip)!="") res_abusedotnet=do_whois("whois.abuse.net",gethost(ip));
		else res_abusedotnet=do_whois("whois.abuse.net",host);
	}
}

if(res_abusedotnet!="") res_email=res_abusedotnet;

// ##### CERT.BR #####

if(res_source=="LACNIC") {
   if(php_strrstr(res_email,".br")==".br") res_email+=", cert@cert.br";
   if(res_email=="" && php_strrstr(res_country,"BR")!="") res_email="cert@cert.br";
}

// ##### DISPLAY #####

if(!mode_h) {
cout << echo_header() << endl;
cout << "[ Informations about " << ip << " ]\n" << endl;
}

#if defined(MTA)
   if(res_range!="")   cout << "X-Abuse-Range: "        << res_range   << endl;
   if(res_netname!="") cout << "X-Abuse-Network-Name: " << res_netname << endl;
   if(res_as!="")      cout << "X-Abuse-Announced-by: " << res_as      << endl;
   if(res_country!="") cout << "X-Abuse-Country: "      << res_country << endl;
   if(res_email!="")   cout << "X-Abuse-Email: "        << res_email   << endl;
   if(res_source!="")  cout << "X-Abuse-Source: " << get_whois_srv(res_source) << endl;
#else
   if(res_range!="")   cout << " IP range     :    " << res_range   << endl;
   if(res_netname!="") cout << " Network name :    " << res_netname << endl;
   if(res_as!="")      cout << " Announced by :    " << res_as      << endl;
   if(res_infos!="")   cout << res_infos                            << endl;
   if(res_country!="") {
      if(mode_c) cout << res_country 			 << endl;
      else             cout << " Country      :    " << res_country << endl;
   }
   if(res_email!="") {
      if(mode_e) cout << res_email 				 << endl;
      else             cout << " Abuse E-mail :    " << res_email   << endl;
   }
   if(res_source!="") {
      if(mode_s) cout << get_whois_srv(res_source) 		 << endl;
      else		    cout << " Source       :    " << res_source  << endl;
   }
#endif

if(mode_f) cout << " Whois output :\n\n" << whois_text << endl;

if(!mode_h) cout << echo_footer(res_source) << endl;

return(0);

}

