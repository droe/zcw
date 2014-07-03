
/*

 includes/do_whois.hpp
   
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

string do_whois(string whois, string whois_q) {

	string cyberabuse_web="www.cyberabuse.org";
	string abusedotnet="whois.abuse.net";

        int s;
        const char *host=whois.c_str();
        FILE *sfi, *sfo;
        char buf[1024];
        size_t len;
        string result;
        string tmp;
        string random_ip;

        struct sockaddr_in server;
        struct sockaddr_in local;
        struct hostent *hostinfo;

        int port = 43;

        if(whois==cyberabuse_web) {
                port=80;
		whois_q="GET http://"+whois+whois_q+" HTTP/1.0\n\n";
        }

	if(whois_q=="") return "";
	if(whois=="whois.nic.ad.jp") whois_q+="/e";
	if(whois=="whois.ripe.net" && is_ip(whois_q)) whois_q="-B "+whois_q;
	else if(whois=="whois.ripe.net") whois_q="-r "+whois_q;

/*

*** If you have a special authorization from RIPE for unlimited queries (proxy) uncomment this.

	string remote_ip="";
	if(whois=="whois.ripe.net") {
		if(getenv("TCPREMOTEIP")) remote_ip=string(getenv("TCPREMOTEIP"));
		if(remote_ip!="") whois_q="-VCyberAbuse_Whois_Client,"+remote_ip+" "+whois_q;
		random_ip="x.x.x.x";
	}

*/

	if((string)DEBUG=="OK") cout << "Query : " << whois_q << STRRET << "Server : " << whois << endl;

        const char *query=whois_q.c_str();

        if ((hostinfo = gethostbyname(host)) == NULL) showerror(1);

	memset((char *) &local, 0, sizeof(struct sockaddr));

	if(random_ip=="") random_ip = give_ip(IP_START,IP_END);
	local.sin_family = AF_INET;
	local.sin_port = htons(0);
	local.sin_addr.s_addr = inet_addr(random_ip.c_str());

        memset((char *) &server, 0, sizeof(struct sockaddr));
        memcpy((char *) &server.sin_addr, hostinfo->h_addr, hostinfo->h_length);

        server.sin_family = AF_INET;
        server.sin_port = htons(port);

	if(signal(SIGALRM,sigalrm_handler)==SIG_IGN) signal(SIGALRM,sigalrm_handler);
	alarm(ALARM_TIMEOUT_WHOIS);

        if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) showerror(1);
	if(bind(s, (struct sockaddr *) &local, sizeof(struct sockaddr)) < 0) showerror(1);
        if (connect(s, (struct sockaddr *) &server, sizeof(struct sockaddr)) < 0) showerror(1);

        sfi = fdopen(s, "r");
        sfo = fdopen(s, "w");

        if (sfi == NULL || sfo == NULL) showerror(1);

        fprintf(sfo, "%s\r\n", query);
        fflush(sfo);

        while(fgets(buf, 1024, sfi)) {
                len = strlen(buf);
		while (len && isspace((int)buf[len-1])) buf[--len] = '\0';
		tmp = php_trim(string(buf));
		if (tmp!="") {
                        if(whois==cyberabuse_web) { result = tmp + STRRET; }
                        else {
				if(whois==abusedotnet) {
					if(tmp.substr((tmp.size()-18),18)!="(default, no info)") {
						result += getmail(tmp)+",";
						if(php_strstr(tmp,"abuse@")!="" || php_strstr(tmp,"security")!="" || php_strstr(tmp,"cert")!="") return getmail(tmp);
					}
				} else {
					if(whois=="whois.arin.net" && whois_q.substr(0,2)=="o " && tmp.substr(0,7)=="Updated") return result;
					result += tmp + STRRET;
				}
			}
		}
        }

if(whois==abusedotnet) return result.substr(0,(result.size()-1));

return result;

}

