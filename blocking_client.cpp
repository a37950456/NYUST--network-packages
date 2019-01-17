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

// �ܼ�
char bufRecv[MAX_NUM_BUF];
char bufSend[MAX_NUM_BUF];
SOCKET sHost;
BOOL bConning;
//���
void InitMember(void);
int ExitClient(int nExit);
BOOL RecvLine(SOCKET s, char* buf);
void ShowErrorMsg(void);

//�D���
int main()
{
	//��l���ܼ�
	InitMember();

	WSADATA wsaData;   //Windos socket DLL
	int retVal;
	//��l��Socket  DLL
	retVal = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (0!= retVal)
		{
			MessageBox(NULL, (WCHAR *)"Can��t find a usable Windows Sockets DLL!", (WCHAR *)"ERROR", MB_OK);
			return CLIENT_DLL_ERROR;
		}
	//�إ�Windows Socket
	sHost = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sHost)
	{
		ShowErrorMsg();
		WSACleanup();
		return CLIENT_API_ERROR;
	}
	//�ǳƳs�����A��
	cout << "Client succeeded!!" << endl;
	cout << "Be ready to connect to server�K" << endl;
	//����D������T
	LPHOSTENT hostEntry;
	char hostname[MAX_NUM_BUF];
	gethostname(hostname, MAX_NUM_BUF);
	hostEntry = gethostbyname(hostname);
	if (!hostEntry)
	{
		ShowErrorMsg();
		return ExitClient(CLIENT_API_ERROR);
	}

	//�]�msocketaddr_in
	SOCKADDR_IN addrServ;
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(SERVERPORT);
	addrServ.sin_addr = *((LPIN_ADDR)*hostEntry->h_addr_list);
	//�s�����A��
	retVal = connect(sHost, (LPSOCKADDR)&addrServ, sizeof(SOCKADDR_IN));
	if (SOCKET_ERROR == retVal)
	{
		ShowErrorMsg();
		return ExitClient(CLIENT_API_ERROR);
	}
	else {
		bConning = true;
	}

	//�s�u���A�����\
	cout << "Connect successfully!!" << endl;
	//�V���A���o�e���
	strcpy(bufSend, "Hello, Server!\n");
	retVal = send(sHost, bufSend, strlen(bufSend), 0);
	if (SOCKET_ERROR == retVal)
	{
		ShowErrorMsg();
		return ExitClient(CLIENT_API_ERROR);
	}
	//�q���A���ݱ������
	if (!RecvLine(sHost, bufRecv))
	{
		ShowErrorMsg();
		return ExitClient(CLIENT_API_ERROR);
	}

	//��ܦ��A������
	cout << bufRecv << endl;
	system("pause");
	//�h�X
	return ExitClient(CLIENT_EXIT_OK);
}

//��ܿ��~��T
void ShowErrorMsg(void)
{
	int nErrCode = WSAGetLastError();
	HLOCAL hlocal = NULL;
	//�I�s FormatMessage()�禡 �H������~���奻�r��
	BOOL fOk = FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
		NULL, nErrCode, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
		(PTSTR)&hlocal, 0, NULL);

	//��ܿ��~��T
	if (hlocal != NULL)
	{
		MessageBox(NULL, (WCHAR *)LocalLock(hlocal), (WCHAR *)"CLIENT ERROR", MB_OK);
		LocalFree(hlocal);
	}
}


//��l�Ʀ����ܼ�
void InitMember(void)
{
	//��l��Ū�M�g�w�İ�
	memset(bufRecv, 0, MAX_NUM_BUF);
	memset(bufSend, 0, MAX_NUM_BUF);
	//��l��
	sHost = INVALID_SOCKET;
	//�S���s�����A
	bConning = FALSE;
}
//�h�X
int ExitClient(int nExit)
{
	closesocket(sHost);
	WSACleanup();
//��ܰh�X��T
cout << "Client exiting�K" << endl;
Sleep(20000);
return nExit;
}
//Ū�@����
BOOL RecvLine(SOCKET s, char* buf)
{
	BOOL retVal = TRUE;
	BOOL bLineEnd = FALSE;
	int nReadLen = 0;
	int nDataLen = 0;
	while (!bLineEnd && bConning)
	{
		nReadLen = recv(s, buf + nDataLen, 1, 0);
		//���~�B�z
		if (SOCKET_ERROR == nReadLen)
		{
			retVal = FALSE;
			break;
		}
		if (0 == nReadLen) //�Τ������
		{
			retVal = FALSE;
			break;
		}
		//Ū�J�ƾ�
		if ('\n' == *(buf + nDataLen))  //����
		{
			bLineEnd = TRUE;
		}
		else {
			nDataLen += nReadLen;
		}
	}
	return retVal;
}