#include "TIocpModel.h"

DWORD WINAPI WorkerThread(LPVOID param)
{
    DWORD dwTransfer; // 읽고,쓰고 한 바이트 
    ULONG_PTR KeyValue;
    OVERLAPPED* ov;

    while (1)
    {
        if (WaitForSingleObject(g_hKillEvent, 0) == WAIT_OBJECT_0)
        {
            break;
        }
        // 비동기 입출력에 결과가 IOCP 큐에 저장된다.
        //GetQueuedCompletionStatus는 IOCP큐에서 완료 입출력 가져온다.
        BOOL bReturn = ::GetQueuedCompletionStatus(g_hIOCP, &dwTransfer, &KeyValue, &ov,
            0);

        if (bReturn == TRUE)
        {
            TUser* pUser = (TUser*)KeyValue;
            if (pUser != nullptr)
            {
                pUser->Dispatch(dwTransfer, ov);
            }
        }
        else
        {
            //프로그램 데이터베이스를 쓰는 동안 오류가 발생했습니다. 디스크 공간이 부족한지, 경로가 잘못되었는지 또는 권한이 없는지 확인하십시오.
            DWORD dwError = GetLastError();
            if (dwError == WAIT_TIMEOUT)
            {
                continue;
            }
            if (ERROR_HANDLE_EOF == dwError)
            {
                ::SetEvent(g_hKillEvent);
                break;
            }
            //#define ERROR_HANDLE_EOF                 38L   
            E_MSG("error");
            PrintDetailA("Error!");
            ::SetEvent(g_hKillEvent);
            break;
        }
    }
    return 0;
}

void IOCPRun()
{
    g_hKillEvent = ::CreateEvent(0, TRUE, FALSE, 0);
    g_hIOCP = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);

    // 1) 윈속 초기화
    WSADATA wsa;
    int iRet = 0;
    iRet = WSAStartup(MAKEWORD(2, 2), &wsa);
    if (iRet != 0) return;
    listensock = socket(AF_INET, SOCK_STREAM, 0);

    SOCKADDR_IN sa;
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(INADDR_ANY);// inet_addr("192.168.0.12");
    sa.sin_port = htons(10000);

    iRet = bind(listensock, (SOCKADDR*)&sa, sizeof(sa));
    if (iRet == SOCKET_ERROR) return;

    iRet = listen(listensock, SOMAXCONN);
    if (iRet == SOCKET_ERROR) return;

    DWORD dwThreadID;
    CloseHandle(::CreateThread(0, 0, WorkerAcceptThread, nullptr, 0, &dwThreadID));

    for (int iThread = 0; iThread < MAX_WORKER_THREAD; iThread++)
    {
        g_hWorkerThread[iThread] = ::CreateThread(0, 0, WorkerThread, nullptr, 0, &dwThreadID);
    }

}