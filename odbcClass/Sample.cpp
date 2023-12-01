#pragma once
#include "TOdbc.h"
#include "resource.h"

HINSTANCE g_hInstance;
HWND  g_hItemList;
HWND  g_hName;
HWND  g_hPrice;
HWND  g_hKorean;

HWND        g_hWnd;
TOdbc       g_Odbc;
INT_PTR   CALLBACK DlgProc(HWND hDlg,
    UINT iMsg, WPARAM wParam, LPARAM lParam);

void SelectAllRecord();
void InsertRecord();
void UpdateRecord();
void DeleteRecord();


void InsertRecord()
{
    SQLTCHAR name[MAX_PATH];
    SQLTCHAR Price[MAX_PATH];
    int  iKorean=0;
    
    GetWindowText(g_hName, name, MAX_PATH);
    GetWindowText(g_hPrice, Price, MAX_PATH);
    if (BST_CHECKED == SendMessage(g_hKorean, BM_GETCHECK, 0, 0))
    {
        iKorean = 1;
    }
    ZeroMemory(g_Odbc.m_szInsertName, sizeof(g_Odbc.m_szInsertName));
    CopyMemory(g_Odbc.m_szInsertName, name, _tcslen(name)*sizeof(TCHAR));
    g_Odbc.m_iInsertPrice = _ttoi(Price);
    g_Odbc.m_iKorean = iKorean;
    SQLRETURN hr = SQLExecute(g_Odbc.g_hInsertStmt);
    if (hr != SQL_SUCCESS)
    {
        g_Odbc.ErrorMsg();
        return;
    }   
    SQLCloseCursor(g_Odbc.g_hInsertStmt);
    SQLFreeStmt(g_Odbc.g_hInsertStmt, SQL_CLOSE); 
    SelectAllRecord();
}
void UpdateRecord()
{
    TCHAR selectName[256] = { 0, };
    int id = SendMessage(g_hItemList, LB_GETCURSEL, 0, 0);
    if (id != -1)
    {        
        SendMessage(g_hItemList, LB_GETTEXT, id, (LPARAM)selectName);
    }
    else
    {
        return;
    }
    SQLTCHAR name[MAX_PATH];
    SQLTCHAR Price[MAX_PATH];
    int  iKorean = 0;
    GetWindowText(g_hName, name, MAX_PATH);
    GetWindowText(g_hPrice, Price, MAX_PATH);
    if (BST_CHECKED == SendMessage(g_hKorean, BM_GETCHECK, 0, 0))
    {
        iKorean = 1;
    }
    TCHAR sql[MAX_PATH] = { 0, };
    _stprintf(sql,
        L"update tblCigar set name='%s', price=%d, korean=%d where name='%s'",
        name, _ttoi(Price), iKorean, selectName);
    SQLRETURN hr = SQLExecDirect(g_Odbc.g_hStmt, sql, SQL_NTS);
    if (hr != SQL_SUCCESS)
    {
        g_Odbc.ErrorMsg();
        return;
    }
    SQLCloseCursor(g_Odbc.g_hStmt);

    SelectAllRecord();
}
void DeleteRecord()
{
    SQLTCHAR name[MAX_PATH];
    GetWindowText(g_hName, name, MAX_PATH);

    TCHAR sql[MAX_PATH] = { 0, };
    _stprintf(sql,
        L"delete from tblCigar where name='%s'",name);
    SQLRETURN hr = SQLExecDirect(g_Odbc.g_hStmt, sql, SQL_NTS);
    if (hr != SQL_SUCCESS)
    {
        g_Odbc.ErrorMsg();
        return;
    }
    SQLCloseCursor(g_Odbc.g_hStmt);

    SelectAllRecord();
}
void SelectAllRecord()
{
    g_Odbc.g_dbDataList.clear();

    // 조회
    TCHAR sql[] = L"SELECT *  FROM tblCigar";
    SQLRETURN hr = SQLExecDirect(g_Odbc.g_hStmt, sql, SQL_NTS);

    TColDescription desc;
    desc.icol = 1;
    while (1)
    {
        int ret = SQLDescribeCol(g_Odbc.g_hStmt,            
            desc.icol,
            desc.szColName, sizeof(desc.szColName),
            &desc.pcchColName,
            &desc.pfSqlType,
            &desc.pcbColDef,
            &desc.pibScale,
            &desc.pfNullable);

        if (ret != SQL_SUCCESS)
        {
            break;
        }
        g_Odbc.g_descList.push_back(desc);
        desc.icol++;
    }    
    while (SQLFetch(g_Odbc.g_hStmt) != SQL_NO_DATA)
    {
        RECORD record;
        for (int iCol = 0; iCol < g_Odbc.g_descList.size(); iCol++)
        {
            int ret = SQLGetData(g_Odbc.g_hStmt,
                g_Odbc.g_descList[iCol].icol, SQL_WCHAR,
                g_Odbc.g_descList[iCol].bindData,
                sizeof(g_Odbc.g_descList[iCol].bindData), NULL);
            if (ret == SQL_SUCCESS)
            {
                record.push_back(g_Odbc.g_descList[iCol].bindData);
            }
            else
            {

            }
        }        

        g_Odbc.g_dbDataList.push_back(record);
    }
    SQLCloseCursor(g_Odbc.g_hStmt);

    SendMessage(g_hItemList, LB_RESETCONTENT, 0,0);
    for (int i = 0; i < g_Odbc.g_dbDataList.size(); i++)
    {
        SendMessage(g_hItemList, LB_ADDSTRING, 0,
            (LPARAM)g_Odbc.g_dbDataList[i][0].c_str());
    }

    SQLFreeStmt(g_Odbc.g_hStmt, SQL_CLOSE);
}

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
        if (g_Odbc.g_dbDataList.size() >= 1)
        {
            SetWindowText(g_hName, g_Odbc.g_dbDataList[0][0].c_str());
            SetWindowText(g_hPrice,g_Odbc.g_dbDataList[0][1].c_str());
            /*SetWindowText(g_hKorean,
                std::to_wstring(tableList[0].Korean).c_str());*/
            SendMessage(g_hKorean, BM_SETCHECK,
                (g_Odbc.g_dbDataList[0][0] == L"1") ? BST_CHECKED : BST_UNCHECKED, 0);
        }
        for (int i = 0; i < g_Odbc.g_dbDataList.size(); i++)
        {
            SendMessage(g_hItemList, LB_ADDSTRING, 0,
                (LPARAM)g_Odbc.g_dbDataList[i][0].c_str());
        }

    }break;
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case IDC_INSERT_ITEM: { InsertRecord(); }break;
        case IDC_UPDATE_ITEM: {UpdateRecord(); }break;
        case IDC_DELETE_ITEM: {DeleteRecord(); }break;
        case IDC_SELECT_ALL: {SelectAllRecord(); }break;
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

                            //std::wstring sqlString = L"SELECT ";
                            //for (int i = 0; i < g_Odbc.g_descList.size(); i++)
                            //{
                            //    sqlString += g_Odbc.g_descList[i].pcchColName;
                            //    if (i != g_Odbc.g_descList.size() - 1)
                            //    {
                            //        sqlString += L", ";
                            //    }
                            //}
                            //sqlString += L"FROM ";
                            //sqlString += L"tblCigar ";
                            //sqlString += L"WHERE ";
                            //sqlString += g_Odbc.g_descList[0].pcchColName;
                            //sqlString += L"='%s'";
                            //_stprintf(sql,
                            //    sqlString.c_str(),
                            //    selectName);

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
        SelectAllRecord();
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
