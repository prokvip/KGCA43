#include "TDxWrite.h"
#include "TStd.h"
bool        TDxWrite::Init(IDXGISurface* dxgiSurface)
{
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED,
		&m_pD2DFactory);
	if (FAILED(hr))
	{
		return false;
	}

	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		(IUnknown **)&m_pDWriteFactory);
	if (FAILED(hr))
	{
		return false;
	}

	hr = m_pDWriteFactory->CreateTextFormat(
		L"궁서",
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		30,
		L"ko-kr", // L"en-us"
		&m_pTextFormat);

	hr = m_pDWriteFactory->CreateTextFormat(
		L"고딕",
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		20,
		L"ko-kr", // L"en-us"
		&m_pTextFormat20);

	FLOAT x, y;
	//error C4996: 'ID2D1Factory::GetDesktopDpi': Deprecated. 
	//Use DisplayInformation::LogicalDpi for Windows Store Apps or GetDpiForWindow for desktop apps.
	//m_pD2DFactory->GetDesktopDpi(&x, &y);
	UINT dpi = GetDpiForWindow(g_hWnd);
	D2D1_RENDER_TARGET_PROPERTIES rtp;
	rtp.type= D2D1_RENDER_TARGET_TYPE_DEFAULT;
	rtp.pixelFormat.format= DXGI_FORMAT_UNKNOWN;
	rtp.pixelFormat.alphaMode= D2D1_ALPHA_MODE_PREMULTIPLIED;
	rtp.dpiX = dpi;
	rtp.dpiY = dpi;
	rtp.usage= D2D1_RENDER_TARGET_USAGE_NONE;
	rtp. minLevel = D2D1_FEATURE_LEVEL_DEFAULT;
	//createdevice 시에 -> D3D11_CREATE_DEVICE_BGRA_SUPPORT 지원해야 된다.
	hr = m_pD2DFactory->CreateDxgiSurfaceRenderTarget(
		dxgiSurface,
		&rtp,
		&m_pRT);

	D2D1_COLOR_F color;// = D2D1::ColorF(D2D1::ColorF::Yellow);
	color.r = 0.0f;
	color.g = 0.0f;
	color.b = 0.0f;
	color.a = 1.0f;
	hr = m_pRT->CreateSolidColorBrush(
		color,
		&m_pDefaultBrush);
	return true;
}
bool        TDxWrite::Frame()
{
	return true;
}
bool        TDxWrite::Render()
{
	if (m_pRT==nullptr) return true;
	std::wstring msg = L"케이지씨에이(KGCA)";
	m_pRT->BeginDraw();		
		D2D1_RECT_F rtf = { 0.0f, 0.0f, 800.0f, 600.0f};
		D2D1_COLOR_F color;// = D2D1::ColorF(D2D1::ColorF::Yellow);
		color.r = 1.0f;
		color.g = 0.0f;
		color.b = 0.0f;
		color.a = 1.0f;
		m_pDefaultBrush->SetColor(color);
		m_pRT->DrawText(msg.c_str(), msg.size(), m_pTextFormat,
			rtf, m_pDefaultBrush);
	m_pRT->EndDraw();
	return true;
}
void		TDxWrite::Draw30(float x, float y,
							std::wstring msg,
							D2D1::ColorF color)
{
	m_pRT->BeginDraw();
		D2D1_RECT_F rtf = { x, y, 800.0f, 600.0f };
		m_pDefaultBrush->SetColor(color);
		m_pRT->DrawText(msg.c_str(), msg.size(), m_pTextFormat,
			rtf, m_pDefaultBrush);
	m_pRT->EndDraw();
}
void		TDxWrite::Draw20(float x, float y,
	std::wstring msg,
	D2D1::ColorF color)
{
	m_pRT->BeginDraw();
	D2D1_RECT_F rtf = { x, y, 800.0f, 600.0f };
	m_pDefaultBrush->SetColor(color);
	m_pRT->DrawText(msg.c_str(), msg.size(), 
		m_pTextFormat20,
		rtf, m_pDefaultBrush);
	m_pRT->EndDraw();
}
bool        TDxWrite::Release()
{
	if(m_pTextFormat20)m_pTextFormat20->Release();
	if (m_pTextFormat)m_pTextFormat->Release();
	if (m_pDefaultBrush)m_pDefaultBrush->Release();
	if (m_pDWriteFactory)m_pDWriteFactory->Release();
	if (m_pRT)m_pRT->Release();
	if(m_pD2DFactory)m_pD2DFactory->Release();
	return true;
}