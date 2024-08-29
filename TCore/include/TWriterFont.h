#pragma once
#include "TStd.h"
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#pragma comment( lib, "d2d1.lib")
#pragma comment( lib, "dwrite.lib")


class TWriterFont : public TBaseCore
{
	ID2D1Factory*		m_pd2dFactory = nullptr;
	ID2D1RenderTarget*	m_pd2dRT = nullptr;
	IDWriteFactory*		m_pWriteFactory = nullptr;
	IDWriteTextFormat*	m_pWriteTF30 = nullptr;
	IDWriteTextFormat*  m_pWriteTF50 = nullptr;
	ID2D1SolidColorBrush* m_pDefaultColor= nullptr;
public:
	virtual void Init() override;
	virtual void Frame()override;
	virtual void Render()override;
	virtual void Release()override;
	virtual bool ResizeDevice();
	void ResourceRelease();
	void ResetDevice(IDXGISurface* dxgiSurface);
	void DrawText(std::wstring msg, POINT pos);
};

