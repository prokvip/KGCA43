#include <windows.h>
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
            //GameFrame()
            //GameRender();
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
