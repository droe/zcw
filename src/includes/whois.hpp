
/*

 includes/whois.hpp
   
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

string which_IR(string whois) {
size_t len=whois.size();
string str1=cutit(whois,2);
if(str1.find("APNIC")<len) return("whois.apnic.net");
if(str1.find("JAPANI")<len) return("whois.v6nic.net");
if(str1.find("LACNIC")<len) return("whois.lacnic.net");
if(str1.find("OrgID")<len && str1.find("RIPE")<len) return("whois.ripe.net");
if(str1.find("JNIC")<len) return("whois.nic.ad.jp");
if(str1.find("AFRINIC")<len) return("whois.afrinic.net");
if(str1.find("IANA")<len) showerror(4);
int offset=php_substr_count(whois,STRRET);
offset+=-3;
string str2=cutit(whois,offset)+cutit(whois,offset+1)+cutit(whois,offset+2);
if(str2.find("ARIN")<len) return("whois.arin.net");
return("NONE");
}

string which_IR_bis(string whois, string RIR) {
int max=php_substr_count(whois,STRRET);
string line="";
string title="";
string content="";
if(RIR=="whois.lacnic.net" || RIR=="whois.apnic.net") {
        for(int i=0;i<max;i++) {
                line=php_strtok(whois,STRRET);
                title=php_strtok(line,":");
                content=php_trim(line);
                if(title=="ownerid" && content=="BR-CGIN-LACNIC") return ("whois.lacnic.net");
                if(title=="source" && content=="KRNIC") return ("whois.nic.or.kr");
                if(title=="country" && content=="KR") return ("whois.nic.or.kr");
                if(title=="source" && content=="JPNIC") return ("whois.nic.ad.jp");
                if(title=="country" && content=="JP") return ("whois.nic.ad.jp");
        }
}
if(RIR=="whois.arin.net") {
        for(int i=0;i<max;i++) {
                line=php_strtok(whois,STRRET);
                title=php_strtok(line,":");
                content=php_strtoupper(php_trim(line));
                if(title=="Country" && content!="") {
                        if(is_ripe(content)) return ("whois.ripe.net");
                        if(content=="KR") return ("whois.nic.or.kr");
                        if(content=="JP") return ("whois.nic.ad.jp");
                }
        }
}
if(RIR=="whois.ripe.net") {
        for(int i=0;i<max;i++) {
                line=php_strtok(whois,STRRET);
                title=php_strtok(line,":");
                content=php_trim(line);
                if(title=="admin-c" && content=="AFRI-RIPE") return ("whois.afrinic.net");
		if(title=="source") break;
	}
}
return ("NONE");
}

bool jp_kr_nomatch(string whois) {
int max=php_substr_count(whois,STRRET);
string line="";
// size_t len=whois.size();
//   if(whois.find("KRNIC")<len && whois.find("IPv4 Address")>len) return true;
        for(int i=0;i<max;i++) {
                line=php_strtok(whois,STRRET);
		if(line=="Reference: WHOIS servers of RIRs") return true;
		if(line=="The IPv4 address is not managed by KRNIC.") return true;
	}
return false;
}

/* add inetnum and netname from APNIC for KRNIC netblocks */
string get_kr_apnic(string whois) {
string tmp=get_network(whois,"APNIC");
tmp.replace(tmp.find(" - "),3,"-");
return "Network Name : "+get_netname(whois,"APNIC")+STRRET+"IPv4 Address : "+tmp+STRRET;
}

bool lacnic_nomatch(string whois) {
int max=php_substr_count(whois,STRRET);
string line="";
	for(int i=0;i<max;i++) {
		line=php_strtok(whois,STRRET);
		if(php_strtok(line,":")=="% Unallocated and unassigned in LACNIC block") return true;
	}
return false;
}

string clean_lacnic(string whois) {
string line="";
string linetmp="";
string title="";
string newwhois="";
int max=php_substr_count(whois,STRRET);
        for(int i=0;i<max;i++) {
                line=php_strtok(whois,STRRET);
                linetmp=line;
                title=php_strtok(line,":");
                if(title!="inetrev" && title!="nserver" && title!="nsstat" && title!="nslastaa" && title!="created" && title!="changed") newwhois+=linetmp+STRRET;
		if(title=="nic-hdl-br") newwhois+="country: BR\n";
	}
return newwhois;
}

bool arin_org_multiplematch(string whois, string type, string query) {
int max=php_substr_count(whois,STRRET);
string line="";
string title="";
string content="";
int num=0;
	for(int i=0;i<max;i++) {
		line=php_strtok(whois,STRRET);
                title=php_strtok(line,":");
                content=php_trim(line);
		if(type=="OrgID" && title==type && content==query) return false;
		if(type=="OrgName" && title==type) num++;
	}
if(type=="OrgName" && num==1) return false;
return true;
}

string clean_multiple_inetnum(string whois) {
int max=php_substr_count(whois,STRRET);
string line="";
string tmp="";
string title="";
string content="";
        for(int i=0;i<max;i++) {
                line=php_strtok(whois,STRRET);
                tmp+=line+STRRET;
                title=php_strtok(line,":");
                content=php_trim(line);
		if(title=="inetnum") tmp="inetnum: "+content+STRRET;
	}
return tmp;
}

string get_arin_org_in_multi(string whois, string orgid, string orgname) {
int max=php_substr_count(whois,STRRET);
string line="";
string title="";
string content="";
string whoisres="";
string mode="";
string linesave="";
        for(int i=0;i<max;i++) {
                line=php_strtok(whois,STRRET);
		linesave=line;
                title=php_strtok(line,":");
                content=php_trim(line);
		if(title=="OrgName" && mode=="log") return whoisres;
		if(title=="OrgName" && content==orgname) mode="OrgID";
		if(title=="OrgID" && content==orgid && mode=="OrgID") mode="log";
		if(mode=="log") {
			if(title=="OrgID") whoisres="OrgName: "+orgname+STRRET;
			if(content!="") whoisres+=linesave+STRRET;
		}
	}
return "";
}

string arin_netblk_format(string whois) {
int max=php_substr_count(whois,STRRET);
string line="";
string revwhois="";
string net="";
        for(int i=0;i<max;i++) {
                line=php_strtok(whois,STRRET)+STRRET;
                revwhois=line+revwhois;
        }
        for(int i=0;i<max;i++) {
                line=php_strtok(revwhois,STRRET);
                net=php_strstr(line,"(NET-");
                if(net!="" && net.size()>0 && net.size()!=line.size()) return php_strenclose(net,"(",")");
        }
return ("NONE");
}

string add_arin_info(string whois) {
int max=php_substr_count(whois,STRRET);
string line="";
string title="";
string content="";
string orgname="";
string whoistmp="";
string whoisbak=whois;
bool confusion=true;
        for(int i=0;i<max;i++) {
                line=php_strtok(whois,STRRET);
                title=php_strtok(line,":");
                content=php_trim(line);
		if(title=="Updated") confusion=false;
		if(title=="OrgName" && content!="") orgname=content;
		if(title=="OrgID" && content!="") {
			whoistmp=do_whois("whois.arin.net", "o "+content);
			if(arin_org_multiplematch(whoistmp, "OrgID", content)) whoistmp=do_whois("whois.arin.net", "+ "+orgname);
			if(arin_org_multiplematch(whoistmp, "OrgName", orgname)) whoistmp=get_arin_org_in_multi(whoistmp,content,orgname);
		}
	}
	if(confusion && whoistmp=="") whois=do_whois("whois.arin.net", "+ "+arin_netblk_format(whoisbak));
return whois;
}

string jpnic_netblk_format(string whois, string target) {
int max=php_substr_count(whois,STRRET);
string line="";
string revwhois="";
string net="";
        for(int i=0;i<max;i++) {
                line=php_strtok(whois,STRRET)+STRRET;
		if(line=="More Specific Info.") i=max;
                revwhois=line+revwhois;
        }
        for(int i=0;i<max;i++) {
        	line=php_strtok(revwhois,STRRET);
		if(line=="More Specific Info.") return "NONE";
		net=php_trim(php_strstr(line,"]",1));
		if(isin_cidr(target,net)) return net;
	}
return "NONE";
}

string jpnic_catch_lessspec(string whois) {
int max=php_substr_count(whois,STRRET);                  
string line="";
string net="";
        for(int i=0;i<max;i++) {
                line=php_strtok(whois,STRRET)+STRRET;
                if(line.substr(0,12)=="[Allocation]") return do_whois(get_whois_srv("JPNIC"),php_trim(line.substr(13)));
                if(line=="More Specific Info.") i=max;
        }
return "";
}

string ERX(string IP) {
int max=0;
string http_ERX="";
string line="";
string IPstart="";
string IPend="";
string IR="";
size_t where=IP.find_first_of(".",0);
string slash_eight=IP.substr(0,where);
http_ERX=do_whois("www.cyberabuse.org","/whois/erx/"+slash_eight+".all");;
max=php_substr_count(http_ERX,STRRET);
        for(int i=0;i<max;i++) {
                line=php_strtok(http_ERX,STRRET);
		if(line.substr(0,where)==slash_eight) {
			IPstart=php_trim(line.substr(0,15));
			IPend=php_trim(line.substr(16,15));
			IR=php_trim(line.substr(31,8));
			IR=php_strtok(IR," ");
			if(isin_net(IP,IPstart,IPend)) return get_whois_srv(IR);
		}
	}
return "NONE";
}

string whois(string IP) {

string zwhois="";
string query="";
string line="";
string zwhois_c="";
string RIR="";
string RIRbis="NONE";

#if defined(SQLITE3)
	RIR=which_IR_cached(IP);

	if(RIR=="IANA") showerror(4);
	if(RIR=="") {
#endif
		zwhois=do_whois("whois.arin.net",IP);
		RIR=which_IR(zwhois);
#if defined(SQLITE3)
	}
#endif

	if(RIR!="whois.arin.net") zwhois=do_whois(RIR, IP);

	if(RIR=="whois.arin.net") {
                zwhois_c=zwhois;
                line=php_strtok(zwhois_c,STRRET);
              if(line=="No match found for "+IP+".") {
			RIR=ERX(IP);
			if(RIR=="NONE") showerror(0);
			zwhois=do_whois(RIR,IP);
	      }
	      else {
                query=arin_netblk_format(zwhois);
                if(query!="NONE") {
                zwhois="";
                zwhois=do_whois(RIR, query);
                }
                zwhois+=add_arin_info(zwhois);
	      }
        }

	if(RIR=="whois.lacnic.net" && lacnic_nomatch(zwhois)) showerror(0);

	if(RIR=="whois.lacnic.net") zwhois=clean_lacnic(zwhois);

        if(RIR!="whois.nic.ad.jp" && RIR!="whois.afrinic.net") {
                RIRbis=which_IR_bis(zwhois,RIR);
                if(RIRbis!="NONE") {
                        zwhois_c=zwhois;
                        zwhois=do_whois(RIRbis,IP);
                	if((RIRbis=="whois.nic.ad.jp" || RIRbis=="whois.nic.or.kr") && jp_kr_nomatch(zwhois)) {
				zwhois=zwhois_c;
				RIRbis="NONE";
			} else if(RIRbis=="whois.nic.or.kr") zwhois=get_kr_apnic(zwhois_c)+zwhois;
			if(is_iana(zwhois)) {
				zwhois=zwhois_c;
				RIRbis="NONE";
			}
                }
        }

	if(RIR=="whois.apnic.net" && RIRbis=="NONE" && (int)php_substr_count(zwhois,"inetnum:")>1) {
		zwhois_c=zwhois;
		zwhois=clean_multiple_inetnum(zwhois_c);
	}

        if(RIRbis=="whois.nic.ad.jp") {
                query=jpnic_netblk_format(zwhois,IP);
                if(query!="NONE") {
                	zwhois="";
                	zwhois=do_whois(RIRbis,query);
                }
		zwhois+=jpnic_catch_lessspec(zwhois);
        }

if(RIRbis!="NONE") return RIRbis + STRRET + zwhois;
else return RIR + STRRET + zwhois;

}

