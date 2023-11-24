#include "TIocpModel.h"
#include "TUser.h"

DWORD WINAPI WorkerThread(LPVOID param)
{
    TIocpModel* iocp = (TIocpModel*)param;
    DWORD dwTransfer; // 읽고,쓰고 한 바이트 
    ULONG_PTR KeyValue;
    OVERLAPPED* ov;

    while (1)
    {
        if (WaitForSingleObject(iocp->g_hKillEvent, 0) == WAIT_OBJECT_0)
        {
            break;
        }
        // 비동기 입출력에 결과가 IOCP 큐에 저장된다.
        //GetQueuedCompletionStatus는 IOCP큐에서 완료 입출력 가져온다.
        BOOL bReturn = ::GetQueuedCompletionStatus(iocp->g_hIOCP, &dwTransfer, &KeyValue, &ov,
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
                ::SetEvent(iocp->g_hKillEvent);
                break;
            }
            //#define ERROR_HANDLE_EOF                 38L   
            E_MSG("error");
            PrintDetailA("Error!");
            ::SetEvent(iocp->g_hKillEvent);
            break;
        }
    }
    return 0;
}

bool TIocpModel::Init()
{
    g_hKillEvent = ::CreateEvent(0, TRUE, FALSE, 0);
    g_hIOCP = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);

    DWORD dwThreadID;
    for (int iThread = 0; iThread < MAX_WORKER_THREAD; iThread++)
    {
        g_hWorkerThread[iThread] = ::CreateThread(0, 0, WorkerThread,
                                                    this, 0, &dwThreadID);
    }
    return true;
}