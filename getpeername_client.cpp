﻿//get peername_server.cpp
#define BUF_SIZE 64
#define PORT 4999
#define IP "127.0.0.1"
#include "stdio.h"
#include "stdlib.h"
#include "winsock2.h"
#include "ws2tcpip.h"
#pragma comment(lib, "ws2_32.lib")
using namespace std;

int main(int argc, char* argv[])
{
	WSADATA wsd;        //WSADARA≈‹º∆
	SOCKET sHost;     //伺服器位址
	SOCKADDR_IN  servaddr;
	int   retVal;    //™¶^≠»

					 //™Ï©l§∆Socket  DLL
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		printf("WSAStartup failed!\n");
		return 1;
	}

	//´ÿ•ﬂInternet Socket
	sHost = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sHost)
	{
		printf("socket failed!\n");
		WSACleanup();  //ƒ¿©ÒSocket∏Í∑Ω
		return -1;
	}

	//設置IPv4 伺服器位址
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = inet_addr(IP);

	printf("(Get from servaddr)\nPeer IP address: %s Peer port: %d\n", inet_ntoa(servaddr.sin_addr), ntohs(servaddr.sin_port));

	//連接伺服器
	retVal = connect(sHost, (LPSOCKADDR)&servaddr, sizeof(servaddr));
	if (SOCKET_ERROR == retVal)
	{
		printf("connect failed!\n");
		closesocket(sHost);  //√ˆ≥¨Socket
		WSACleanup();            //ƒ¿©ÒSocket∏Í∑Ω
		return -1;
	}
		struct sockaddr_storage addr_remote, addr_local;
		char ipstr[INET6_ADDRSTRLEN];//INET6_ADDRSTRLEN = 46
									 //IPv6 address (text presentation) (bytes)
		int len_remote, len_local, port_remote, port_local;

		len_remote = sizeof addr_remote;
		len_local = sizeof addr_local;
		getpeername(sHost, (struct sockaddr*) &addr_remote, &len_remote);
		getsockname(sHost, (struct sockaddr*) &addr_local, &len_local);
		if (addr_remote.ss_family == AF_INET)
		{
			struct sockaddr_in *s_remote = (struct sockaddr_in *) &addr_remote;
			port_remote = ntohs(s_remote->sin_port);
			inet_ntop(AF_INET, &s_remote->sin_addr, ipstr, sizeof ipstr);
		}
		else //AF_INET6
		{
			struct sockaddr_in6 *s_remote = (struct sockaddr_in6 *) &addr_remote;
			port_remote = ntohs(s_remote->sin6_port);
			inet_ntop(AF_INET6, &s_remote->sin6_addr, ipstr, sizeof ipstr);
		}
		printf("(Using getpeername())\nPeer IP address : %s", ipstr);
		printf("Peer port : %d \n", port_remote);
		ZeroMemory(ipstr, INET6_ADDRSTRLEN);//memset(ipstr, 0 , INET6_ADDRSTRLEN);
		if (addr_local.ss_family == AF_INET)
		{
			struct sockaddr_in *s_local = (struct sockaddr_in *)&addr_local;
			port_local = ntohs(s_local->sin_port);
			inet_ntop(AF_INET, &s_local->sin_addr, ipstr, sizeof ipstr);
		}
		else //AF_INET6
		{
			struct sockaddr_in6 *s_local = (struct sockaddr_in6 *) &addr_remote;
			port_remote = ntohs(s_local->sin6_port);
			inet_ntop(AF_INET6, &s_local->sin6_addr, ipstr, sizeof ipstr);
		}
		printf("(Using getpeername())\nPeer IP address : %s", ipstr);
		printf("Local port : %d \n", port_remote);

		char buf[64];
		ZeroMemory(buf, 64);
		recv(sHost, buf, 64, 0);
		printf("%s\n", buf);
		system("pause");
		//∞h•X
		closesocket(sHost);  //√ˆ≥¨Socket
		WSACleanup();         //ƒ¿©ÒSocket∏Í∑Ω
		system("pause");
		return 0;

	}
