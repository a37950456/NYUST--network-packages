// gethostname_gethostbyname.cpp
#include "stdio.h"
#include "stdlib.h"  //for system("pause")
#include "winsock2.h"
#pragma comment(lib, "ws2_32.lib")

int main(int argc, char* argv[])
{
	WSADATA  wsd;   //WSADARA變數
	struct hostent *he;
	char hostname[128];

	//初始化Socket  DLL
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		printf("WSAStartup failed!\n");
		return 1;
	}

	gethostname(hostname, sizeof hostname);
	printf("The hostname : %s\n", hostname);

	if ((he = gethostbyname(hostname)) == NULL)
	{
		DWORD dwError = WSAGetLastError();
		if (dwError != 0) {
			if (dwError == WSAHOST_NOT_FOUND) {
				printf("Host not found\n");
				return 1;
			}
			else if (dwError == WSANO_DATA) {
				printf("No data record found\n");
				return 1;
			}
			else {
				printf("Function failed with error : %ld\n, dwError");
				return 1;
			}
		}
	}
	//印出關於這個host的資訊:
	printf("The hostname : %s\n", he->h_name);
	printf("IP addresses : \n");
	if (he->h_addrtype == AF_INET)
	{
		struct in_addr **addr_list;
		addr_list = (struct in_addr **)he->h_addr_list;
		for (int i = 0; addr_list[i] != NULL; i++)
			printf("\tIP_%d: %s\n", i, inet_ntoa(*addr_list[i]));
	}
	for (int i = 0; he->h_aliases[i] != NULL; i++)
		printf("\tAlternate name #%d : %s\n", i, *he->h_aliases[i]);

	system("pause");
	return 0;

}
