//nonblocking_Server.cpp
#include <iosteram>
using namespace std;
#include "winsock2.h"
#include <list>
#include "Client.h"
#pragma comment(lib,"ws2_32.lib")
using namespace std;
typedef list<CClient*>CLIENTLIST;  //鏈表 list

#define SERVERPORT          5556
#define SERVER_SETUP_FALL   1

#define TIMEFOR_THREAD_EXIT    5000 //主執行緒睡眠時間
#define TIMEFOR_THREAD_HELP    1500
#define TIMEFOR_THREAD_SLEEP   500

HANDLE hThreadAccept;
HANDLE hThreadHelp;
SOCKET sServer;
BOOL bServerRunning;
HANDLE hServerEvent;
CLIENTLIST    clientlist;
CRITICAL_SECTION csClientList;

BOOL InitSever(void);
BOOL StartService(void);
void StopService(void);
BOOL CreateHelperAndAcceptThread(void);
void ExitServer(void);

void InitMember(void);
BOOL InitSocket(void);

void ShowTipMsg(BOOL bFirstInput);
void ShowServiceStartMsg(BOOL bSuc);
void ShowServerExitMsg(void);

DWORD __stdcall HelperThread(void *pParam);
DWORD __stdcall AcceptThread(void *pParam);
void ShowConnectNum();

int main(int argc, char* argv[])
{
    //初始化伺服器
    if(!InitSever())
    {
        ExitServer();
        return SERVER_SETUP_FALL;
    }
    //啟動服務
    if(!StartService())
    {
        ShowServiceStartMsg(FALSE);
        ExitServer();
        return SERVER_SETUP_FALL;
    }
    //停止服務
    StopService();
    
    //伺服器退出
    ExitServer();
    
    return 0;
}
/*
 初始化
 */
BOOL InitSever(void)
{
    InitMember(); //初始化全域變數
    //初始化Socket
    if(!InitSocket())
        return FALSE;
    
    return TRUE;
}
/*
 初始化全域變數
 */
void InitMember(void)
{
    InitializeCriticalSection(&csClientList);
    hServerEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    hThreadAccept = NULL;
    hThreadHelp = NULL;
    sServer = INVALID_SOCKET;
    bServerRunning = FALSE;
    clientlist.clear();
}
/*
 初始化Socket
 */
BOOL InitSocket(void)
{
    //返回值
    int retVal;
    //初始化
    retVal = WSAStartup(MAKEWORD(2, 2), &wsaData);
    //創建通訊端
    sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == sServer)
    {
        return FALSE;    }
    //設置通訊端非阻塞模式
    unsigned long ul = 1;
    reVal = ioctlsocket(sServer, FIONBIO, (unsigned long*)&ul);
    if(SOCKET_ERROR == reVal)
        return FALSE;
    //綁定通訊端
    sockaddr_in serAddr;
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(SERVERPORT);
    serAddr.sin_addr.S_un.S_addr = INADDR_ANY;
    reVal = bind(sServer,(struct sockadr*)&serAddr, sizeof(serAddr));
    if(SOCKET_ERROR== retVal)
        return FALSE;
    //監聽
    reVal = listen(sServer, SOMAXCONN);
    if(SOCKET_ERROR == reVal)
        return FALSE;
    
    return TRUE;
}





