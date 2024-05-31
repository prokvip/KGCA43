#pragma once
#include <Windows.h>
#include <vector>
#include <string>
#include <map>
#include <list>
#include <algorithm>

class TBaseCore
{
	virtual void Init() = 0;
	virtual void Frame() = 0;
	virtual void Render() = 0;
	virtual void Release() = 0;
};

#define SAMPLE_CREATE class Sample : public TCore {
#define SAMPLE_END };
#define T_GAME_START(x, y) int WINAPI wWinMain(HINSTANCE hInstance,	HINSTANCE hPrevInstance,PWSTR pCmdLine,int nCmdShow){	Sample win;	if (win.CreateWin(hInstance, x, y))	{		win.GameRun();	}	return 0;}
#define T_GAME(x,y)  SAMPLE_CREATE SAMPLE_END T_GAME_START(x,y)