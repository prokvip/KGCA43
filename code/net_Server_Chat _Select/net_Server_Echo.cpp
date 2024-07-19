#include "TNetServer.h"

int main()
{
    TNetServer net;
    net.Create("192.168.0.12", 10000);
    net.m_pSelectIO = std::make_shared<TStdSelect>(&net);
    net.Run();
    net.Release();
}
