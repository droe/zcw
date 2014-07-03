
/*

 includes/parse.hpp
   
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

string get_country(string whois, string mode, bool tld_only) {
if(tld_only) {
if(mode=="KRNIC") return "KR";
if(mode=="JPNIC") return "JP";
if(mode=="V6NIC") return "JP";
} else {
if(mode=="KRNIC") return get_countryname("kr")+" (KR)";
if(mode=="JPNIC") return get_countryname("jp")+" (JP)";
if(mode=="V6NIC") return get_countryname("jp")+" (JP)";
}
int max=php_substr_count(whois,STRRET);
string line="";
string title="";
string content="";
        for(int i=0;i<max;i++) {
                line=php_strtok(whois,STRRET);
                title=php_strtolower(php_strtok(line,":"));
                content=php_trim(line);
                if(title=="country" && get_countryname(content)!="Not Found") {
		   if(mode=="AFRINIC") {
			 content=content.substr(0,2);
			 if(content=="EU") return "";
		   }
			if(tld_only) return php_strtoupper(content);
			else return get_countryname(content)+" ("+php_strtoupper(content)+")";
		}
		if(title=="remarks" && content=="mail-abuse@cert.br") {
			if(tld_only) return "BR";
			else return get_countryname("br")+" (BR)";
		}
        }
return "";
}

string get_as(string whois) {
string tmp="";
string asn="";
string asname="";
	for(int i=0;i<3;i++) {
		tmp=php_strtok(whois,"|");
		if(i==0) asn=php_trim(tmp);
		if(i==2) asname=php_trim(tmp);
        }
if(asn=="NA") return "";
return "AS"+asn+" ("+asname+")";
}

string get_netname(string whois, string mode) {
int max=php_substr_count(whois,STRRET);
string line="";
string title="";
string content="";
if(mode=="LACNIC") return "";
if(mode=="ARIN" || mode=="RIPE" || mode=="APNIC" || mode=="AFRINIC") {
        for(int i=0;i<max;i++) {
                line=php_strtok(whois,STRRET);
                title=php_strtolower(php_strtok(line,":"));
                content=php_trim(line);
                if(title=="netname" || title=="nethandle") return php_strtoupper(content);
        }
}
if(mode=="KRNIC") {
        for(int i=0;i<max;i++) {
                line=php_strtok(whois,STRRET);
                title=php_trim(php_strtok(line,":"));
                content=php_trim(line);
                if(title=="Network Name") return php_strtoupper(content);
        }       
}
if(mode=="JPNIC") {
        for(int i=0;i<max;i++) {
                line=php_strtok(whois,STRRET);
                title=php_strtok(line,"]");
                content=php_trim(line);
                if(title=="b. [Network Name") return php_strtoupper(content);
        }
}
if(mode=="V6NIC") {
        for(int i=0;i<max;i++) {
                line=php_strtok(whois,STRRET);
                title=php_strtok(line,":");
                content=php_trim(line);
                if(title=="OrgID") return php_strtoupper(content);
        }
}
return "";
}

string get_network(string whois, string mode) {
int max=php_substr_count(whois,STRRET);
string line="";
string title="";
string content="";
if(mode=="ARIN") {
        for(int i=0;i<max;i++) {
                line=php_strtok(whois,STRRET);
                title=php_strtok(line,":");
                content=php_trim(line);
                if(title=="NetRange") return content;
        }
}
if(mode=="RIPE" || mode=="APNIC" || mode=="AFRINIC") {
        for(int i=0;i<max;i++) {
                line=php_strtok(whois,STRRET);
                title=php_strtok(line,":");
                content=php_trim(line);
                if(title=="inetnum") return content;
        }
}
if(mode=="LACNIC") {
        for(int i=0;i<max;i++) {
                line=php_strtok(whois,STRRET);
                title=php_strtok(line,":");
                content=php_trim(line);
                if(title=="inetnum") return broadconv(content);
        }
}
if(mode=="KRNIC") {
        for(int i=0;i<max;i++) {
                line=php_strtok(whois,STRRET);
                title=php_trim(php_strtok(line,":"));
                content=php_trim(line);
                if(title=="IPv4 Address") return content.replace(content.find("-"),1," - ");
        }
}
if(mode=="JPNIC") {
        for(int i=0;i<max;i++) {
                line=php_strtok(whois,STRRET);
                title=php_strtok(line,"]");
                content=php_trim(line);
                if(title=="a. [Network Number") {
			if(is_ip(content)) return broadconv(content+"/24");
			if(is_cidr(content)) return broadconv(content);
			content=unfuck_netnumber(content);
			return content.replace(content.find("-"),1," - ");
		}
        }
}
if(mode=="V6NIC") {
        for(int i=0;i<max;i++) {
                line=php_strtok(whois,STRRET);
                title=php_trim(php_strtok(line,":"));
                content=php_trim(line);
                if(title=="Network") return broadconv(content);
        }
}
return "";
}

string get_infos(string whois, string mode) {

string STRINF=" Infos        :    ";

int max=php_substr_count(whois,STRRET);

string whoissave="";
string line="";
string linesave="";
string title="";
string content="";
string company="";
string address="";
string city="";

if(mode=="ARIN") {
        for(int i=0;i<max;i++) {
                line=php_strtok(whois,STRRET);
		linesave=line;
                title=php_strtok(line,":");
                content=php_trim(line);
		if(title==linesave && content=="") content=linesave;
                if(title=="OrgName" || title=="CustName") company=content;
                if(title=="Company" && content!="" && content!=title) company=content;
                if(title=="Address") mode="address";
                if(title=="Country") break;
                if(company!="" && address=="" && title=="Address" && php_strstr(content,company)!="") content=php_trim(php_strstr(content,company));
		if((title=="Address" || mode=="address") && company!=content && content.size()>0) address+=STRINF+content+STRRET;
        }
if(address=="") return STRINF+company;
else return STRINF+company+STRRET+address.substr(0,(address.size()-1));
}

if(mode=="RIPE" || mode=="AFRINIC") {
	whoissave=whois;
        for(int i=0;i<max;i++) {
                line=php_strtok(whois,STRRET);
                title=php_strtok(line,":");
                content=php_trim(line);
		if(title=="descr") address+=STRINF+content+STRRET;
		if(title=="source") break;
	}
	if(address=="" && mode!="AFRINIC") {
	whois=whoissave;
         for(int i=0;i<max;i++) {
                line=php_strtok(whois,STRRET);
                title=php_strtok(line,":");
                content=php_trim(line);
	 	if(title=="role" || title=="person") break;
	 	if(title=="role") mode="role";
	 	if(title=="person") mode="person";
	 	if(title=="address" && (mode=="role" || mode=="person")) address+=STRINF+content+STRRET;
	 }
	}
if(address!="") return address.substr(0,(address.size()-1));
}

if(mode=="LACNIC") {
	for(int i=0;i<max;i++) {
                line=php_strtok(whois,STRRET);
                title=php_strtok(line,":");
                content=php_trim(line);
                if(title=="owner") company=content;
                if(company!="" && address=="" && title=="address" && php_strstr(content,company)!="") content=php_trim(php_strstr(content,company));
		if(title=="address" && company!=content) address+=STRINF+content+STRRET;
		if(title=="nic-hdl" || title=="nic-hdl-br" || title=="owner-c") break;
	}
if(address=="") return STRINF+company;
else return STRINF+company+STRRET+address.substr(0,(address.size()-1));
}

if(mode=="APNIC") {
        for(int i=0;i<max;i++) {
                line=php_strtok(whois,STRRET);
                title=php_strtok(line,":");
                content=php_trim(line);
		if(title=="descr" || title=="address") address+=STRINF+content+STRRET;
		if(title=="country" && address!="") break;
	}
if(address!="") return address.substr(0,(address.size()-1));
}

if(mode=="KRNIC") {
        for(int i=0;i<max;i++) {
                line=php_strtok(whois,STRRET);
                title=php_trim(php_strtok(line,":"));
                content=php_trim(line);
		if(title=="Address" && address=="") address+=STRINF+content+STRRET;
		if(title=="Org Address") address+=STRINF+content+STRRET;
		if(title=="Org Name") company+=STRINF+content+STRRET;
		if(title=="Service Name") company=company.substr(0,(company.size()-1))+" ("+content+")"+STRRET;
		if(title=="State") city+=content;
		if(title=="Zip Code") break;
	}
content="";
if(company!="") content=company;
if(address!="") content+=address;
if(city!="") content+=STRINF+city+STRRET; 
return content.substr(0,(content.size()-1));
}

if(mode=="JPNIC") {
        for(int i=0;i<max;i++) {
                line=php_strtok(whois,STRRET);
                title=php_strtok(line,"]");
                content=php_trim(line);
		if(title=="g. [Organization") address=STRINF+content;
	}
if(address!="") return address;
}

if(mode=="V6NIC") {
        for(int i=0;i<max;i++) {
                line=php_strtok(whois,STRRET);
                title=php_strtok(line,":");
                content=php_trim(line);
                if(title=="OrgName") company=STRINF+content+STRRET;
                if(title=="OrgAddress") address=STRINF+content;
        }
return company+address;
}

return "";
}

string get_email(string whois, string mode, string default_email, bool antispam) {

int max=php_substr_count(whois,STRRET);
int maxtmp;

string whoissave="";
string line="";
string title="";
string content="";

string whoistmp="";
string tmp="";

string admin_email="";
string tech_email="";
string org_email="";
string noc_email="";

if(mode=="ARIN") {
        for(int i=0;i<max;i++) {
                line=php_strtok(whois,STRRET);
                title=php_strtok(line,":");
                content=php_strtolower(php_trim(line));
                if( !antispam && ( ( title=="Comment" && (php_strstr(content,"abuse")!="" || php_strstr(content,"security")!="") ) || php_strstr(title,"AbuseEmail")!="" ) && getmail(content)!="") return getmail(content);
                if( antispam && ( ( title=="Comment" && (php_strstr(content,"abuse")!="" || php_strstr(content,"spam")!="") ) || php_strstr(title,"AbuseEmail")!="" ) && getmail(content)!="") return getmail(content);
                if(title=="OrgNOCEmail" && getmail(content)!="") noc_email=getmail(content);
                if(title=="AdminEmail" && getmail(content)!="") admin_email=getmail(content);
                if(title=="RTechEmail" && getmail(content)!="") tech_email=getmail(content);
                if(title=="OrgTechEmail" && getmail(content)!="") org_email=getmail(content);
		if(title=="Parent" && php_strstr(content,"net-")!="") tmp=content;
	}

if(admin_email!="") return admin_email;
if(noc_email!="") return noc_email;
if(tech_email!="") return tech_email;
if(org_email!="") return org_email;

whois=do_whois("whois.arin.net",tmp);

max=php_substr_count(whois,STRRET);

        for(int i=0;i<max;i++) {
                line=php_strtok(whois,STRRET);
                title=php_strtok(line,":");
                content=php_strtolower(php_trim(line));
                if( !antispam && ( ( title=="Comment" && (php_strstr(content,"abuse")!="" || php_strstr(content,"security")!="") ) || php_strstr(title,"AbuseEmail")!="" ) && getmail(content)!="") return getmail(content);
                if( antispam && ( ( title=="Comment" && php_strstr(content,"spam")!="" ) || php_strstr(title,"AbuseEmail")!="" ) && getmail(content)!="") return getmail(content);
                if(title=="OrgNOCEmail" && getmail(content)!="") noc_email=getmail(content);
		if(title=="AdminEmail" && getmail(content)!="") admin_email=getmail(content);
                if(title=="RTechEmail" && getmail(content)!="") tech_email=getmail(content);
                if(title=="OrgTechEmail" && getmail(content)!="") org_email=getmail(content);
	}

if(admin_email!="") return admin_email;
if(noc_email!="") return noc_email;
if(tech_email!="") return tech_email;
if(org_email!="") return org_email;

}


if(mode=="RIPE" || mode=="AFRINIC") {

whoissave=whois;

        for(int i=0;i<max;i++) {
                line=php_strtok(whoissave,STRRET);
                title=php_strtok(line,":");
                content=php_strtolower(php_trim(line));
                if(title=="mnt-irt") { tmp=content; break; }
	}

/* IRT object query... */
	if(tmp!="") {
	        whoistmp=do_whois(get_whois_srv(mode),tmp);
      		maxtmp=php_substr_count(whoistmp,STRRET);
	  	for(int i=0;i<maxtmp;i++) {
	                line=php_strtok(whoistmp,STRRET);
	                title=php_strtok(line,":");
	                content=php_strtolower(php_trim(line));
			if(title=="e-mail") return getmail(content);
                	if(title=="abuse-mailbox") return getmail(content);
                	if(title=="source") break;
                }
	tmp="";
	}

whoissave=whois;

/* abuse-mailbox, or e-mail/email/descr/remarks field with an email and "abuse|security|csirt|cert|hacking" on the same line */
/* then first tech-c's mail found */
/* then first admin-c's mail found */
/* then first role's mail found */

        for(int i=0;i<max;i++) {
                line=php_strtok(whois,STRRET);
                title=php_strtok(line,":");
                content=php_strtolower(php_trim(line));

		if(title=="role") mode="role";
		if(title=="organisation") mode="role";
		if(title=="person") mode="person";

	if(!antispam) {
		if((title=="remarks" || title=="e-mail" || title=="email" || title=="descr") && ( php_strstr(content,"abuse")!="" || php_strstr(content,"security")!="" || php_strstr(content,"hacking")!="" ) && getmail(content)!="") return getmail(content);
		if((title=="remarks" || title=="descr") && getmail(content)!="" && ( php_strstr(getmail(content),"abuse@")!="" || php_strstr(getmail(content),"security@")!="" || php_strstr(getmail(content),"sirt@")!="" || php_strstr(getmail(content),"cert@")!="" )) return getmail(content);
	} else {
		if((title=="remarks" || title=="descr") && getmail(content)!="" && ( php_strstr(getmail(content),"spam@")!="" || php_strstr(getmail(content),"sirt@")!="" || php_strstr(getmail(content),"cert@")!="" )) return getmail(content);
		if((title=="remarks" || title=="e-mail" || title=="email" || title=="descr") && ( php_strstr(content,"spam")!="" || php_strstr(content,"abuse@")!="" ) && getmail(content)!="") return getmail(content);
	}

                if(title=="abuse-mailbox") return getmail(content);

		if(org_email=="" && mode=="role" && (title=="email" || title=="e-mail") && getmail(content)!="") org_email=getmail(content); 

		if(admin_email=="" && mode=="person" && (title=="email" || title=="e-mail") && getmail(content)!="") admin_email=getmail(content); 

		if(tech_email=="" && admin_email=="" && mode=="person" && (title=="email" || title=="e-mail") && getmail(content)!="") tech_email=getmail(content); 

	}

if(tech_email!="") return tech_email;
if(admin_email!="") return admin_email;
if(org_email!="") return org_email;

whoissave=whois;

	for(int i=0;i<max;i++) {
	        line=php_strtok(whoissave,STRRET);
	        title=php_strtok(line,":");
        	content=php_strtolower(php_trim(line));
		if((title=="e-mail" || title=="email") && getmail(content)!="") return getmail(content);
		if(tech_email=="" && title=="notify" && php_strstr(content,"ripe.net")=="" && php_strstr(content,"hostmaster@")=="" && getmail(content)!="") tech_email=getmail(content);
		if(admin_email=="" && title=="changed" && php_strstr(content,"ripe.net")=="" && php_strstr(content,"hostmaster@")=="" && getmail(content)!="") admin_email=getmail(content);
	}

if(tech_email!="") return tech_email;
if(admin_email!="") return admin_email;

}

if(mode=="LACNIC") {
whoissave=whois;
        for(int i=0;i<max;i++) {
                line=php_strtok(whois,STRRET);
                title=php_strtok(line,":");
                content=php_strtolower(php_trim(line));
		if(!antispam && title=="e-mail" && (php_strstr(content,"abuse")!="" || php_strstr(content,"security")!="") && getmail(content)!="") return getmail(content);
		if(antispam && title=="e-mail" && php_strstr(content,"spam")!="" && getmail(content)!="") return getmail(content);
	}
if(antispam) {
whoissave=whois;
        for(int i=0;i<max;i++) {
                line=php_strtok(whois,STRRET);
                title=php_strtok(line,":");
                content=php_strtolower(php_trim(line));
                if(title=="e-mail" && (php_strstr(content,"abuse")!="" || php_strstr(content,"security")!="") && getmail(content)!="") return getmail(content);
        }
}
whois=whoissave;
        for(int i=0;i<max;i++) {
                line=php_strtok(whois,STRRET);
                title=php_strtok(line,":");
                content=php_trim(line);
		if(title=="abuse-c") tmp=content;
		if((title=="nic-hdl-br" || title=="nic-hdl") && content==tmp) mode="catchit";
		if((title=="nic-hdl-br" || title=="nic-hdl") && mode=="catchit") break;
		if(mode=="catchit" && title=="e-mail" && getmail(content)!="") return getmail(content);
	}
whois=whoissave;
        for(int i=0;i<max;i++) {
                line=php_strtok(whois,STRRET);
                title=php_strtok(line,":");
                content=php_trim(line);
                if((title=="e-mail" || title=="email") && getmail(content)!="") return getmail(content);
	}
}

if(mode=="APNIC") {
        for(int i=0;i<max;i++) {
                line=php_strtok(whois,STRRET);
                title=php_strtok(line,":");
                content=php_strtolower(php_trim(line));
	if(!antispam) {
		if((title=="remarks" || title=="descr" || title=="trouble") && getmail(content)!="" && ( php_strstr(getmail(content),"abuse@")!="" || php_strstr(getmail(content),"security@")!="" || php_strstr(getmail(content),"sirt@")!="" || php_strstr(getmail(content),"cert@")!="" || php_strstr(getmail(content),"hacking@")!="" )) return getmail(content);
		if((title=="remarks" || title=="e-mail" || title=="email" || title=="descr" || title=="trouble") && (php_strstr(content,"abuse")!="" || php_strstr(content,"security")!="") && getmail(content)!="") return getmail(content);
	} else {
		if((title=="remarks" || title=="e-mail" || title=="email" || title=="descr" || title=="trouble") && php_strstr(content,"spam")!="" && getmail(content)!="") return getmail(content);
		if((title=="remarks" || title=="descr" || title=="trouble") && getmail(content)!="" && ( php_strstr(getmail(content),"spam@")!="" || php_strstr(getmail(content),"abuse@")!="" || php_strstr(getmail(content),"sirt@")!="" || php_strstr(getmail(content),"cert@")!="")) return getmail(content);
	}
		if(title=="person") mode="go";
		if(mode=="go" && tech_email=="" && (title=="email" || title=="e-mail") && getmail(content)!="") tech_email=getmail(content);
		if(noc_email=="" && (title=="email" || title=="e-mail") && getmail(content)!="") noc_email=getmail(content);
	}
if(tech_email!="") return tech_email;
if(noc_email!="") return noc_email;
}

if(mode=="KRNIC") {
        for(int i=0;i<max;i++) {
                line=php_strtok(whois,STRRET);
		tmp=line;
                title=php_trim(php_strtok(line,":"));
                content=php_trim(line);
		if(php_strstr(tmp,"Network Abuse Contact")!="") mode="security";
		if(mode=="security" && php_strtolower(title)=="e-mail" && getmail(content)!="") return getmail(content);
		if(tech_email=="" && php_strtolower(title)=="e-mail" && getmail(content)!="") tech_email=getmail(content);
	}
if(tech_email!="") return tech_email;
}

if(mode=="JPNIC") {
whoistmp=whois;
        for(int i=0;i<max;i++) {
                line=php_strtok(whois,STRRET);
                tmp=line;
		if(line=="Less Specific Info.") i=max;
                title=php_strtok(line,"]");
                content=php_trim(line);
                if(title=="[Abuse") return getmail(content);
	}
whois=whoistmp;
        for(int i=0;i<max;i++) {
                line=php_strtok(whois,STRRET);
		tmp=line;
                title=php_strtok(line,"]");
                content=php_trim(line);
		if(title=="m. [Administrative Contact") {
			whoistmp=do_whois("whois.nic.ad.jp",content);
			whoissave=content;
			maxtmp=php_substr_count(whoistmp,STRRET);
		        for(int e=0;e<maxtmp;e++) {
		                tmp=php_strtok(whoistmp,STRRET);
		                title=php_strtok(tmp,"]");
		                content=php_trim(tmp);
				if(title=="[E-Mail" && php_strstr(content,"@")!="" && getmail(content)!="") return getmail(content);
			}
		tmp="";
		}
		if(title=="n. [Technical Contact" && content!=whoissave) {
                        whoistmp=do_whois("whois.nic.ad.jp",content);
                        maxtmp=php_substr_count(whoistmp,STRRET);
                        for(int e=0;e<maxtmp;e++) {
                                tmp=php_strtok(whoistmp,STRRET);
                                title=php_strtok(tmp,"]");
                                content=php_trim(tmp);
                                if(title=="[E-Mail" && php_strstr(content,"@")!="" && getmail(content)!="") return getmail(content);
                        }
		tmp="";
		}
		if(title=="[Abuse") org_email=getmail(content);
		if(php_strstr(tmp,"@")!="" && php_strstr(tmp,".nic.")=="" && getmail(content)!="") noc_email=getmail(tmp);
	}
if(noc_email!="") return noc_email;
if(org_email!="") return org_email;
}

if(mode=="V6NIC") {
        for(int i=0;i<max;i++) {
                line=php_strtok(whois,STRRET);
                title=php_trim(php_strtok(line,":"));
                content=php_trim(line);
		if(title=="OrgMail") return getmail(content);
	}
}

return default_email;
}

