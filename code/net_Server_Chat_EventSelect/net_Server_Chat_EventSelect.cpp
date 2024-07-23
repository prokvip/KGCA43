#include "TNetServer.h"
#include <iostream>

#ifdef _DEBUG
#pragma comment(lib, "TNetwork_d.lib")
#else
#pragma comment(lib, "TNetwork.lib")
#endif

int main()
{
    std::cout << "EventSelect Server starting!" << std::endl;
    TNetServer net;

    net.CreateServer("192.168.0.12", 10000);

    net.m_pSelectIO = std::make_shared<TEventSelect>(&net);
    net.Run();
    net.Release();
}
