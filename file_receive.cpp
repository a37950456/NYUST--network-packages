//  file_receive.cpp
// 配合  file_send.cpp
#include "stdio.h"
#include "stdlib.h"  //for system("pause");
#include "winsock2.h"
#include "ws2tcpip.h"
#include <iostream>
#include <fstream>
using namespace std;
#pragma comment(lib, "ws2_32.lib")
int server_process();

#define SERVER_PORT  5566
#define BUFLEN       1024

SOCKET    sockfd;
struct    sockaddr_in   client_addr, server_addr;
char op_file[] = "source_copy.pdf";//改成 目地檔 的檔名

int main(int argc, char * argv[])
{
	WSADATA wsa;

	// 啟動 WinSock
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) 
	{
		printf("server: WSAStartup failure !\n");
		return -1;
	}

	//  建立一個 TCP 通訊端
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) 
	{
		printf("server: can't open a TCP socket !\n");
		return -1;
	}

	//  server資訊
	server_addr.sin_family = AF_INET;  // address family
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // ip address
	server_addr.sin_port = htons(SERVER_PORT);       // port

	//  將 sockfd 綁定到這個 server
	if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0) 
	{
		printf("server: can't bind local address !\n");
		return -1;
	}

	// 透過 sockfd, 建立傾聽連結
	if (listen(sockfd, SOMAXCONN) < 0) 
	{
		printf("server: can't listen !\n");
		return -1;
	}

	//執行 server 工作
	server_process();

	//關閉通訊端
	closesocket(sockfd);

	//** 終止 WinSock
	WSACleanup();

	system("pause");

	return 0;
}


int server_process()
{
	int    n;
	struct sockaddr_in  client_addr;
	int    len = sizeof(client_addr);
	SOCKET newSock;
	ofstream output;
	char   x[BUFLEN];
	double count = 0;

	output.open(op_file,ios::out|ios::binary);

	//  等待 TCP client 要求連結 (connect)
	printf("接受連線中...\n");
	newSock = accept(sockfd, (struct sockaddr *)&client_addr, &len);
	if (newSock == INVALID_SOCKET) 
	{
		printf("server: accept error !\n");
		return 1; // 離開第一層迴圈
	}
	else
		printf("檔案接收中...\n");

	while (1) 
	{
		//  根據 newSock 接收資料
		memset(x, 0, BUFLEN);  //清空變數
		n = recv(newSock, x, BUFLEN, 0);
		if (n == 0) // 對方 client 關閉這個 socket
		{  
			printf("%s connection closed... \n", inet_ntoa(client_addr.sin_addr)); 
			break; // 離開第二層迴圈
		}
		if (n > 0) // 資料接收
		{ 
			output.write((char *) x, sizeof(char)*n);
			count+=n;
		}
	}
	printf("成功接收...(檔案大小 %.0f bytes) \n", count);
	output.close();

	closesocket(newSock); // 關閉與這個 client 的連結
	return 0;
}

