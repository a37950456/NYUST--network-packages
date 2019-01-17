#include "stdafx.h"
//getsockname_server.cpp
#define BUF_SIZE 64
#define PORT 4999
#include "stdio.h"
#include "stdlib.h"
#include "winsock2.h"
#pragma comment(lib, "ws2_32.lib")

int main(int argc, char* argv[])
{
	WSADATA wsd;        //WSADARA變數
	SOCKET sServer;    //伺服器Socket
	SOCKET sClient;     //客戶端Socket
	SOCKADDR_IN  addrServ; // IPv4 伺服器位址
	SOCKADDR_IN  addrClient; //IPv4 客戶端位址
	char   buf[BUF_SIZE];   //接收資料緩衝區
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
	//Receives the address (name) of the socket sServer
	struct sockaddr_in local;
	int saSize = sizeof(local);//siszeof(struct sockaddr);
	memset(&local, 0, sizeof(local));
	getsockname(sServer, (struct sockaddr*)&local, &saSize);
	printf("The address of the listening socket : &s : %d\n", inet_ntoa(local.sin_addr), ntohs(local.sin_port));

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
	//Receives the address (name) of the socket sClient
	memset(&local, 0, sizeof(local));
	getsockname(sClient, (struct sockaddr *)&local, &saSize);
	printf("The address of the accepted socket : &s : %d\n", inet_ntoa(local.sin_addr), ntohs(local.sin_port));
	system("pause");

	//退出
	closesocket(sServer);  //關閉Socket
	closesocket(sClient);  //關閉Socket
	WSACleanup();         //釋放Socket資源
	return 0;


}