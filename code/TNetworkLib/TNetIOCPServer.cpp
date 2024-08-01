#include "TNetIOCPServer.h"

DWORD  WINAPI  WorkerProc(LPVOID lpThreadParameter)
{
    TNetIOCPServer* iocp = ((TNetIOCPServer*)lpThreadParameter);
    DWORD		dwTransfer;
    ULONG_PTR	CompletionKey;
    tOV  m_ov;
    tOV* lpOverlapped = nullptr;
    TSession* pSession = nullptr;

    WaitForSingleObject(iocp->m_hRunning, INFINITE);

    while (iocp->m_bRun)
    {
        if (WaitForSingleObject(iocp->m_hKillEvent,1) == WAIT_OBJECT_0)
        {
            break;
        }
        BOOL ret = GetQueuedCompletionStatus(iocp->g_hIOCP,
            &dwTransfer, &CompletionKey,
            (LPOVERLAPPED*)&lpOverlapped, INFINITE);

        // 어떤 유저의 비동기 작업인지 알수가 있다.
        pSession = (TSession*)CompletionKey;
        if (pSession && lpOverlapped && lpOverlapped->iIOFlag == tOV::t_END)
        {
            pSession->m_bDisConnected = true;
            continue;
        }
        if (ret == TRUE)
        {
            if (dwTransfer == 0)
            {                
                // 유저 정상적으로 탈퇴
                lpOverlapped->iIOFlag = tOV::t_END;
                PostQueuedCompletionStatus(iocp->g_hIOCP, 0, CompletionKey,
                    (LPOVERLAPPED)lpOverlapped);
                continue;
            }            
            if (pSession->Dispatch(dwTransfer, lpOverlapped))
            {
                // 통계처리, 별도처리
            }
        }
        else
        {
            lpOverlapped->iIOFlag = tOV::t_END;
            PostQueuedCompletionStatus(iocp->g_hIOCP, 0, CompletionKey,
                (LPOVERLAPPED)lpOverlapped);
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
    // 수동리셋이벤트 : 초기값 = 넌시그널(비신호상태)
    m_hKillEvent = ::CreateEvent(0, TRUE, FALSE, L"testEvent");
    //::SetEvent(m_hKillEvent);
    //::ResetEvent(m_hKillEvent);

    m_hRunning = ::CreateEvent(0, TRUE, FALSE, L"RunEvent");
    

    TNetwork::CreateServer(ip, port);
    
    
    ::SetEvent(m_hRunning);
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