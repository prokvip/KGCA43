#include "TNetIOCPServer.h"
DWORD  WINAPI  WorkerProc(LPVOID lpThreadParameter)
{
    TNetIOCPServer* iocp = ((TNetIOCPServer*)lpThreadParameter);
    DWORD		dwTransfer;
    ULONG_PTR	CompletionKey;
    tOV  m_ov;
    tOV* lpOverlapped = nullptr;
    TSession* pSession = nullptr;
    while (iocp->m_bRun)
    {
        BOOL ret = GetQueuedCompletionStatus(iocp->g_hIOCP,
            &dwTransfer, &CompletionKey,
            (LPOVERLAPPED*)&lpOverlapped, INFINITE);

        if (ret == TRUE)
        {
            // 어떤 유저의 비동기 작업인지 알수가 있다.
            pSession = (TSession*)CompletionKey;
            if (pSession->Dispatch(dwTransfer, lpOverlapped))
            {
               // iocp->AddPacket(pSession->m_packet);
            }
        }
    }
    return 1;
}

void TNetIOCPServer::CreateServer(std::string ip, USHORT port)
{
    g_hIOCP = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
    DWORD threadID;
    for (int iThread = 0; iThread < MAX_NUM_WORKDER_THREAD; iThread++)
    {
        g_hWorkerThread[iThread] = CreateThread(0, 0, WorkerProc,
            this, 0, &threadID);
    }
    TNetwork::CreateServer(ip, port);
    // iocp
}
void TNetIOCPServer::AddSession(SOCKET sock, SOCKADDR_IN addr)
{
    std::cout << "접속 ip=" << inet_ntoa(addr.sin_addr) << "port=" << ntohs(addr.sin_port) << std::endl;
       
    m_SessionList.emplace_back(sock, addr,this);
    ::CreateIoCompletionPort((HANDLE)sock, g_hIOCP,
        (ULONG_PTR)&m_SessionList.back(), 0);
    //::CreateIoCompletionPort((HANDLE)sock, g_hIOCP, sock, 0);

    TSession& user = m_SessionList.back();
    user.AsyncRecv();
}
void   TNetIOCPServer::Run()
{   
    while (m_bRun)
    {
        if (Accept() == false)
        {
            break;
        }        
        if (PacketProcess() == false)
        {
            break;
        }
        CheckConnected();
    }
}

void	TNetIOCPServer::Release()
{
    for (int iThread = 0; iThread < MAX_NUM_WORKDER_THREAD; iThread++)
    {
        CloseHandle(g_hWorkerThread[iThread]);
    }
    CloseHandle(g_hIOCP);
    TNetwork::Release();
}