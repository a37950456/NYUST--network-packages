//  file_send.cpp
// 配合  file_receive.cpp 
#include <iostream>
#include <string>
#include <fstream>
using namespace std;
#include "stdio.h"
#include "stdlib.h"  //for system("pause");
#include "winsock2.h"
#include "ws2tcpip.h"
#pragma comment(lib, "ws2_32.lib")
bool init_tcp();
bool send_to_server(char *);

# define SERVER_ADDR  "127.0.0.1"
# define SERVER_PORT  5566
# define BUFLEN       1024

SOCKET    sockfd;
struct    sockaddr_in   client_addr, server_addr;
char ip_file[] = "source.pdf";//改成 來源檔 的檔名

int main(int argc, char * argv[])
{
	ifstream input;
	char x[BUFLEN];
	int n;
	double count = 0;

	// 初始 TCP
	if (!init_tcp()) return -1;

	printf("已連線至 %s:%d\n", SERVER_ADDR, SERVER_PORT);
	cout << "開始傳送檔案...\n";
	input.open(ip_file,ios::in|ios::binary);
	if(input.fail())
	{
		cout << "檔案無法開啟\n";
		exit(1);
	}

	input.read((char*) x, sizeof(char)*BUFLEN);
	while(!input.eof()) 
	{
		n = send(sockfd, x, BUFLEN, 0);
		count+=n;
		input.read((char*) x, sizeof(char)*BUFLEN);
	}
	input.close();

	//  關閉通訊端
	printf("傳送結束...(檔案大小 %.0f bytes) \n", count);
	closesocket(sockfd);

	//  終止 WinSock
	WSACleanup();

	system("pause");
	return 0;
}

bool init_tcp()
{
	WSADATA wsa;

	// 啟動 WinSock
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		printf("client: WSAStartup failure !\n");
		return false;
	}

	// 將 server 的資訊寫下來
	server_addr.sin_family = AF_INET;  // address family
	server_addr.sin_port = htons(SERVER_PORT);            // port
	server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR); // ip

	// 建立一個 TCP 通訊端
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		printf("client: can't open a TCP socket !");
		return false;
	}

	// 將 sockfd 通訊端連結到 server
	printf("Try to connect to the server...\n");
	while (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0) 
	{
	}

	return true;
}

