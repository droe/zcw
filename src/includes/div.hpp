
/*

 includes/div.hpp
   
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

string cutit(string text, int where) {
int max=php_substr_count(text,STRRET);
if (where>max) where=max;
string tmp="";
	for(int i=0;i<where;i++) {
		tmp=php_strtok(text,STRRET);
	}
return tmp;
}

bool is_email(string email) {
bool state=false;
size_t emailsize=email.size();
string dom=php_strrstr(email,"@",1);
string tld=php_strrstr(email,".",1);
size_t usersize=email.size();
string user=php_strtok(email,"@");
if(email=="") return false;
if(email.find("bitbucket@")<emailsize) return false;
if(get_countryname(tld)!="Not Found") state=true;
if(is_gtld(php_strtoupper(tld))) state=true;
if(is_ip(dom) && dom!="") state=true;
if(state==true && usersize<emailsize && user.find(" ")>usersize) state=true;
return state;
}

string getmail(string str) {
int max=php_substr_count(str," ");
int nmbmails=php_substr_count(str,"@");
string oldmail="";
string tmp="";
	for(int i=0;i<=max;i++) {
	 str=php_trim(str);
 	 tmp=php_strenclose(php_strenclose(php_strenclose(php_strenclose(php_strenclose(php_strtok(str," "),"(",")"),"<",">"),"[","]"),"\"","\""),"'","'");
	  if(tmp.find(":")<tmp.size()) tmp=php_strstr(tmp,":",1);
	  if(tmp.find_last_of(".")==tmp.size()-1) tmp=php_substr(tmp,0,tmp.size()-1);
	  if(tmp.find_last_of(",")==tmp.size()-1) tmp=php_substr(tmp,0,tmp.size()-1);
	  if(tmp.find_last_of(";")==tmp.size()-1) tmp=php_substr(tmp,0,tmp.size()-1);
	    if(is_email(tmp)) {
	      if(nmbmails>1) {
		if(tmp.find("abuse")<tmp.size() || tmp.find("security")<tmp.size()) return(php_strtolower(tmp));
		else oldmail=php_strtolower(tmp);
	      }
	      else return(php_strtolower(tmp));
	    }
	  }
if(nmbmails>1) return oldmail;
return "";
}

bool is_ripe(string str) {
string needle="GL|MT|SM|AD|LI|FO|GI|GE|IS|IE|GB|PT|MC|ES|FR|VA|CH|BE|NL|LU|DE|DK|NO|SE|FI|CZ|AT|SI|IT|HR|SK|HU|PL|YU|MK|GR|CY|BG|RU|LT|LV|EE|UA|MD|RO|BA|BY|AL|SJ|KW|BH|IR|JO|SA|SY|EG|AM|TR|AE|YE|IQ|AZ|LB|IL|GZ";
int max=php_substr_count(needle,"|")+1;
string tmp;
        for(int i=0;i<max;i++) {
        	tmp=php_strtok(needle,"|");
        	if(str==tmp) return true;
        }
return false;
}

bool is_iana(string whois) {
size_t len=whois.size();
string str=cutit(whois,7);
if(str.find("IANA-BLK")<len) return true;
if(whois.find("IANA-")<len) return true;
if(whois.find("bitbucket@ripe.net")<len) return true;
return false;
}

string IR_URL(string IR) {
if(IR=="RIPE") return "% Rights restricted by copyright.\n% See http://www.ripe.net/ripencc/pub-services/db/copyright.html";
if(IR=="APNIC") return "% Whois data copyright terms:\n% http://www.apnic.net/db/dbcopyright.html";
if(IR=="LACNIC") return "% Copyright LACNIC - lacnic.net";
if(IR=="AFRINIC") return "% Copyright AfriNIC - afrinic.net";
if(IR=="JPNIC") return "% Copyright JPNIC\n% See : http://jprs.jp/en/copyright.html";
if(IR=="KRNIC") return "% Copyright Korea Network Information Center";
if(IR=="V6NIC") return "% Copyright IPv6 Promotion Council - v6nic.net";
return "";
}

string echo_header() {
string header;
header="% This is the CyberAbuse Whois v";
header+=VERSION;
header+="\n\n";
return header;
}

string echo_footer(string IR) {
string footer=STRRET; 
if(IR!="" && IR!="ARIN") footer+=STRRET+IR_URL(IR)+STRRET;
footer+="\n% The CyberAbuse Whois\n% Copyright 2003-2008, Philippe Bourcier\n% http://www.cyberabuse.org/whois/\n";
return footer;
}

void usage() {
string usage=" Usage: zcw [flags] <host>\n\n  Flags are:\n\n   * /e : search for the \"Abuse E-mail\" only\n   * /c : search for the \"Country\" only\n   * /s : search for the \"Source\" only\n   * /a : optimize e-mail search for spam report\n\n   * +a : search for \"Announced by\" infos\n   * +f : display the IP whois query output\n   * +w : use abuse.net to correct \"Abuse E-mail\"\n\n   * -h : remove headers from output\n   * -t : show only the TLD in the \"Country\" field\n\n   * update : check if an update is available";
cout << echo_header() << endl;
cout << usage << endl;
cout << echo_footer("") << endl;
exit(0);
}

void showerror(int code) {
#ifndef MTA
string error;
if (code==0) error="No entry was found for this IP.";
if (code==1) error="Could not connect to remote server.";
if (code==2) error="Could not find a whois server for this IP.";
if (code==3) error="This is not a valid IP.";
if (code==4) error="This IP is in the IANA IP space or ARIN_cache.db needs to be updated.";
if (code==5) error="Connection timed out.";
if (code==6) error="Database error.";
cout << echo_header() << endl;
cout << "% ERROR: " << error << endl;
cout << echo_footer("") << endl;
#endif
exit(1);
}

static void sigalrm_handler(int x) {
showerror(5);
}

string set_mode(string whois) {
if (whois=="whois.arin.net") return "ARIN";
if (whois=="whois.ripe.net") return "RIPE";
if (whois=="whois.apnic.net") return "APNIC";
if (whois=="whois.lacnic.net") return "LACNIC";
if (whois=="whois.afrinic.net") return "AFRINIC";
if (whois=="whois.v6nic.net") return "V6NIC";
if (whois=="whois.nic.or.kr") return "KRNIC";
if (whois=="whois.nic.ad.jp") return "JPNIC";
return "";
}

string get_whois_srv(string mode) {
if (mode=="ARIN") return "whois.arin.net";
if (mode=="RIPE") return "whois.ripe.net";
if (mode=="APNIC") return "whois.apnic.net";
if (mode=="LACNIC") return "whois.lacnic.net";
if (mode=="AFRINIC") return "whois.afrinic.net";
if (mode=="V6NIC") return "whois.v6nic.net";
if (mode=="KRNIC") return "whois.nic.or.kr";
if (mode=="JPNIC") return "whois.nic.ad.jp";
if (mode=="Asia") return "whois.apnic.net";
if (mode=="Afri") return "whois.afrinic.net";
if (mode=="Lati") return "whois.lacnic.net";
return "NONE";
}

