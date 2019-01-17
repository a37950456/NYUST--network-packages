//blocking_server.cpp
#include<iostream>
using namespace std;
#include<winsock2.h>
#include<stdlib.h>
#pragma comment(lib, "ws2_32.lib")


#define SERVER_EXIT_OK              0
#define SERVER_DLL_ERROR       1
#define SERVER_API_ERROR        2
#define SERVERPORT                    5555
#define MAX_NUM_BUF                 64

// 變數
char bufRecv[MAX_NUM_BUF];
char bufSend[MAX_NUM_BUF];
SOCKET sServer;
SOCKET sClient;
BOOL bConning;
//函數
void InitMember(void);
int ExitClient(int nExit);
BOOL RecvLine(SOCKET s, char* buf);
BOOL SendLine(SOCKET s, char*buf);
int HandleSocketError(char *str);
void ShowSocketMsg(char* str);
//主函數
int main(int argc, char* argv[])
{
	InitMember();

	WSADATA wsaData;   //Windos socket DLL
	int retVal;
	//初始化Socket  DLL
	retVal = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (0!= retVal)
		{
			ShowSocketMsg("Can’t find a usable Windows Sockets DLL!");
			return SERVER_DLL_ERROR;
		}
	//建立ISocket
	sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sServer)
	{
		return HandleSocketError("Failed socket()!");
	}
	//伺服器socket位址
	SOCKADDR_IN addrServ;
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(SERVERPORT);
	addrServ.sin_addr.s_addr = INADDR_ANY;
	//綁定Socket
	retVal = bind(sServer, (LPSOCKADDR)&addrServ, sizeof(SOCKADDR_IN));
	if (SOCKET_ERROR == retVal)
	{
		closesocket(sServer);  //關閉Socket 
		return HandleSocketError("Failed bind()!");
	}
	//開始監聽
	retVal = listen(sServer, 1);
	if (SOCKET_ERROR == retVal)
	{
		closesocket(sServer);  //關閉Socket
		return HandleSocketError("Failed listen()!");
	}
	//等待用戶端的連接
	cout << "Server succeeded!!" << endl;
	cout << "Wait for new clients⋯" << endl;
	//接受用戶端請求
	sockaddr_in addrClient;
	int addrClientlen = sizeof(addrClient);
	sClient = accept(sServer, (sockaddr *FAR)&addrClient, &addrClientlen);
	if (INVALID_SOCKET == sClient)
	{
		closesocket(sServer);
		return HandleSocketError("Failed accept()!");
	}
	else
	{
		bConning = true;
	}
	//顯示用戶端的IP和埠
	char *pClientIP = inet_ntoa(addrClient.sin_addr);
	u_short clientPort = ntohs(addrClient.sin_port);
	cout << "Accept a client." << endl;
	cout << "IP: " << pClientIP << endl;
	cout << "Port:" << clientPort << endl;
	//接收用戶端資料
	if (!RecvLine(sClient, bufRecv))
	{
		return ExitClient(SERVER_API_ERROR);
	}
	//顯示用戶端資料
	cout << bufRecv << endl;
	//向用戶端發送資料
	strcpy(bufSend, "Hello, Client!\n");
	if (!SendLine(sClient, bufSend))
	{
		return ExitClient(SERVER_API_ERROR);
	}
	//顯示退出資訊
	cout << "Server exigent⋯" << endl;
	system("pause");
	//退出
	return ExitClient(SERVER_EXIT_OK);
}
//初始化成員變數
void InitMember(void)
{
	//初始化讀和寫緩衝區
	memset(bufRecv, 0, MAX_NUM_BUF);
	memset(bufSend, 0, MAX_NUM_BUF);
	//初始化
	sServer = INVALID_SOCKET;
	sClient = INVALID_SOCKET;
	//沒有連接狀態
	bConning = FALSE;
}
//退出
int ExitClient(int nExit)
{
	closesocket(sServer);
	closesocket(sClient);
	WSACleanup();
	return nExit;
}
//讀一行資料
BOOL RecvLine(SOCKET s, char* buf)
{
	BOOL retVal = TRUE;
	BOOL bLineEnd = FALSE;
	int nReadLen = 0;
	int nDataLen = 0;
	while (!bLineEnd && bConning)
	{
		nReadLen = recv(s, buf + nDataLen, 1, 0);
		//錯誤處理
		if (SOCKET_ERROR == nReadLen)
		{
			int nErrCode = WSAGetLastError();
			if (WSAENOTCONN == nErrCode)
			{
				ShowSocketMsg("The socket is not connected!");
			}
			else if (WSAESHUTDOWN == nErrCode)
			{
				ShowSocketMsg("The socket has been shut down!");
			}
			else if (WSAETIMEDOUT == nErrCode)
			{
				ShowSocketMsg("The socket has been dropped!");
			}
			else if (WSAECONNRESET == nErrCode)
			{
				ShowSocketMsg("The virtual circuit was reset by remote side!");
			}
			else {}
			retVal = false;
			break;
		}
		if (0 == nReadLen) //用戶端關閉
		{
			retVal = FALSE;
			break;
		}
		//讀入數據
		if ('\n' == *(buf + nDataLen))  //
		{
			bLineEnd = TRUE;
		}
		else {
			nDataLen += nReadLen;
		}
	}
	return retVal;
}
//發送一行資料
BOOL SendLine(SOCKET s, char* str)
{
	int retVal;
	retVal = send(s, str, strlen(str), 0);
	//錯誤處理
	if (SOCKET_ERROR == retVal)
	{
		int nErrCode = WSAGetLastError();
		if (WSAENOTCONN == nErrCode)
		{
			ShowSocketMsg("The socket is not connected!");
		}
		else if (WSAESHUTDOWN == nErrCode)
		{
			ShowSocketMsg("The socket has been shut down!");
		}
		else if (WSAETIMEDOUT == nErrCode)
		{
			ShowSocketMsg("The socket has been dropped!");
		}
		else if (WSAECONNRESET == nErrCode)
		{
			ShowSocketMsg("The virtual circuit was reset by remote side!");
		}
		else {}
		retVal = false;
	}
	return true;
}

//錯誤處理
int   HandleSocketError(char *str)
{
	ShowSocketMsg(str);
	WSACleanup();
	return SERVER_API_ERROR;
}
//顯示錯誤 
void ShowSocketMsg(char* str)
{
	MessageBox(NULL, (WCHAR *)str, (WCHAR *)"SERVER ERROR", MB_OK);
}