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
extern HWND     g_hWnd;

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
#define randf(x) (x*rand()/(float)RAND_MAX)
#define randf2(x,off) (off+x*rand()/(float)RAND_MAX)
#define randstep(fMin,fMax) (fMin+((float)fMax-(float)fMin)*rand()/(float)RAND_MAX)
#define clamp(x,MinX,MaxX) if (x>MaxX) x=MaxX; else if (x<MinX) x=MinX;
#define DegreeToRadian( degree ) ((degree) * (XM_PI / 180.0f))
#define RadianToDegree( radian ) ((radian) * (180.0f / XM_PI))
// 
/////////////////////////////////////////  반환하지 않는다. ////////////////////////////////////////////////////////
#if defined(DEBUG) | defined(_DEBUG) 
#ifndef H
//주의사항:매크로 함수안에서 매크로 함수(x가 함수로 해석)를 중복 실행 되지 않게 x=DX메소드만 사용해야 한다.
#define H(x){ hr = (x);if (FAILED(hr)){\
							LPWSTR output;\
							WCHAR buffer[256]={0,};\
							FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_IGNORE_INSERTS |FORMAT_MESSAGE_ALLOCATE_BUFFER,\
							NULL,x,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR)&output,0,NULL);\
							wsprintf(buffer,L"File=%s\nLine=%d", _T(__FILE__), __LINE__);\
							MessageBox(NULL, buffer,output,MB_OK);}\
						}
#endif
#else
#ifndef H
#define H(x) (x)
#endif
#endif

#define SAMPLE_CREATE class Sample : public TCore {
#define SAMPLE_END };
#define T_GAME_START(x, y) int WINAPI wWinMain(HINSTANCE hInstance,	HINSTANCE hPrevInstance,PWSTR pCmdLine,int nCmdShow){	Sample win;	if (win.CreateWin(hInstance, x, y))	{		win.GameRun();	}	return 0;}
#define T_GAME(x,y)  SAMPLE_CREATE SAMPLE_END T_GAME_START(x,y)