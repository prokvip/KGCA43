#include "TServer.h"
bool	TServer::Init()
{
    return true;
}

bool	TServer::Relese()
{
    return true;
}
void TServer::Run()
{
    //PreProcess(); // 패킷처리
    //PostProcess(); // 전송처리 및 정리
    WaitForMultipleObjects(MAX_WORKER_THREAD, g_hWorkerThread, TRUE, INFINITE);
    CloseHandle(g_hKillEvent);
    for (int iThread = 0; iThread < MAX_WORKER_THREAD; iThread++)
    {
        CloseHandle(g_hWorkerThread[iThread]);
    }
    return true;
}
int SendPacket(TUser * pUser, UPACKET & packet)
{
    char* sendbuffer = (char*)&packet;

    pUser->wsaSendBuffer.buf = (char*)&packet;
    pUser->wsaSendBuffer.len = packet.ph.len;
    pUser->ovSend.flag = OVERLAPPED2::MODE_SEND;

    int iSendByte = 0;
    int iTotalSendByte = 0;
    DWORD dwSendByte;
    int iRet = WSASend(pUser->sock, &pUser->wsaSendBuffer, 1,
        &dwSendByte, 0, (LPOVERLAPPED)&pUser->ovSend, NULL);
    if (iRet == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSA_IO_PENDING)
        {
            return -1;
        }
    }
    return packet.ph.len;
}

bool Broadcastting(UPACKET& packet)
{
    for (std::list<TUser*>::iterator iterSend = g_userlist.begin();
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
    }
    return true;
}

void Run()
{
    for (std::list<TUser*>::iterator iterSend = g_userlist.begin();
        iterSend != g_userlist.end();
        iterSend++)
    {
        TUser* pUser = (*iterSend);
        if (pUser->bConneted == false) continue;
        for (auto& data : pUser->list)
        {
            if (!Broadcastting(data))
            {
                pUser->bConneted = false;
            }
        }
        pUser->list.clear();
    }

    //post
    for (std::list<TUser*>::iterator iterSend = g_userlist.begin();
        iterSend != g_userlist.end();
        )
    {
        TUser* pUser = (*iterSend);
        if (pUser->bConneted == false)
        {
            pUser->Close();
            iterSend = g_userlist.erase(iterSend);
        }
        else
        {
            iterSend++;
        }
    }
}

DWORD WINAPI WorkerAcceptThread(LPVOID param)
{
    SOCKADDR_IN clientaddr;
    while (1)
    {
        int addlen = sizeof(clientaddr);
        SOCKET clientsock = accept(listensock, (SOCKADDR*)&clientaddr, &addlen);
        if (clientsock == SOCKET_ERROR)
        {
            int iError = WSAGetLastError();
            if (iError != WSAEWOULDBLOCK)
            {
                break;
            }
        }
        else
        {
            TUser* user = new TUser(clientsock, clientaddr);
            user->bind(g_hIOCP);
            user->recv();
            g_userlist.push_back(user);

            PrintA("클라이언트 접속 ip=%s, Port:%d\n",
                inet_ntoa(clientaddr.sin_addr),
                ntohs(clientaddr.sin_port));


        }
    }
    return 1;
}