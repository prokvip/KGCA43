#include "TAsyncSelect.h"
#include "TNetwork.h"
bool TAsyncSelect::Set(HWND hWnd)
{
    WSAAsyncSelect(m_pNet->m_hSock, hWnd, WM_SOCKET,
        FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);
    return true;
}

bool TAsyncSelect::Run()
{
    // 메세지큐에 담겨있는 모든 메세지를 수작업으로 프로시져에 전달한다.
    MSG msg = { };
    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return true;
}

LRESULT TAsyncSelect::MsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM	lParam)
{
    int iEvent = WSAGETSELECTEVENT(lParam);
    int iSock = WSAGETSELECTEVENT(wParam);

    switch (uMsg)
    {
    case WM_SOCKET:
    {
        switch (iEvent)
        {
        case  FD_CONNECT:
        {
            m_pNet->Connected(true);
            int k = 0;
        }break;
        case  FD_READ:
        {
            m_pNet->Run();
        }break;
        case  FD_WRITE:
        {
            int k = 0;
        }break;
        case  FD_CLOSE:
        {
            int k = 0;
        }break;
        }
    }
    }
    return  0;
};

TAsyncSelect::TAsyncSelect(TNetwork* net) : TSelectModel(net->m_hSock)
{
	m_pNet = net;
}
TAsyncSelect::~TAsyncSelect()
{

}