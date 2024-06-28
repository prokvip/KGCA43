#include "TSceneStart.h"
#include "TGame.h"
void    TSceneStart::Execute()
{
	m_fEventTimer += g_fSecondPerFrame;
	if (m_fEventTimer > 3.0f)
	{
		TScene* pScene = m_pGame->Transition(this, EVENT_TIMER);
		pScene->Reset();
		m_fEventTimer = 0.0f;
	}
}
void   TSceneStart::Init()
{
	RECT rtBk = { 0, 0, 800.0f, 600.0f };
	m_bkScreen.Create(TDevice::m_pd3dDevice.Get(),
		TDevice::m_pContext, rtBk,
		L"../../data/kgca3.png",
		L"intro.txt");
	m_bkScreen.SetAnim(3.0f, I_Sprite.GetPtr(L"kgca"));
}
void    TSceneStart::Frame()
{
	T_Math::FVector2 vScale = { 0.5f + m_fEventTimer / 3.0f,
								0.5f + m_fEventTimer / 3.0f };
	m_bkScreen.SetScale(vScale);
	m_bkScreen.Frame();
}
void    TSceneStart::Render()
{
	m_bkScreen.UpdateSprite();
	//m_bkScreen.SetViewTransform(m_Cam.GetMatrix());
	m_bkScreen.Render(TDevice::m_pContext);
}
void    TSceneStart::Release()
{
	m_bkScreen.Release();
}
TSceneStart::TSceneStart(TGame* pGame) : TScene(pGame)
{
	m_iStateIndex = GAME_START;
}