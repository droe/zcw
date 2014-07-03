
/*

 includes/net.hpp
   
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

bool is_ip(string ip) {
string num;
int num_i;
int max=php_substr_count(ip,".");
if (max!=3) return false;
for(int i=0;i<=max;i++) {
num=php_strtok(ip,".");
num_i=atoi(num.c_str());
if(i==0 && num=="0" && num_i==0) return false;
if (num_i>255 || (num_i==0 && num!="0") || num_i<0) return false;
}
return true;
}

bool is_cidr(string cidr) {
string ip=php_strtok(cidr,"/");
if(!is_ip(ip)) return false;
int mask=atoi(cidr.c_str());
if(mask<=32 && mask>7) return true;
return false;
}

string getip(string hostname) {
hostent *h=NULL;
const char *h_chr=hostname.c_str();
if ((h=gethostbyname(h_chr)) == NULL) return hostname;
else return inet_ntoa(*(reinterpret_cast<in_addr*>(h->h_addr)));
}

string gethost(string ip) {
struct hostent *h=NULL;
struct in_addr addr;
string hostname="";
addr.s_addr=inet_addr(ip.c_str());
if((h=gethostbyaddr((const char *)&addr,sizeof(addr),AF_INET)) != NULL) hostname = h->h_name;
return hostname;
}

u_long ip2long(string ip_str) {
const char *ip_chr=ip_str.c_str();
struct in_addr addr;
if(inet_aton(ip_chr, &addr)) return (u_long)ntohl((u_long)addr.s_addr);
else return 0;
}

string long2ip(u_long iplong) {
struct in_addr addr;
addr.s_addr=(u_long)htonl(iplong);
return inet_ntoa(addr);
}

bool isin_cidr(string ip, string cidr) {
if(!is_ip(ip) || !is_cidr(cidr)) return false;
u_long ip_start=ip2long(php_strtok(cidr,"/"));
u_long nb_ips=(1<<(32-atoi(cidr.c_str())))-1;
u_long ip_stop=ip_start+nb_ips;
u_long ip_search=ip2long(ip);
if(ip_search>=ip_start && ip_search<=ip_stop) return true;
return false;
}

bool isin_net(string ip, string IPstart, string IPstop) {
if(!is_ip(IPstart) || !is_ip(IPstop)) return false;
u_long ip_start=ip2long(IPstart);
u_long ip_stop=ip2long(IPstop);
u_long ip_search=ip2long(ip);
if(ip_search>=ip_start && ip_search<=ip_stop) return true;
return false;
}

string broadconv(string net) {
u_long ip;
int netmask;
int nb_ips;
int broadcast;
string str_ip=php_strtok(net,"/");
int missingdots=3-php_substr_count(str_ip,".");
if(!is_ip(str_ip)) for(int i=0;i<missingdots;i++) str_ip+=".0";
ip=ip2long(str_ip);
netmask=atoi(net.c_str());
nb_ips=1<<(32-netmask);
nb_ips--;
broadcast=ip+nb_ips;
if(netmask>32 && netmask<1) return "";
else {
net = long2ip(ip);
net += " - ";
net += long2ip(broadcast);
}
return net;
}

string unfuck_netnumber(string fucked) {
string IPstart=php_strtok(fucked,"-");
u_long ip_start=ip2long(IPstart);
string IPstop=fucked;
u_long ip_stop=ip2long(IPstop);
int ip_nmb=ip_stop-ip_start;
if (ip_nmb>256 && ((ip_nmb/256)-1)%2!=0 && ((((ip_stop+255)-ip_start)/256)-1)%2!=0) return IPstart + "-" + long2ip((long)(double)(ip_stop+255));
return IPstart + "-" + IPstop;
}

string give_ip(string ip_s, string ip_e) {
u_long n_ip_s = ip2long(ip_s);
u_long n_ip_e = ip2long(ip_e);
u_long numIPs = n_ip_e-n_ip_s;
srand((unsigned)time(0));
rand();
return long2ip((long)((double)(numIPs+1)*rand()/(RAND_MAX+1.0))+n_ip_s);
}

