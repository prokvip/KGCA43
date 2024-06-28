#include "TSceneIngame.h"
#include "TStd.h"
#include "TInput.h"
#include "TGame.h"
#include "TObjectMgr.h"
void    TSceneIngame::Execute()
{
	m_fEventTimer += g_fSecondPerFrame;
	if (m_fEventTimer > 30.0f)
	{
		TScene* pScene = m_pGame->Transition(this, EVENT_TIMER);
		pScene->Reset();
		m_fEventTimer = 0.0f;
	}
	if (m_bMissionComplite)
	{
		TScene* pScene = m_pGame->Transition(this, EVENT_MISSION_COMPLETE);
		pScene->Reset();
		m_fEventTimer = 0.0f;
	}
}
void   TSceneIngame::SetSound()
{	
	m_pBGSound = I_Sound.Load(L"../../data/sound/romance.mid");
	if (m_pBGSound)
	{
		//m_pBGSound->Play(true);
	}
	m_pEffectSound = I_Sound.Load(L"../../data/sound/GunShot.mp3").get();

}
void   TSceneIngame::SetUI()
{
	RECT rtBk = { -1000, -1000, 1000.0f, 1000.0f };
	m_bkScreen.Create(TDevice::m_pd3dDevice.Get(), TDevice::m_pContext, rtBk,
		L"../../data/1234.jpg",
		L"../../data/shader/Default.txt");
	m_bkScreen.m_pSprite = nullptr;

	m_UIList.resize(4);
	m_UIList[0].Create(TDevice::m_pd3dDevice.Get(), TDevice::m_pContext, { 0, 0, 100, 100 },
		L"../../data/kgca1.png",
		L"Alphablend.hlsl");
	m_UIList[0].SetAnim(1.0f, I_Sprite.GetPtr(L"DefalultNumber"));

	m_UIList[1].Create(TDevice::m_pd3dDevice.Get(), TDevice::m_pContext, { 700, 0, 800, 100 },
		L"../../data/kgca1.png",
		L"Alphablend.hlsl");
	m_UIList[1].SetAnim(20.0f, I_Sprite.GetPtr(L"IconList"));

	m_UIList[2].Create(TDevice::m_pd3dDevice.Get(), TDevice::m_pContext,
		{ 700, 500, 800, 600 },
		L"../../data/kgca1.png",
		L"Alphablend.hlsl");
	m_UIList[2].SetAnim(1.0f, I_Sprite.GetPtr(L"Fog"));

	m_UIList[3].Create(TDevice::m_pd3dDevice.Get(), TDevice::m_pContext,
		{ 0, 500, 100, 600 },
		L"../../data/Effect/slashFire_4x4.png",
		L"Alphablend.hlsl");
	m_UIList[3].SetAnim(1.0f, I_Sprite.GetPtr(L"wik"));

	// game timer
	m_UISceneTimer.resize(2);
	m_UISceneTimer[0].Create(TDevice::m_pd3dDevice.Get(), TDevice::m_pContext,
		{ 300, 0, 400, 100 },
		L"../../data/Effect/slashFire_4x4.png",
		L"Alphablend.hlsl");
	m_UISceneTimer[0].SetAnim(100.0f, I_Sprite.GetPtr(L"DefalultNumber"));

	m_UISceneTimer[1].Create(TDevice::m_pd3dDevice.Get(), TDevice::m_pContext,
		{ 400, 0, 500, 100 },
		L"../../data/Effect/slashFire_4x4.png",
		L"Alphablend.hlsl");
	m_UISceneTimer[1].SetAnim(10.0f, I_Sprite.GetPtr(L"DefalultNumber"));
}
void   TSceneIngame::SetPlayer()
{
	//DrawRect = { 91, 1, 91+40, 1+60 }
	m_Hero.Create(TDevice::m_pd3dDevice.Get(), TDevice::m_pContext, { 380, 270, 420, 330 },
		L"../../data/Sprite/bitmap1Alpha.bmp",
		L"Alphablend.hlsl");
	m_Hero.m_fSpeed = 500.0f;
}
void    TSceneIngame::LevelUp(UINT iLevel)
{
	Sleep(100);	

	m_fEventTimer = 0.0f;
	std::vector<std::wstring> spriteName =
	{
		L"DefalultNumber",
		L"wik",
		L"rtClash",
		L"IconList",
		L"rtBomb",
		L"rtExplosion",
	};	

	m_npcList.resize(iLevel);

	for (int iNpc = 0; iNpc < iLevel; iNpc++)
	{
		//TNpc npc;
		T_Math::FVector2 pos;
		pos.X = randstep(0.0f, g_xClientSize);
		pos.Y = randstep(0.0f, g_yClientSize);
		m_npcList[iNpc].Create(TDevice::m_pd3dDevice.Get(), TDevice::m_pContext,
			{ (LONG)pos.X, (LONG)pos.Y,(LONG)(pos.X + 67.0f), (LONG)(pos.Y + 78.0f) },
			L"../../data/Sprite/bitmap1Alpha.bmp",
			L"Alphablend.hlsl");		
		m_npcList[iNpc].SetAnim(1.0f, I_Sprite.GetPtr(spriteName[m_iLevel % spriteName.size()]));
		m_npcList[iNpc].SetCollision();
		//m_npcList.push_back(npc);
	}
	m_iNpcCounter = m_npcList.size();
	/*for (int iNpc = 0; iNpc < m_npcList.size(); iNpc++)
	{
		m_npcList[iNpc].SetCollision();
	}*/

	m_UISceneTimer[0].ResetSpriteData();
	m_UISceneTimer[1].ResetSpriteData();
	m_UISceneTimer[0].m_SpriteData.m_iAnimIndex = 2;
	m_UISceneTimer[1].m_SpriteData.m_iAnimIndex = 9;

	m_fSceneTime = 0.0f;
	m_Hero.SetVertexData({ 380, 270, 420, 330 });
	m_Cam.m_vCameraPos = { 0.0f, 0.0f };
}
void   TSceneIngame::Reset()
{		
	I_ObjMgr.Release();

	for (int iNpc = 0; iNpc < m_npcList.size(); iNpc++)
	{
		m_npcList[iNpc].Release();
	}
	m_npcList.clear();
	m_bMissionComplite = m_bWaveComplite = false;

	m_iLevel = 1;

	LevelUp(m_iLevel);	

	m_Hero.SetCollision();
	for (auto& ui : m_UIList)
	{
		ui.SetSelect();
	}
	for (int iNpc = 0; iNpc < m_npcList.size(); iNpc++)
	{
		m_npcList[iNpc].SetCollision();
	}
}
void   TSceneIngame::Init()
{
	SetSound();
	SetUI();
	SetPlayer();
	LevelUp(m_iLevel);
}
void    TSceneIngame::Frame()
{
	I_ObjMgr.Frame();
	m_fSceneTime += g_fSecondPerFrame;

	if (m_iNpcCounter <= 9)
	{
		if (I_Input.KeyCheck(VK_F1) == KEY_PUSH)
		{			
			TNpc npc;
			T_Math::FVector2 pos;
			pos.X = randstep(0.0f, g_xClientSize);
			pos.Y = randstep(0.0f, g_yClientSize);
			npc.Create(TDevice::m_pd3dDevice.Get(), TDevice::m_pContext,
				{ (LONG)pos.X, (LONG)pos.Y,(LONG)(pos.X + 67.0f), (LONG)(pos.Y + 78.0f) },
				L"../../data/Sprite/bitmap1Alpha.bmp",
				L"Alphablend.hlsl");
			npc.SetAnim(1.0f, I_Sprite.GetPtr(L"DefalultNumber"));
			m_npcList.push_back(npc);
			m_iNpcCounter = m_npcList.size();
		}
		m_npcList[m_npcList.size()].SetCollision();
	}

	//T_Math::FVector2 vPos = m_bkScreen.m_vPos;
	//T_Math::FVector2 vScale = { (float)cos(g_fGameTime) * 0.5f + 0.5f, (float)cos(g_fGameTime) * 0.5f + 0.5f };
	//T_Math::FVector2 vCenter = { -800.0f * 0.5f, -600.0f * 0.5f };

	////m_bkScreen.SetCenterMove(vCenter);
	////m_bkScreen.SetScale(vScale);
	////m_bkScreen.SetRotate(g_fGameTime);
	////m_bkScreen.SetTrans(vPos);
	m_bkScreen.Frame();


	for (auto& ui : m_UIList)
	{
		if (ui.m_dwSelectState == T_ACTIVE )//TCollision::RectToPt(ui.m_rt, I_Input.m_ptMousePos))
		{
			T_Math::FVector2 vScale = { 0.7f + (float)cos(g_fGameTime * 5) * 0.5f + 0.5f,
										0.7f + (float)cos(g_fGameTime * 5) * 0.5f + 0.5f };
			ui.SetScale(vScale);
			ui.SetRotate(g_fGameTime);
			ui.SetTrans(ui.m_vPos);
		}
		ui.Frame();
	}
	for (auto& ui : m_UISceneTimer)
	{
		/*if (TCollision::RectToPt(ui.m_rt, I_Input.m_ptMousePos))
		{
			T_Math::FVector2 vScale = { 0.7f + (float)cos(g_fGameTime * 5) * 0.5f + 0.5f,
										0.7f + (float)cos(g_fGameTime * 5) * 0.5f + 0.5f };
			ui.SetScale(vScale);
			ui.SetRotate(g_fGameTime);
			ui.SetTrans(ui.m_vPos);
		}*/
		ui.Frame();
	}

	for (auto& npc : m_npcList)
	{
		npc.Frame();
		if (npc.m_bDead == true)//&& TCollision::RectToRect(npc.m_rt, m_Hero.m_rt))
		{
			m_iNpcCounter = max(0, m_iNpcCounter - 1);
		}
	}
	for (auto iter = m_npcList.begin(); iter != m_npcList.end(); )
	{
		if ((*iter).m_bDead == true)
		{
			(*iter).Release();
			iter = m_npcList.erase(iter);
		}
		else
		{
			iter++;
		}
	}

	//m_Cam.Up();
	m_Hero.Frame();
	m_Cam.Move(m_Hero.m_vOffset);
	m_Cam.Frame();
}
void    TSceneIngame::Render()
{
	m_bkScreen.SetViewTransform(m_Cam.GetMatrix());
	m_bkScreen.Render(TDevice::m_pContext);

	m_UIList[0].PreRender(TDevice::m_pContext);
	TDevice::m_pContext->PSSetShaderResources(0, 1,
		m_UIList[0].m_pSprite->GetSRV(m_iNpcCounter).GetAddressOf());
	m_UIList[0].PostRender(TDevice::m_pContext);

	//for (auto obj : m_UIList)
	for (int iUI = 1; iUI < m_UIList.size(); iUI++)
	{
		m_UIList[iUI].UpdateSprite();
		// 화면 고정( 뷰 변환 생략 )
		//obj.SetViewTransform(m_Cam.GetMatrix());
		m_UIList[iUI].Render(TDevice::m_pContext);
	}
	for (auto& obj : m_UISceneTimer)
	{
		obj.UpdateSprite(-1.0f);
		// 화면 고정( 뷰 변환 생략 )
		//obj.SetViewTransform(m_Cam.GetMatrix());
		obj.Render(TDevice::m_pContext);
	}

	/// <summary>
	/// 
	/// </summary>
	m_Hero.SetViewTransform(m_Cam.GetMatrix());
	m_Hero.Render(TDevice::m_pContext);	

	for_each(begin(m_npcList), end(m_npcList), [&](auto& obj)
		{
			if (!obj.m_bDead)
			{
				obj.UpdateSprite();
				obj.SetViewTransform(m_Cam.GetMatrix());
				obj.Render(TDevice::m_pContext);
			}
		});

	// 적 격퇴
	if (m_npcList.size() == 0 )
	{
		m_bWaveComplite = true;
		m_fSceneTime = 0.0f;

		if (m_iLevel >= 9)
		{
			m_bMissionComplite = true;
		}
		else
		{
			m_iLevel++;
			LevelUp(m_iLevel);
		}
	}
}
void    TSceneIngame::Release()
{	
	for (auto& obj : m_UIList)
	{
		obj.Release();
	}
	for (auto& obj : m_UISceneTimer)
	{
		obj.Release();
	}	
	for (int iNpc = 0; iNpc < m_npcList.size(); iNpc++)
	{
		m_npcList[iNpc].Release();
	}

	m_bkScreen.Release();
	m_Hero.Release();
	m_UIList.clear();
	m_UISceneTimer.clear();
	m_npcList.clear();
}

TSceneIngame::TSceneIngame(TGame* pGame) : TScene(pGame)
{
	m_iStateIndex = GAME_INGAME;
}