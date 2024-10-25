#pragma once
#include "TCore.h"
#include "TMap.h"

#include <wincodec.h>
#include <wincodecsdk.h>
#pragma comment (lib,"windowscodecs.lib")


#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

class Sample : public TCore
{	
	TMap		m_Map;
public:
	void   Init() override;
	void   Frame() override;
	void   Render() override;
	void   PostRender() override;
	void   Release() override;
	void   LoadHeightMapTexture(T_STR texName, TMap& map);
	void   LoadTextureAndPixelWriteSave(T_STR texName, T_STR texSave);
	void   SaveFile(T_STR name, ID3D11Texture2D* pRes);
public:
	virtual LRESULT MsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM	lParam);
	void   SetImGui();
	void   FrameImGui();
	void   RenderImGui();
};