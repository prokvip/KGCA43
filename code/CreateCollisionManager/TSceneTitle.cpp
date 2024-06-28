#include "TSceneTitle.h"
#include "TInput.h"
#include "TGame.h"
#include "TObjectMgr.h"
// GAME_TITLE	-> EVENT_CLICK			-> GAME_INGAME
// GAME_TITLE	-> EVENT_TIMER:3		-> GAME_INGAME
void    TSceneTitle::Execute()
{	
	I_ObjMgr.Frame();

	/*m_fEventTimer += g_fSecondPerFrame;
	if (m_fEventTimer > 3.0f)
	{
		TScene* pScene = m_pGame->Transition(this, EVENT_TIMER);
		pScene->Reset();
		m_fEventTimer = 0.0f;
	}*/

	/*if (TCollision::RectToPt(m_StartBtn.m_rt, I_Input.m_ptMousePos))
	{			
		if (I_Input.KeyCheck(VK_LBUTTON) == KEY_PUSH)
		{
			m_bSceneChange = true;
		}
	}*/
	if (m_StartBtn.m_dwSelectState == T_SELECTED)
	{
		TScene* pScene = m_pGame->Transition(this, EVENT_CLICK);
		pScene->Reset();
	}

	/*if (m_bSceneChange)
	{
		TScene* pScene = m_pGame->Transition(this, EVENT_CLICK);
		pScene->Reset();
		m_bSceneChange = false;
	}*/
}

void   TSceneTitle::Init()
{
	m_pBGSound = I_Sound.Load(L"../../data/sound/abel_leaf.asf");
	if (m_pBGSound)
	{
		m_pBGSound->Play(true);
	}

	RECT rtBk = { 0, 0, 800.0f, 600.0f };
	m_bkScreen.Create(TDevice::m_pd3dDevice.Get(), 
		TDevice::m_pContext, rtBk,
		L"../../data/title.jpg",
		L"intro.txt");
	m_bkScreen.m_pSprite = nullptr;

	m_StartBtn.Create(TDevice::m_pd3dDevice.Get(),
		TDevice::m_pContext, { 280, 370, 520, 430 },
		L"../../data/ui/Zosma/Stage/Gameplay.png",
		L"Alphablend.hlsl");
	m_StartBtn.SetSelect();
	m_StartBtn.SetAnim(0.0f, I_Sprite.GetPtr(L"DefaultButton"));

	//m_UIList.push_back(button);
}
void    TSceneTitle::Frame()
{
	//if (TCollision::RectToPt(m_StartBtn.m_rt, I_Input.m_ptMousePos))
	//if (m_StartBtn.m_dwSelectState == T_HOVER)
	//{
	//	// hover			
	//	T_Math::FVector2 vScale = { 0.7f + (0.3f*(float)cos(g_fGameTime * 10) * 0.5f + 0.5f),
	//								0.7f + (0.3f * (float)cos(g_fGameTime * 10) * 0.5f + 0.5f )};
	//	m_StartBtn.SetScale(vScale);
	//	//m_StartBtn.SetRotate(g_fGameTime);
	//	m_StartBtn.SetTrans(m_StartBtn.m_vPos);
	//}
	m_StartBtn.Frame();

	
	for (auto& ui : m_UIList)
	{
		if (TCollision::RectToPt(ui.m_rt, I_Input.m_ptMousePos))
		{
			// hover			
			T_Math::FVector2 vScale = { 0.7f + (float)cos(g_fGameTime * 5) * 0.5f + 0.5f,
										0.7f + (float)cos(g_fGameTime * 5) * 0.5f + 0.5f };
			ui.SetScale(vScale);
			ui.SetRotate(g_fGameTime);
			ui.SetTrans(ui.m_vPos);

			// click
			if (I_Input.KeyCheck(VK_LBUTTON) == KEY_PUSH)
			{

			}
		}
		ui.Frame();
	}

	static float alpha = 0.0f;
	alpha += g_fSecondPerFrame*0.33f;
	for (auto& v : m_bkScreen.m_vList)
	{
		if (m_bFadeOut)
		{
			v.c.X = max(0.0f, 1.0f - alpha);
			v.c.Y = max(0.0f, 1.0f - alpha);
			v.c.Z = max(0.0f, 1.0f - alpha);
			v.c.W = max(0.0f, 1.0f - alpha);
		}
		else
		{
			v.c.X = 1.0f;
			v.c.Y = 1.0f;
			v.c.Z = 1.0f;
			v.c.W = min(1.0f, alpha); //fade in
		}
	}
	m_bkScreen.UpdateVertexBuffer();

	if (alpha > 0.5f && m_bFadeOut ==false)
	{
		m_bFadeOut = true;
		alpha = 0.0f;
	}

	if (alpha > 0.5f && m_bFadeOut)
	{
		m_pBGSound->Stop();
	}
}
void    TSceneTitle::Render()
{
	//m_bkScreen.SetViewTransform(m_Cam.GetMatrix());
	m_bkScreen.Render(TDevice::m_pContext);
	
	m_StartBtn.PreRender(TDevice::m_pContext);
	TDevice::m_pContext->PSSetShaderResources(0, 1,
		m_StartBtn.m_pSprite->GetSRV(
			m_StartBtn.m_dwSelectState).GetAddressOf());	
	m_StartBtn.PostRender(TDevice::m_pContext);

	m_StartBtn.m_dwSelectState = TSelectState::T_DEFAULT;
}
void    TSceneTitle::Release()
{
	m_bkScreen.Release();
	m_StartBtn.Release();

	for (auto& obj : m_UIList)
	{
		obj.Release();
	}
}