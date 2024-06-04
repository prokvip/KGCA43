#include "TCore.h"
#include "TDxObject.h"

class Sample: public TCore
{
	TDxObject objScreen; // »ï°¢Çü
	std::vector<TDxObject> objList;	
	TDxObject hero;
public:
	void   Init() override
	{		
		RECT rtBk = { 0, 0, g_xClientSize, g_yClientSize };
		objScreen.Create(m_pd3dDevice,m_pContext, rtBk, L"bk.bmp");

		objList.resize(3);

		objList[0].Create(m_pd3dDevice, m_pContext, { 0, 0, 100, 100 }, L"kgca1.png");
		objList[1].Create(m_pd3dDevice, m_pContext, { 700, 0, 800, 100 }, L"kgca2.png");
		objList[2].Create(m_pd3dDevice, m_pContext, { 700, 500, 800, 600 }, L"kgca3.png");
		hero.Create(m_pd3dDevice, m_pContext, { 350, 250, 450, 350 }, L"obj.jpg");
	}
	void   Frame() override
	{
		float fSpeed = g_fSecondPerFrame;
		if (m_Input.KeyCheck('W') == KEY_HOLD)
		{			
			hero.Move(0.0f, fSpeed);
		}
		if (m_Input.KeyCheck('S') == KEY_HOLD)
		{			
			hero.Move(0.0f, -fSpeed);
		}
		if (m_Input.KeyCheck('A') == KEY_HOLD)
		{
			hero.Move(-fSpeed , 0);
		}
		if (m_Input.KeyCheck('D') == KEY_HOLD)
		{
			hero.Move(fSpeed , 0);
		}	
		hero.Render(m_pContext);
	}
	void   Render() override
	{ 		
		objScreen.Render(m_pContext);
		for (auto& obj : objList)
		{
			obj.Render(m_pContext);
		}
		hero.Render(m_pContext);
	}
	void   Release() override
	{		
		objScreen.Release();
		for (auto& obj : objList)
		{
			obj.Release();
		}
		hero.Release();
	}

};
T_GAME_START(800, 600);