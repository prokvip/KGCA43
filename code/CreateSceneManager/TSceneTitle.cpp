#include "TSceneTitle.h"
#include "TInput.h"
#include "TGame.h"
// GAME_TITLE	-> EVENT_CLICK			-> GAME_INGAME
// GAME_TITLE	-> EVENT_TIMER:3		-> GAME_INGAME
void    TSceneTitle::Execute()
{	
	if (TCollision::RectToPt(m_StartBtn.m_rt, I_Input.m_ptMousePos))
	{			
		if (I_Input.KeyCheck(VK_LBUTTON) == KEY_PUSH)
		{
			m_bSceneChange = true;
		}
	}

	if (m_bSceneChange)
	{
		int iOutState =
			m_pSceneFSM->GetTransition(
				m_iStateIndex, EVENT_CLICK);
		m_pGame->m_pCurrentScene = m_pGame->m_SceneList[iOutState];	
		m_bSceneChange = false;
	}
}

void   TSceneTitle::Init()
{
	m_pBGSound = I_Sound.Load(L"../../data/sound/abel_leaf.asf");
	if (m_pBGSound)
	{
		m_pBGSound->Play(true);
	}

	RECT rtBk = { 0, 0, 800.0f, 600.0f };
	objScreen.Create(TDevice::m_pd3dDevice.Get(), 
		TDevice::m_pContext, rtBk,
		L"../../data/title.png",
		L"intro.txt");
	objScreen.m_pSprite = nullptr;

	m_StartBtn.Create(TDevice::m_pd3dDevice.Get(),
		TDevice::m_pContext, { 280, 370, 520, 430 },
		L"../../data/ui/Zosma/Stage/Gameplay.png",
		L"Alphablend.hlsl");
	//m_StartBtn.SetAnim(1.0f, nullptr);

	//m_UIList.push_back(button);
}
void    TSceneTitle::Frame()
{
	if (TCollision::RectToPt(m_StartBtn.m_rt, I_Input.m_ptMousePos))
	{
		// hover			
		T_Math::FVector2 vScale = { 0.7f + (0.3f*(float)cos(g_fGameTime * 10) * 0.5f + 0.5f),
									0.7f + (0.3f * (float)cos(g_fGameTime * 10) * 0.5f + 0.5f )};
		m_StartBtn.SetScale(vScale);
		//m_StartBtn.SetRotate(g_fGameTime);
		m_StartBtn.SetTrans(m_StartBtn.m_vPos);
	}
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
	for (auto& v : objScreen.m_vList)
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
	objScreen.UpdateVertexBuffer();

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
	//objScreen.SetViewTransform(m_Cam.GetMatrix());
	objScreen.Render(TDevice::m_pContext);
	
	m_StartBtn.PreRender(TDevice::m_pContext);
	/*TDevice::m_pContext->PSSetShaderResources(0, 1,
		m_StartBtn.m_pSprite->GetSRV(m_iNpcCounter).GetAddressOf());*/
	m_StartBtn.PostRender(TDevice::m_pContext);
}
void    TSceneTitle::Release()
{
	objScreen.Release();
	m_StartBtn.Release();

	for (auto& obj : m_UIList)
	{
		obj.Release();
	}
}