#include "TAsyncSelect.h"
#include "TNetwork.h"
bool TAsyncSelect::Set(HWND hWnd)
{
    _ASSERT(m_pNet);
    WSAAsyncSelect(m_pNet->m_hSock, hWnd, WM_SOCKET, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);
    return true;
}

LRESULT TAsyncSelect::MsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM	lParam)
{
    _ASSERT(m_pNet);  

    switch (uMsg)
    {
        case WM_SOCKET:
        {
            int iEvent = WSAGETSELECTEVENT(lParam);
            int iSock = WSAGETSELECTEVENT(wParam);

            switch (iEvent)
            {
                case  FD_CONNECT:
                {
                    m_pNet->Connected();
                    int k = 0;
                }break;
                case  FD_READ:
                {
                    if (m_pNet->Recv()==false)
                    {
                        m_pNet->DisConnected();
                    }
                }break;
                case  FD_WRITE:
                {
                    int k = 0;
                }break;
                case  FD_CLOSE:
                {
                    m_pNet->DisConnected();
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