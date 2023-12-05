#include "Sample.h"
bool    Sample::Init()
{     
    // v0       v1
    //
    // v3       v2
    m_VertexList.emplace_back(0.0f, 0.0f);      // 0
    m_VertexList.emplace_back(800.0f, 0.0f);    // 1
    m_VertexList.emplace_back(800.0f, 600.0f);  // 2
    m_VertexList.emplace_back(0.0f, 600.0f);    // 3

    m_IndexList.push_back(0);
    m_IndexList.push_back(1);
    m_IndexList.push_back(2);

    m_IndexList.push_back(2);
    m_IndexList.push_back(3);
    m_IndexList.push_back(0);

    return true;
}
bool    Sample::Render() 
{   
    return true;
}
bool    Sample::Release() 
{   
    return true;
}

//HINSTANCE : 운영체제가 관리하는 윈도우 프로세스ID
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    Sample win;
    win.SetWindow(hInstance, nCmdShow);
    win.Run();
    return 0;    
}


