#pragma once
#include <Windows.h>
#include <vector>
#include <string>
#include <map>
#include <list>
#include <memory>
#include <algorithm>
#include <tchar.h>
#include <atlconv.h> // A2W
#include <functional>
#include "TMath.h"
#include "FMath.h"
#include "TDevice.h"

#ifdef _DEBUG
#pragma comment(lib, "DirectXTK_D.lib")
#else
#pragma comment(lib, "DirectXTK_R.lib")
#endif

#ifdef _DEBUG
#pragma comment(lib, "TCore_d.lib")
#else
#pragma comment(lib, "TCore.lib")
#endif

extern UINT		g_xClientSize;
extern UINT		g_yClientSize;
extern double   g_fSecondPerFrame;
extern double   g_fGameTime;
extern bool     g_bGameRun;

typedef std::basic_string<TCHAR>				T_STR;
typedef std::basic_string<wchar_t>				W_STR; // std::wstring
typedef std::basic_string<char>					C_STR; // std::string
typedef std::vector<std::basic_string<TCHAR>>	STRING_VECTOR;
typedef std::vector<DWORD>						DWORD_VECTOR;
typedef std::vector<RECT>						RECT_ARRAY;
class TBaseCore
{
public:
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