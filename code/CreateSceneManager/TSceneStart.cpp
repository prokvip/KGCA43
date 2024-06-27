#include "TSceneStart.h"
#include "TGame.h"
void    TSceneStart::Execute()
{
	m_fEventTimer += g_fSecondPerFrame;
	if (m_fEventTimer > 5.0f)
	{
		int iOutState = m_pGame->Transition(this, EVENT_TIMER);
		m_fEventTimer = 0.0f;
	}
}
void   TSceneStart::Init()
{
	RECT rtBk = { 0, 0, 800.0f, 600.0f };
	objScreen.Create(TDevice::m_pd3dDevice.Get(),
		TDevice::m_pContext, rtBk,
		L"../../data/kgca3.png",
		L"intro.txt");
	objScreen.SetAnim(5.0f, I_Sprite.GetPtr(L"kgca"));
}
void    TSceneStart::Frame()
{
	T_Math::FVector2 vScale = { 0.5f + m_fEventTimer / 3.0f,
								0.5f + m_fEventTimer / 3.0f };
	objScreen.SetScale(vScale);
	objScreen.Frame();
}
void    TSceneStart::Render()
{
	objScreen.UpdateSprite();
	//objScreen.SetViewTransform(m_Cam.GetMatrix());
	objScreen.Render(TDevice::m_pContext);
}
void    TSceneStart::Release()
{
	objScreen.Release();
}
TSceneStart::TSceneStart(TGame* pGame) : TScene(pGame)
{
	m_iStateIndex = GAME_START;
}