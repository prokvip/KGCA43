#include <iostream>
#include "TNetServer.h"
#ifdef _DEBUG
#pragma comment(lib, "TNetwork_d.lib")
#else
#pragma comment(lib, "TNetwork.lib")
#endif
// 라이브러리 프로젝트 속성->빌드이벤트->빌드 후 이벤트
// copy "소스대상" "목적지 복사"
// copy "*.h" "../../TNetwork/Inc/"
// copy "..\\..\\output\\TNetworkLib.lib" "../../TNetwork/lib/"
int main()
{
    std::cout << "Select Server starting!" << std::endl;
    TNetServer net;

    net.CreateServer("192.168.0.12", 10000);

    net.m_pSelectIO = std::make_shared<TStdSelect>(&net);
    net.Run();
    net.Release();
}
