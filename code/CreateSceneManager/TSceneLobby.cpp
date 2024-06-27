#include "TSceneLobby.h"
#include "TInput.h"
#include "TGame.h"
void    TSceneLobby::Execute()
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
		int iOutState = m_pGame->Transition(this, EVENT_CLICK);
		m_bSceneChange = false;
	}
}
void   TSceneLobby::Init()
{
	RECT rtBk = { 0, 0, 800.0f, 600.0f };
	objScreen.Create(TDevice::m_pd3dDevice.Get(),
		TDevice::m_pContext, rtBk,
		L"../../data/lobby.png",
		L"intro.txt");
	//objScreen.SetAnim(3.0f, I_Sprite.GetPtr(L"kgca"));
	m_StartBtn.Create(TDevice::m_pd3dDevice.Get(),
		TDevice::m_pContext, { 500, 500, 600, 550 },
		L"../../data/ui/Zosma/Stage/Gameplay.png",
		L"Alphablend.hlsl");

	if (TCollision::RectToPt(m_StartBtn.m_rt, I_Input.m_ptMousePos))
	{
		// hover			
		T_Math::FVector2 vScale = { 0.7f + (0.3f * (float)cos(g_fGameTime * 10) * 0.5f + 0.5f),
									0.7f + (0.3f * (float)cos(g_fGameTime * 10) * 0.5f + 0.5f) };
		m_StartBtn.SetScale(vScale);
		//m_StartBtn.SetRotate(g_fGameTime);
		m_StartBtn.SetTrans(m_StartBtn.m_vPos);
	}
	m_StartBtn.Frame();
}
void    TSceneLobby::Frame()
{
	if (TCollision::RectToPt(m_StartBtn.m_rt, I_Input.m_ptMousePos))
	{
		// hover			
		T_Math::FVector2 vScale = { 0.7f + (0.3f * (float)cos(g_fGameTime * 10) * 0.5f + 0.5f),
									0.7f + (0.3f * (float)cos(g_fGameTime * 10) * 0.5f + 0.5f) };
		m_StartBtn.SetScale(vScale);
		//m_StartBtn.SetRotate(g_fGameTime);
		m_StartBtn.SetTrans(m_StartBtn.m_vPos);
	}
	m_StartBtn.Frame();
	objScreen.Frame();
}
void    TSceneLobby::Render()
{
	objScreen.UpdateSprite();
	//objScreen.SetViewTransform(m_Cam.GetMatrix());
	objScreen.Render(TDevice::m_pContext);

	m_StartBtn.PreRender(TDevice::m_pContext);
	/*TDevice::m_pContext->PSSetShaderResources(0, 1,
		m_StartBtn.m_pSprite->GetSRV(m_iNpcCounter).GetAddressOf());*/
	m_StartBtn.PostRender(TDevice::m_pContext);
}
void    TSceneLobby::Release()
{
	objScreen.Release();
	m_StartBtn.Release();
}
TSceneLobby::TSceneLobby(TGame* pGame) : TScene(pGame)
{
	m_iStateIndex = GAME_LOBBY;
}