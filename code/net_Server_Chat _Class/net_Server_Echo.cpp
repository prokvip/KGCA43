#include "TNetwork.h"

int main()
{
    TNetworkServer net;
    net.Create("192.168.0.12", 10000);
    net.Run();
    net.Release();
}
