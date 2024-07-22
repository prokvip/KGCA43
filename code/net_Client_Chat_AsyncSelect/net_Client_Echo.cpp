#include "TNetClient.h"
#include "TWindow.h"
#ifdef _DEBUG
#pragma comment(lib, "TNetwork_d.lib")
#else
#pragma comment(lib, "TNetwork.lib")
#endif


void SenderThread(TNetClient& net)
{
    std::string UserName = "[홍길동]";
    std::string Sendbuf;
    Sendbuf.reserve(256);
    std::string KeyDatabuf;
    KeyDatabuf.reserve(256);
    
    while (net.m_bRun)
    {
        Sendbuf.clear();
        Sendbuf = UserName;
        //std::cout << "데이터입력(종료:엔터) : ";
        std::getline(std::cin, KeyDatabuf);
        if (KeyDatabuf.empty())
        {
            break;
        }
        Sendbuf += KeyDatabuf;

        UPACKET packet = TNetwork::MakePacket(
            Sendbuf.size(),
            PACKET_CHAT_MSG,
            Sendbuf.c_str());

        net.SendPacket(packet);
    }
    closesocket(net.m_hSock);
}


TNetClient net;

class TWin : public TWindow
{
public:
    LRESULT MsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM	lParam)
    {
        if (net.m_pSelectIO != nullptr)
        {
            TAsyncSelect* pAsync = (TAsyncSelect*)net.m_pSelectIO.get();
            pAsync->MsgProc(hwnd, uMsg, wParam, lParam);
        }
        return 0;
    }
};
int main()
{
    TWin win;
    win.CreateWin(NULL);    
    
    net.m_pSelectIO = std::make_shared<TAsyncSelect>(&net);
    TAsyncSelect* pAsync = (TAsyncSelect*)net.m_pSelectIO.get();
    pAsync->Set(win.m_hWnd);


    net.Connected("192.168.0.12", 10000);

    // 종업원 1명
    std::thread sendThread(SenderThread, std::ref(net));
    sendThread.detach();
    
    MSG msg = { };
    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    net.Release();    
}
