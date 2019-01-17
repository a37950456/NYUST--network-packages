//blocking_client.cpp
#include<iostream>
using namespace std;
#include<winsock2.h>
#include<stdlib.h>
#pragma comment(lib, "ws2_32.lib")


#define CLIENT_EXIT_OK         0
#define CLIENT_DLL_ERROR   1
#define CLIENT_API_ERROR    2
#define SERVERPORT              5555
#define MAX_NUM_BUF           64

// 變數
char bufRecv[MAX_NUM_BUF];
char bufSend[MAX_NUM_BUF];
SOCKET sHost;
BOOL bConning;
//函數
void InitMember(void);
int ExitClient(int nExit);
BOOL RecvLine(SOCKET s, char* buf);
void ShowErrorMsg(void);

//主函數
int main()
{
	//初始化變數
	InitMember();

	WSADATA wsaData;   //Windos socket DLL
	int retVal;
	//初始化Socket  DLL
	retVal = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (0!= retVal)
		{
			MessageBox(NULL, (WCHAR *)"Can’t find a usable Windows Sockets DLL!", (WCHAR *)"ERROR", MB_OK);
			return CLIENT_DLL_ERROR;
		}
	//建立Windows Socket
	sHost = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sHost)
	{
		ShowErrorMsg();
		WSACleanup();
		return CLIENT_API_ERROR;
	}
	//準備連接伺服器
	cout << "Client succeeded!!" << endl;
	cout << "Be ready to connect to server…" << endl;
	//獲取主機的資訊
	LPHOSTENT hostEntry;
	char hostname[MAX_NUM_BUF];
	gethostname(hostname, MAX_NUM_BUF);
	hostEntry = gethostbyname(hostname);
	if (!hostEntry)
	{
		ShowErrorMsg();
		return ExitClient(CLIENT_API_ERROR);
	}

	//設置socketaddr_in
	SOCKADDR_IN addrServ;
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(SERVERPORT);
	addrServ.sin_addr = *((LPIN_ADDR)*hostEntry->h_addr_list);
	//連接伺服器
	retVal = connect(sHost, (LPSOCKADDR)&addrServ, sizeof(SOCKADDR_IN));
	if (SOCKET_ERROR == retVal)
	{
		ShowErrorMsg();
		return ExitClient(CLIENT_API_ERROR);
	}
	else {
		bConning = true;
	}

	//連線伺服器成功
	cout << "Connect successfully!!" << endl;
	//向伺服器發送資料
	strcpy(bufSend, "Hello, Server!\n");
	retVal = send(sHost, bufSend, strlen(bufSend), 0);
	if (SOCKET_ERROR == retVal)
	{
		ShowErrorMsg();
		return ExitClient(CLIENT_API_ERROR);
	}
	//從伺服器端接收資料
	if (!RecvLine(sHost, bufRecv))
	{
		ShowErrorMsg();
		return ExitClient(CLIENT_API_ERROR);
	}

	//顯示伺服器應答
	cout << bufRecv << endl;
	system("pause");
	//退出
	return ExitClient(CLIENT_EXIT_OK);
}

//顯示錯誤資訊
void ShowErrorMsg(void)
{
	int nErrCode = WSAGetLastError();
	HLOCAL hlocal = NULL;
	//呼叫 FormatMessage()函式 以獲取錯誤的文本字串
	BOOL fOk = FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
		NULL, nErrCode, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
		(PTSTR)&hlocal, 0, NULL);

	//顯示錯誤資訊
	if (hlocal != NULL)
	{
		MessageBox(NULL, (WCHAR *)LocalLock(hlocal), (WCHAR *)"CLIENT ERROR", MB_OK);
		LocalFree(hlocal);
	}
}


//初始化成員變數
void InitMember(void)
{
	//初始化讀和寫緩衝區
	memset(bufRecv, 0, MAX_NUM_BUF);
	memset(bufSend, 0, MAX_NUM_BUF);
	//初始化
	sHost = INVALID_SOCKET;
	//沒有連接狀態
	bConning = FALSE;
}
//退出
int ExitClient(int nExit)
{
	closesocket(sHost);
	WSACleanup();
//顯示退出資訊
cout << "Client exiting…" << endl;
Sleep(20000);
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
			retVal = FALSE;
			break;
		}
		if (0 == nReadLen) //用戶端關閉
		{
			retVal = FALSE;
			break;
		}
		//讀入數據
		if ('\n' == *(buf + nDataLen))  //分行
		{
			bLineEnd = TRUE;
		}
		else {
			nDataLen += nReadLen;
		}
	}
	return retVal;
}