#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <thread>
#include "TProtocol.h"
#include <list>
#include "TOdbc.h"
#include "resource.h"

HINSTANCE g_hInstance;
HWND  g_hItemList;
HWND  g_hName;
HWND  g_hPrice;
HWND  g_hKorean;
struct TTable
{
    std::wstring name;
    int   Price;
    int   Korean;
    TTable(std::wstring _name, int _Price, int _Korean)
    {
        name = _name;
        Price = _Price;
        Korean = _Korean;
    }
};
std::vector< TTable> tableList;

static std::string wtm(std::wstring data)
{
    char retData[4096] = { 0 };
    // 변형되는 문자열의 크기가 반환된다.
    int iLength = WideCharToMultiByte(CP_ACP, 0, data.c_str(), -1, 0, 0, NULL, NULL);
    int iRet = WideCharToMultiByte(CP_ACP, 0,
        data.c_str(), -1,  //  소스
        retData, iLength, // 대상
        NULL, NULL);
    return retData;
}
static std::wstring mtw(std::string data)
{
    WCHAR retData[4096] = { 0 };
    // 변형되는 문자열의 크기가 반환된다.
    int iLength = MultiByteToWideChar(CP_ACP, 0, data.c_str(), -1, 0, 0);
    int iRet = MultiByteToWideChar(CP_ACP, 0,
        data.c_str(), -1,  //  소스
        retData, iLength); // 대상
    return retData;
}

static void ConsolePrintW(const wchar_t* fmt, ...)
{
    va_list ap;
    wchar_t buf[MAX_PATH];

    va_start(ap, fmt);
    vswprintf_s(buf, fmt, ap);
    va_end(ap);

    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwBytesWriten;
    WriteConsoleW(handle, buf, wcslen(buf), &dwBytesWriten, 0);
}
static void ConsolePrintA(const char* fmt, ...)
{
    va_list ap;
    char buf[MAX_PATH];

    va_start(ap, fmt);
    vsprintf_s(buf, fmt, ap);
    va_end(ap);

    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwBytesWriten;
    WriteConsoleA(handle, buf, strlen(buf), &dwBytesWriten, 0);
}

#define PrintA(fmt,...)         ConsolePrintA( fmt, __VA_ARGS__ )
#define PrintDetailA(fmt,...)   ConsolePrintA( "[%s %s %d] : " fmt, __FILE__,__FUNCTION__, __LINE__, ##__VA_ARGS__ )
#define PrintW(fmt,...)         ConsolePrintW( fmt, __VA_ARGS__ )
#define PrintDetailW(fmt,...)   ConsolePrintW( L"[%s %d] : " fmt, __FILEW__,__LINE__, ##__VA_ARGS__ )


HWND        g_hWnd;
TOdbc       g_Odbc;
INT_PTR   CALLBACK DlgProc(HWND hDlg,
    UINT iMsg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {    
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case ID_DB_DLG:
            DialogBox(g_hInstance, MAKEINTRESOURCE(IDD_DIALOG1),
                hWnd, (DLGPROC)DlgProc);//HWND_DESKTOP
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);// WM_QUIT
        break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
ATOM MyRegisterClass(HINSTANCE hInstance);

INT_PTR   CALLBACK DlgProc (HWND hDlg, 
    UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    switch (iMsg)
    {
    case WM_INITDIALOG:
    {
//#define IDC_CHECK1                      1005
        g_hItemList = GetDlgItem(hDlg, IDC_ITEMLIST);
        g_hName = GetDlgItem(hDlg, IDC_NAME);
        g_hPrice = GetDlgItem(hDlg, IDC_PRICE);
        g_hKorean = GetDlgItem(hDlg, IDC_CHECK1);
        SetWindowText(g_hName,tableList[0].name.c_str());
        SetWindowText(g_hPrice, 
            std::to_wstring(tableList[0].Price).c_str());
        /*SetWindowText(g_hKorean,
            std::to_wstring(tableList[0].Korean).c_str());*/
        SendMessage(g_hKorean, BM_SETCHECK, 
            (tableList[0].Korean) ?  BST_CHECKED: BST_UNCHECKED,0);

        for (int i = 0; i < tableList.size(); i++)
        {
            SendMessage(g_hItemList, LB_ADDSTRING, 0,
                (LPARAM)tableList[i].name.c_str());
        }

    }break;
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
            case IDC_ITEMLIST:
            {
                switch (HIWORD(wParam))
                {
                    case LBN_SELCHANGE:
                    {
                        int id = SendMessage(g_hItemList, LB_GETCURSEL, 0,0);
                        if (id != -1)
                        {
                            TCHAR selectName[256] = { 0, };
                            SendMessage(g_hItemList, LB_GETTEXT, id, (LPARAM)selectName);

                            // 조회
                            TCHAR sql[MAX_PATH] = { 0, };
                            _stprintf(sql, 
                                L"SELECT name, price, korean  FROM tblCigar WHERE name='%s'", 
                                 selectName);
                            SQLRETURN hr = SQLExecDirect(g_Odbc.g_hStmt, sql, SQL_NTS);

                            TCHAR retName[25] = { 0, };
                            int   retPrice;
                            int   retKorean;
                            SQLLEN lName;
                            SQLLEN lPrive;
                            SQLLEN lKorean;

                            SQLBindCol(g_Odbc.g_hStmt, 1, SQL_UNICODE, retName, _countof(retName), &lName);
                            SQLBindCol(g_Odbc.g_hStmt, 2, SQL_INTEGER, &retPrice, 0, &lPrive);
                            SQLBindCol(g_Odbc.g_hStmt, 3, SQL_C_ULONG, &retKorean, 0, &lKorean);

                            while (SQLFetch(g_Odbc.g_hStmt) != SQL_NO_DATA)
                            {
                                SetWindowText(g_hName, retName);
                                SetWindowText(g_hPrice,
                                    std::to_wstring(retPrice).c_str());
                                SendMessage(g_hKorean, BM_SETCHECK,
                                    (retKorean) ? BST_CHECKED : BST_UNCHECKED, 0);
                            }
                            SQLCloseCursor(g_Odbc.g_hStmt);
                        }
                    }break;
                }
            }break;
            case IDOK:
            case IDCANCEL:
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
        }
        break;     
    }break;
    }
    return FALSE;
}
//HINSTANCE : 운영체제가 관리하는 윈도우 프로세스ID
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{

    g_hInstance = hInstance;

    int iSize = sizeof(CHAT_MSG);

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

    g_Odbc.Init();    
    if (g_Odbc.Connect())
    {
        // 조회
        TCHAR sql[] = L"SELECT *  FROM tblCigar";
        SQLRETURN hr = SQLExecDirect(g_Odbc.g_hStmt, sql, SQL_NTS);

        TCHAR retName[25] = { 0, };
        int   retPrice;
        int   retKorean;
        SQLLEN lName;
        SQLLEN lPrive;
        SQLLEN lKorean;

        SQLBindCol(g_Odbc.g_hStmt, 1, SQL_UNICODE, retName, _countof(retName), &lName);
        SQLBindCol(g_Odbc.g_hStmt, 2, SQL_INTEGER, &retPrice,  0,              &lPrive);
        SQLBindCol(g_Odbc.g_hStmt, 3, SQL_C_ULONG, &retKorean, 0,              &lKorean);

        while (SQLFetch(g_Odbc.g_hStmt) != SQL_NO_DATA)
        {
            tableList.emplace_back(retName ,retPrice, retKorean);
        }
        SQLCloseCursor(g_Odbc.g_hStmt);
    }

    /*DialogBox(hInstance,
        MAKEINTRESOURCE(IDD_DIALOG1),
        HWND_DESKTOP, (DLGPROC)DlgProc);*/

    //1번 ) 윈도우 레지스터 클래스 등록
    MyRegisterClass(hInstance);
    //2번 ) 윈도우 생성
    InitInstance(hInstance, nCmdShow);
    
    //3번 ) 윈도우 프로시져 작업
    MSG msg;
    SOCKADDR_IN PeerAddr;
    int addlen;

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
    g_Odbc.Release();
    return (int)msg.wParam;    
}
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hWnd = CreateWindowW(L"KGCA_WINDOWS", 
        L"체팅프로그램", WS_OVERLAPPEDWINDOW,
        0, 0, 800, 600, nullptr, nullptr, 
        hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }
    g_hWnd = hWnd;
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
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDR_MENU1);

    //wcex.hCursor = LoadCursor(NULL, IDC_CROSS);
    wcex.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
    return RegisterClassExW(&wcex);
}
