#include "TNetStd.h"
#define MAX_WORKER_THREAD 2
#define g_dwMaxReadSize (4096*4096)
char    g_pDataBuffer[g_dwMaxReadSize];
HANDLE	        g_hKillEvent;
HANDLE          g_hWorkerThread[MAX_WORKER_THREAD];
HANDLE          g_hIOCP;
TCHAR*          g_pFileBuffer = nullptr;
LARGE_INTEGER   g_filesize;
LARGE_INTEGER   g_OffsetRead;
LARGE_INTEGER   g_OffsetWrite;

HANDLE g_hReadFile;
HANDLE g_hWriteFile;
OVERLAPPED g_hReadOverlapped;
OVERLAPPED g_hWriteOverlapped;
bool  DispatchRead(DWORD dwTransfer, OVERLAPPED* ov)
{
    g_OffsetRead.QuadPart += dwTransfer;
    g_hReadOverlapped.Offset     = g_OffsetRead.LowPart;
    g_hReadOverlapped.OffsetHigh = g_OffsetRead.HighPart;
    DWORD dwWriteByte;
    BOOL bRet = WriteFile(g_hWriteFile, g_pDataBuffer, dwTransfer, &dwWriteByte, &g_hWriteOverlapped);
    if (bRet == FALSE)
    {
        DWORD dwError = GetLastError();
        if (dwError != ERROR_IO_PENDING)
        {
            return false;
        }
    }
    return true;
}
bool DispatchWrite(DWORD dwTransfer, OVERLAPPED* ov)
{
    g_OffsetWrite.QuadPart += dwTransfer;
    g_hWriteOverlapped.Offset = g_OffsetWrite.LowPart;
    g_hWriteOverlapped.OffsetHigh = g_OffsetWrite.HighPart;
    
    if (dwTransfer < g_dwMaxReadSize)
    {
        return false;
    }

    DWORD dwReadByte;
    BOOL bRet = ReadFile(g_hReadFile, g_pDataBuffer, g_dwMaxReadSize, &dwReadByte, &g_hReadOverlapped);
    if (bRet == FALSE)
    {
        DWORD dwError = GetLastError();
        if (dwError != ERROR_IO_PENDING)
        {
            return false;
        }
    }
    return true;

}

bool bRun = true;

DWORD WINAPI WorkerThread(LPVOID param)
{
    DWORD dwTransfer; // 읽고,쓰고 한 바이트 
    ULONG_PTR KeyValue;
    OVERLAPPED*  ov;
    
    while (1)
    {
        if (WaitForSingleObject(g_hKillEvent, 0) == WAIT_OBJECT_0)
        {
            break;
        }
        // 비동기 입출력에 결과가 IOCP 큐에 저장된다.
        //GetQueuedCompletionStatus는 IOCP큐에서 완료 입출력 가져온다.
       BOOL bReturn = ::GetQueuedCompletionStatus(g_hIOCP, &dwTransfer, &KeyValue, &ov,
           0 );       

       if (bReturn == TRUE)
       {
           if (KeyValue == 1000)
           {
               if (DispatchRead(dwTransfer, ov)== FALSE)
               {                   
                   ::SetEvent(g_hKillEvent);
                   break;
               }
               PrintA("READ[%f] \n", (float)g_OffsetRead.QuadPart /
                                        (float)g_filesize.QuadPart * 100.0f);
           }
           if (KeyValue == 2000)
           {
               if (DispatchWrite(dwTransfer, ov)==FALSE)
               {
                   ::SetEvent(g_hKillEvent);
                   break;
               }
               PrintA("WRITE[%f] \n", (float)g_OffsetWrite.QuadPart /
                   (float)g_filesize.QuadPart * 100.0f);
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

    g_OffsetRead.QuadPart = 0;
    g_OffsetWrite.QuadPart = 0;

    g_hIOCP = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0,0,0);

    

    std::wstring readname = L"sql";
    std::wstring writename = L"Copy";
    std::wstring ext = L".iso";
    readname += ext;
    writename += ext;
    g_hReadFile = CreateFile(readname.c_str(),
        GENERIC_READ | GENERIC_WRITE, 0,
        NULL, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
        NULL);

    ::GetFileSizeEx(g_hReadFile, &g_filesize);

    g_hWriteFile = CreateFile(writename.c_str(),
        GENERIC_READ | GENERIC_WRITE, 0,
        NULL, CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
        NULL);

    ::CreateIoCompletionPort(g_hReadFile, g_hIOCP, 1000, 0);
    ::CreateIoCompletionPort(g_hWriteFile, g_hIOCP, 2000, 0);    

    //g_hWorkerThread[MAX_WORKER_THREAD];
    DWORD dwThreadID;
    for (int iThread = 0; iThread < MAX_WORKER_THREAD; iThread++)
    {
        g_hWorkerThread[iThread] = ::CreateThread(0, 0, WorkerThread, nullptr, 0, &dwThreadID);
    }

    ZeroMemory(&g_hReadOverlapped, sizeof(g_hReadOverlapped));
    ZeroMemory(&g_hWriteOverlapped, sizeof(g_hWriteOverlapped));

    DWORD dwReadByte;
    ReadFile(g_hReadFile, g_pDataBuffer, g_dwMaxReadSize, &dwReadByte, &g_hReadOverlapped);

    WaitForMultipleObjects(MAX_WORKER_THREAD, g_hWorkerThread, TRUE, INFINITE);

    CloseHandle(g_hReadFile);
    CloseHandle(g_hWriteFile);
    CloseHandle(g_hKillEvent);

    for (int iThread = 0; iThread < MAX_WORKER_THREAD; iThread++)
    {
        CloseHandle(g_hWorkerThread[iThread]);
    }
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_SIZE:
    {
        UINT width = LOWORD(lParam);
        UINT height = HIWORD(lParam);
        int k = 0;
    }break;
    case WM_CREATE: break;
    case WM_DESTROY:
        PostQuitMessage(0);// WM_QUIT
        break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
ATOM MyRegisterClass(HINSTANCE hInstance);

//HINSTANCE : 운영체제가 관리하는 윈도우 프로세스ID
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{

    if (AllocConsole() == TRUE)
    {
        FILE* console = nullptr;
        if (freopen_s(&console, "conin$", "rt", stdin))
        {
            return 0;
        }
        if (freopen_s(&console, "conin$", "wt", stdout))
        {
            return 0;
        }
        if (freopen_s(&console, "conin$", "wt", stderr))
        {
            return 0;
        }
    }
    IOCPRun();

    //1번 ) 윈도우 레지스터 클래스 등록
    MyRegisterClass(hInstance);
    //2번 ) 윈도우 생성
    InitInstance(hInstance, nCmdShow);
    //3번 ) 윈도우 프로시져 작업
    MSG msg;
    // 기본 메시지 루프입니다:
    //WM_QUIT : FALSE
    //while (GetMessage(&msg, nullptr, 0, 0))
    while(1)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
            {
                break;
            }
        }
        else
        {            
                   
        }
    }
    return (int)msg.wParam;    
}
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hWnd = CreateWindowW(L"KGCA_WINDOWS", 
        L"Sample Projects", WS_OVERLAPPEDWINDOW,
        0, 0, 800, 600, nullptr, nullptr, 
        hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
}
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;
    ZeroMemory(&wcex, sizeof(wcex));

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.lpszClassName = L"KGCA_WINDOWS";
    //wcex.hCursor = LoadCursor(NULL, IDC_CROSS);
    wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    return RegisterClassExW(&wcex);
}
