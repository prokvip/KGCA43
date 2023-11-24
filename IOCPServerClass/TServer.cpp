#include "TServer.h"
#include "TLog.h"

DWORD WINAPI WorkerAcceptThread(LPVOID param)
{
    //SOCKADDR_IN clientaddr;
    //while (1)
    //{
    //    int addlen = sizeof(clientaddr);
    //    SOCKET clientsock = accept(listensock, (SOCKADDR*)&clientaddr, &addlen);
    //    if (clientsock == SOCKET_ERROR)
    //    {
    //        int iError = WSAGetLastError();
    //        if (iError != WSAEWOULDBLOCK)
    //        {
    //            break;
    //        }
    //    }
    //    else
    //    {
    //        TUser* user = new TUser(clientsock, clientaddr);
    //        user->bind(g_hIOCP);
    //        user->recv();
    //        g_userlist.push_back(user);

    //        PrintA("클라이언트 접속 ip=%s, Port:%d\n",
    //            inet_ntoa(clientaddr.sin_addr),
    //            ntohs(clientaddr.sin_port));


    //    }
    //}
    return 1;
}

bool	TServer::Init()
{   
    // 1) 윈속 초기화
    WSADATA wsa;
    int iRet = 0;
    iRet = WSAStartup(MAKEWORD(2, 2), &wsa);
    if (iRet != 0)
    {
        TLog::Get().log("WSAStartup Failed!");
        return false;
    }
    LogDetailA("WSAStartup");
    listensock = socket(AF_INET, SOCK_STREAM, 0);

    SOCKADDR_IN sa;
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(INADDR_ANY);// inet_addr("192.168.0.12");
    sa.sin_port = htons(10000);

    iRet = bind(listensock, (SOCKADDR*)&sa, sizeof(sa));
    if (iRet == SOCKET_ERROR) return false;
    LogDetailA("bind");
    iRet = listen(listensock, SOMAXCONN);
    if (iRet == SOCKET_ERROR) return  false;
    LogDetailA("listen");
    m_Iocp.Init();
    LogDetailA("m_Iocp.Init()");
    return true;
}

bool	TServer::Release()
{    
    /*WaitForMultipleObjects(MAX_WORKER_THREAD, g_hWorkerThread, TRUE, INFINITE);
    CloseHandle(g_hKillEvent);
    for (int iThread = 0; iThread < MAX_WORKER_THREAD; iThread++)
    {
        CloseHandle(g_hWorkerThread[iThread]);
    }*/
    return true;
}
int TServer::SendPacket(TUser * pUser, UPACKET & packet)
{
    char* sendbuffer = (char*)&packet;

    pUser->wsaSendBuffer.buf = (char*)&packet;
    pUser->wsaSendBuffer.len = packet.ph.len;

    TOV* tov = new TOV(TOV::MODE_SEND);   

    int iSendByte = 0;
    int iTotalSendByte = 0;
    DWORD dwSendByte;
    int iRet = WSASend(pUser->sock, &pUser->wsaSendBuffer, 1,
        &dwSendByte, 0, (LPOVERLAPPED)tov, NULL);
    if (iRet == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSA_IO_PENDING)
        {
            return -1;
        }
    }
    return packet.ph.len;
}

bool TServer::Broadcastting(UPACKET& packet)
{
    /*for (std::list<TUser*>::iterator iterSend = g_userlist.begin();
        iterSend != g_userlist.end();
        iterSend++)
    {
        TUser* pUser = (*iterSend);
        if (pUser->bConneted == false) continue;
        int iSendByte = SendPacket(pUser, packet);
        if (iSendByte == SOCKET_ERROR)
        {
            pUser->bConneted = false;
            continue;
        }
    }*/
    return true;
}

bool TServer::Run()
{
    //for (std::list<TUser*>::iterator iterSend = g_userlist.begin();
    //    iterSend != g_userlist.end();
    //    iterSend++)
    //{
    //    TUser* pUser = (*iterSend);
    //    if (pUser->bConneted == false) continue;
    //    for (auto& data : pUser->list)
    //    {
    //        if (!Broadcastting(data))
    //        {
    //            pUser->bConneted = false;
    //        }
    //    }
    //    pUser->list.clear();
    //}

    ////post
    //for (std::list<TUser*>::iterator iterSend = g_userlist.begin();
    //    iterSend != g_userlist.end();
    //    )
    //{
    //    TUser* pUser = (*iterSend);
    //    if (pUser->bConneted == false)
    //    {
    //        pUser->Close();
    //        iterSend = g_userlist.erase(iterSend);
    //    }
    //    else
    //    {
    //        iterSend++;
    //    }
    //}
    return true;
}

