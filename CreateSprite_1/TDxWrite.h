#pragma once
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <string>
#include <list>
#pragma comment (lib, "d2d1.lib")
#pragma comment (lib, "dwrite.lib")

class TDxWrite
{
public:
    ID2D1RenderTarget*      m_pRT = nullptr;
    ID2D1Factory*           m_pD2DFactory = nullptr;
    ID2D1SolidColorBrush*   m_pDefaultBrush = nullptr;
    IDWriteFactory*         m_pDWriteFactory = nullptr;
    IDWriteTextFormat*      m_pTextFormat = nullptr;
    IDWriteTextFormat*      m_pTextFormat20 = nullptr;
    //IDWriteTextLayout* m_pTextLayout;
public:
    void		Draw30(   float x, float y,
                        std::wstring msg,        
        D2D1::ColorF color = D2D1::ColorF(0, 0, 0, 1));
    void		Draw20(float x, float y,
        std::wstring msg,
        D2D1::ColorF color = D2D1::ColorF(0, 0, 0, 1));
public:
    bool        Init(IDXGISurface* dxgiSurface);
    bool        Frame();
    bool        Render();
    bool        Release();
};

