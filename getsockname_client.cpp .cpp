#include "stdafx.h"
//getsockname_client.cpp
#define BUF_SIZE 64
#define PORT 4999
#define IP "140.123.46.12"   //127.0.0.1   //server lP (local host)
#include "stdio.h"
#include "stdlib.h"  //for system("pause");
#include "winsock2.h"
#pragma comment(lib, "ws2_32.lib")

int main(int argc, char* argv[])
{
	WSADATA wsd;        //WSADARA變數
	SOCKET sHost;    //伺服器Socket
	SOCKADDR_IN  servAddr;  //IPv4 伺服器位址
	char   buf[BUF_SIZE];   //接收資料緩衝區
	int   retVal;    //返回值

					 //初始化Socket  DLL
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		printf("WSAStartup failed!\n");
		return -1;
	}
	//建立Internet Socket
	sHost = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sHost)
	{
		printf("socket failed!\n");
		WSACleanup();  //釋放Socket資源
		return -1;
	}
	//設置IPv4 伺服器位址
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(IP);
	servAddr.sin_port = htons(PORT);
	int nServAddlen = sizeof(servAddr);
	//連接伺服器
	retVal = connect(sHost, (LPSOCKADDR)&servAddr, sizeof(servAddr));

	//Receives the address (name) of the socket sHost
	struct sockaddr_in local;
	int saSize = sizeof(local); //siszeof(struct sockaddr);
	memset(&local, 0, sizeof(local));
	getsockname(sHost, (struct sockaddr *)&local, &saSize);
	printf("The address of the connecting socket : &s : %d\n", inet_ntoa(local.sin_addr), ntohs(local.sin_port));

	system("pause");
	//退出
	closesocket(sHost);  //關閉Socket
	WSACleanup();         //釋放Socket資源
	return 0;
}