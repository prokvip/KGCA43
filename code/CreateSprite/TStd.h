#pragma once
#include <Windows.h>
#include <vector>
#include <string>
#include <map>
#include <list>
#include <memory>
#include <algorithm>
#include <d3d11.h>
#include <d3dcompiler.h> 
#include <tchar.h>
#include <wrl.h> // windows runtime c++ template library
//../../dxtk/include  기본경로로 설정한다. (속성)
#include "WicTextureLoader.h"
#include "DDSTextureLoader.h"
#include "TMath.h"
#include <atlconv.h> // A2W

#pragma comment ( lib, "d3dcompiler.lib")

#ifdef _DEBUG
#pragma comment(lib, "DirectXTK_D.lib")
#else
#pragma comment(lib, "DirectXTK_R.lib")
#endif
using namespace Microsoft::WRL;

extern UINT		g_xClientSize;
extern UINT		g_yClientSize;
extern double   g_fSecondPerFrame;
extern double   g_fGameTime;
class TBaseCore
{
	virtual void Init() = 0;
	virtual void Frame() = 0;
	virtual void Render() = 0;
	virtual void Release() = 0;
};
static std::wstring to_mw(const std::string& _src)
{
	USES_CONVERSION;
	return std::wstring(A2W(_src.c_str()));
};

static std::string to_wm(const std::wstring& _src)
{
	USES_CONVERSION;
	return std::string(W2A(_src.c_str()));
};

#define SAMPLE_CREATE class Sample : public TCore {
#define SAMPLE_END };
#define T_GAME_START(x, y) int WINAPI wWinMain(HINSTANCE hInstance,	HINSTANCE hPrevInstance,PWSTR pCmdLine,int nCmdShow){	Sample win;	if (win.CreateWin(hInstance, x, y))	{		win.GameRun();	}	return 0;}
#define T_GAME(x,y)  SAMPLE_CREATE SAMPLE_END T_GAME_START(x,y)