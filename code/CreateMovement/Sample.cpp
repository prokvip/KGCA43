#include "Sample.h"

void   Sample::Init() 
{	
	T_Math::FVector2 a = { 10.0f, 0.0f };
	T_Math::FVector2 b = { 0.0f, 10.0f };
	float fDot = a | b;
	float fAngle = a.Angle(b);

	RECT rtBk = { 0, 0, g_xClientSize, g_yClientSize };
	objScreen.Create(m_pd3dDevice.Get(),m_pContext, rtBk, L"bk.bmp");

	m_UIList.resize(3);

	m_UIList[0].Create(m_pd3dDevice.Get(), m_pContext, { 0, 0, 100, 100 }, L"kgca1.png");
	m_UIList[1].Create(m_pd3dDevice.Get(), m_pContext, { 700, 0, 800, 100 }, L"kgca2.png");
	m_UIList[2].Create(m_pd3dDevice.Get(), m_pContext, { 700, 500, 800, 600 }, L"kgca3.png");
	hero.Create(m_pd3dDevice.Get(), m_pContext, { 350, 250, 450, 350 }, L"obj.jpg");
	
	for (int iNpc = 0; iNpc < 10; iNpc++)
	{
		TNpc npc;
		T_Math::FVector2 pos;
		pos.X = randstep(0.0f, g_xClientSize);
		pos.Y = randstep(0.0f, g_yClientSize);
		npc.Create(m_pd3dDevice.Get(), m_pContext,
					{ (LONG)pos.X, (LONG)pos.Y,(LONG)(pos.X+ 50.0f), (LONG)(pos.Y+50.0f) }, L"bump_512.bmp");
		m_npcList.push_back(npc);
	}
}
void    Sample::Frame() 
{		
	if (m_Input.KeyCheck('W') == KEY_HOLD)
	{
		hero.Move({ 0.0f, -1.0f });
		//hero.Front();
	}
	if (m_Input.KeyCheck('S') == KEY_HOLD)
	{
		hero.Move({ 0.0f, 1.0f });
		//hero.Back();
	}
	if (m_Input.KeyCheck('A') == KEY_HOLD)
	{
		hero.Move({ -1.0f, 0.0f });
		//hero.Left();
	}
	if (m_Input.KeyCheck('D') == KEY_HOLD)
	{
		hero.Move({ 1.0f, 0.0f });
		//hero.Right();
	}	

	hero.Frame();
	
	for (int iNpc = 0; iNpc < m_npcList.size(); iNpc++)
	{
		m_npcList[iNpc].Frame();
	}

	/*float fSpeed = 100.0f* g_fSecondPerFrame;
	if (m_Input.KeyCheck('W') == KEY_HOLD)
	{			
		hero.Move(0.0f, -fSpeed);
	}
	if (m_Input.KeyCheck('S') == KEY_HOLD)
	{			
		hero.Move(0.0f, fSpeed);
	}
	if (m_Input.KeyCheck('A') == KEY_HOLD)
	{
		hero.Move(-fSpeed , 0);
	}
	if (m_Input.KeyCheck('D') == KEY_HOLD)
	{
		hero.Move(fSpeed , 0);
	}	*/
}
void    Sample::Render() 
{ 		
	objScreen.Render(m_pContext);
	/*for (auto& obj : m_UIList)
	{
		obj.Render(m_pContext);
	}*/
	//auto LamdaRender = [&](auto& obj) { obj.Render(m_pContext); };
	for_each(begin(m_UIList), end(m_UIList), [&](auto& obj) 
		{
			obj.Render(m_pContext); 
		});

	hero.Render(m_pContext);
	for (int iNpc = 0; iNpc < m_npcList.size(); iNpc++)
	{
		m_npcList[iNpc].Render(m_pContext);
	}
}
void    Sample::Release() 
{		
	objScreen.Release();
	for (auto& obj : m_UIList)
	{
		obj.Release();
	}
	hero.Release();
	for (int iNpc = 0; iNpc < m_npcList.size(); iNpc++)
	{
		m_npcList[iNpc].Release();
	}
}

T_GAME_START(800, 600);