//get peername_server.cpp
#define BUF_SIZE 64
#define PORT 4999
#include "stdio.h"
#include "stdlib.h"
#include "winsock2.h"
#include "ws2tcpip.h"
#pragma comment(lib, "ws2_32.lib")
using namespace std;

int main(int argc, char* argv[])
{
	WSADATA wsd;        //WSADARA變數
	SOCKET sServer;    //伺服器Socket
	SOCKET sClient;     //客戶端Socket
	SOCKADDR_IN  addrServ; // IPv4 伺服器位址
	SOCKADDR_IN  addrClient; //IPv4 客戶端位址
	int   retVal;    //返回值

					 //初始化Socket  DLL
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		printf("WSAStartup failed!\n");
		return 1;
	}

	//建立Internet Socket
	sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sServer)
	{
		printf("socket failed!\n");
		WSACleanup();  //釋放Socket資源
		return -1;
	}

	//設置伺服器位址
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(PORT);
	addrServ.sin_addr.s_addr = INADDR_ANY;
	//綁定Socket
	retVal = bind(sServer, (LPSOCKADDR)&addrServ, sizeof(SOCKADDR_IN));
	if (SOCKET_ERROR == retVal)
	{
		printf("bind failed!\n");
		closesocket(sServer);  //關閉Socket 
		WSACleanup;            //釋放Socket資源
		return -1;
	}
	//開始監聽
	retVal = listen(sServer, 1);
	if (SOCKET_ERROR == retVal)
	{
		printf("listen failed!\n");
		closesocket(sServer);  //關閉Socket 
		WSACleanup;             //釋放Socket資源
		return -1;
	}
	//接受IPv4 客戶端請求
	int addrClientlen = sizeof(addrClient);
	sClient = accept(sServer, (SOCKADDR FAR*)&addrClient, &addrClientlen);
	if (INVALID_SOCKET == sClient)
	{
		printf("accept failed!\n");
		closesocket(sServer); //關閉Socket
		WSACleanup(); //釋放Socket資源;
		return -1;
	}
	printf("(Using addrClient returned from accept())\npeer IP address: %s Peer port: %d\n", inet_ntoa(addrClient.sin_addr), ntohs(addrClient.sin_port));
	struct sockaddr_storage addr_remote, addr_local;
	char ipstr[INET6_ADDRSTRLEN];//INET6_ADDRSTRLEN = 46
								 //IPv6 address (text presentation) (bytes)
	int len_remote, len_local, port_remote, port_local;

	len_remote = sizeof addr_remote;
	len_local = sizeof addr_local;
	getpeername(sClient, (struct sockaddr*) &addr_remote, &len_remote);
	getsockname(sClient, (struct sockaddr*) &addr_local, &len_local);
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
	strcpy(buf, "~~Hello from Server. ~~");
	send(sClient, buf, strlen(buf), 0);

	system("pause");
	//退出
	closesocket(sServer);  //關閉Socket
	closesocket(sClient);  //關閉Socket
	WSACleanup();         //釋放Socket資源
	return 0;


}