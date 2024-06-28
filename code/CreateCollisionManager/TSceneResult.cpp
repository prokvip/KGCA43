#include "TSceneResult.h"
#include "TInput.h"
#include "TGame.h"
void    TSceneResult::Execute()
{
	m_fEventTimer += g_fSecondPerFrame;
	if (m_fEventTimer > 10.0f)
	{
		TScene* pScene = m_pGame->Transition(this, EVENT_TIMER);
		pScene->Reset();		
		m_fEventTimer = 0.0f;
	}
	if (TCollision::RectToPt(m_StartBtn.m_rt, I_Input.m_ptMousePos))
	{
		if (I_Input.KeyCheck(VK_LBUTTON) == KEY_PUSH)
		{
			m_bSceneChange = true;
		}
	}

	if (m_bSceneChange)
	{
		TScene* pScene = m_pGame->Transition(this, EVENT_CLICK);
		pScene->Reset();
		m_bSceneChange = false;
	}
}
void   TSceneResult::Init()
{
	RECT rtBk = { 0, 0, 800.0f, 600.0f };
	m_bkScreen.Create(TDevice::m_pd3dDevice.Get(),
		TDevice::m_pContext, rtBk,
		L"../../data/result.png",
		L"intro.txt");	
	m_StartBtn.Create(TDevice::m_pd3dDevice.Get(),
		TDevice::m_pContext, { 280, 370, 520, 430 },
		L"../../data/ui/Zosma/Stage/Gameplay.png",
		L"Alphablend.hlsl");
}
void    TSceneResult::Frame()
{
	m_bkScreen.Frame();
	if (TCollision::RectToPt(m_StartBtn.m_rt, I_Input.m_ptMousePos))
	{
		// hover			
		T_Math::FVector2 vScale = { 0.7f + (0.3f * (float)cos(g_fGameTime * 10) * 0.5f + 0.5f),
									0.7f + (0.3f * (float)cos(g_fGameTime * 10) * 0.5f + 0.5f) };
		m_StartBtn.SetScale(vScale);
		m_StartBtn.SetRotate(g_fGameTime);
		m_StartBtn.SetTrans(m_StartBtn.m_vPos);
	}
	m_StartBtn.Frame();
}
void    TSceneResult::Render()
{
	m_bkScreen.UpdateSprite();
	//m_bkScreen.SetViewTransform(m_Cam.GetMatrix());
	m_bkScreen.Render(TDevice::m_pContext);

	m_StartBtn.PreRender(TDevice::m_pContext);
	/*TDevice::m_pContext->PSSetShaderResources(0, 1,
		m_StartBtn.m_pSprite->GetSRV(m_iNpcCounter).GetAddressOf());*/
	m_StartBtn.PostRender(TDevice::m_pContext);
}
void    TSceneResult::Release()
{
	m_bkScreen.Release();
}
TSceneResult::TSceneResult(TGame* pGame) : TScene(pGame)
{
	m_iStateIndex = GAME_RESULT;
}