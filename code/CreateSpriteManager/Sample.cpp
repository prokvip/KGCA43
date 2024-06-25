#include "Sample.h"
#include "TTexMgr.h"

HRESULT  Sample::SetAlphaBlendState()
{
	HRESULT hr;
	D3D11_BLEND_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));

	bd.AlphaToCoverageEnable = FALSE;//discard;같은 결과.
	bd.IndependentBlendEnable = FALSE;
	//D3D11_RENDER_TARGET_BLEND_DESC RenderTarget[8];
	// 백버퍼의 컬러값(DestBlend) 과  현재 출력 컬러(SrcBlend)값을 혼합연산한다.
	bd.RenderTarget[0].BlendEnable=TRUE;
	// RGA 컬러값 연산( 기본 알파블랭딩 공식) 알파범위( 0 ~ 1 )
	// 최종 컬러값 = 소스컬러*소스알파 	+  데스크컬러* (1.0 - 소스알파)
	//  정점위치 = 목적지위치*S + 현재위치* (1- S); S=0, S=0.5, S = 1
	// 
	// 만약 소스컬러 = 1,0,0,1(빨강)   배경컬러 = 0,0,1,1(파랑)
	// 1)소스알파 = 1.0F ( 완전불투명)
		// RGB = R*ALPHA, G = G*Alpha, B = B*Alpha
		// 최종 컬러값 = 빨강*1.0F 	+  파랑* (1.0 - 1.0F)
		// ->최종 컬러값(소스색) = [1,0,0] 	+  [0,0,0]
	// 2)소스알파 = 0.0F ( 완전투명)
		// RGB = R*ALPHA, G = G*Alpha, B = B*Alpha
		// 최종 컬러값 = 빨강*0.0F 	+  파랑* (1.0 - 0.0F)
		// ->최종 컬러값(배경색) = [0,0,0] +  [0,0,1]
	bd.RenderTarget[0].SrcBlend		= D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend    = D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOp		= D3D11_BLEND_OP_ADD;
	// A 알파값 연산
	// A = SRC Alpha*1 + DestAlpha*0;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	bd.RenderTarget[0].RenderTargetWriteMask=
		D3D11_COLOR_WRITE_ENABLE_ALL;

	hr=m_pd3dDevice->CreateBlendState(&bd , m_pAlphaBlend.GetAddressOf());
	if (SUCCEEDED(hr))
	{
		m_pContext->OMSetBlendState(m_pAlphaBlend.Get(), 0,-1);
	}
	return hr;
}
void   Sample::SetSound()
{
	I_Sound.Set(nullptr, nullptr);
	m_pBGSound = I_Sound.Load(L"../../data/sound/romance.mid");
	if (m_pBGSound)
	{
		m_pBGSound->Play(true);
	}
	m_pEffectSound = I_Sound.Load(L"../../data/sound/GunShot.mp3").get();

}
void   Sample::SetUI()
{
	RECT rtBk = { -1000, -1000, 1000.0f, 1000.0f };
	objScreen.Create(m_pd3dDevice.Get(), m_pContext, rtBk,
		L"../../data/1234.jpg",
		L"../../data/shader/Default.txt");
	objScreen.m_pSprite = nullptr;

	m_UIList.resize(4);
	m_UIList[0].Create(m_pd3dDevice.Get(), m_pContext, { 0, 0, 100, 100 },
		L"../../data/kgca1.png",
		L"Alphablend.hlsl");
	m_UIList[0].SetAnim(1.0f, I_Sprite.GetPtr(L"DefalultNumber"));

	m_UIList[1].Create(m_pd3dDevice.Get(), m_pContext, { 700, 0, 800, 100 },
		L"../../data/kgca1.png",
		L"Alphablend.hlsl");
	m_UIList[1].SetAnim(20.0f, I_Sprite.GetPtr(L"IconList"));

	m_UIList[2].Create(m_pd3dDevice.Get(), m_pContext,
		{ 700, 500, 800, 600 },
		L"../../data/kgca1.png",
		L"Alphablend.hlsl");
	m_UIList[2].SetAnim(1.0f, I_Sprite.GetPtr(L"Fog"));

	m_UIList[3].Create(m_pd3dDevice.Get(), m_pContext,
		{ 0, 500, 100, 600 },
		L"../../data/Effect/slashFire_4x4.png",
		L"Alphablend.hlsl");
	m_UIList[3].SetAnim(1.0f, I_Sprite.GetPtr(L"wik"));
}
void   Sample::SetPlayer()
{
	//DrawRect = { 91, 1, 91+40, 1+60 }
	hero.Create(m_pd3dDevice.Get(), m_pContext, { 380, 270, 420, 330 },
		L"../../data/Sprite/bitmap1Alpha.bmp",
		L"Alphablend.hlsl");
	hero.m_fSpeed = 500.0f;
}


void    Sample::LevelUp(UINT iLevel)
{
	Sleep(1000);
	std::vector<std::wstring> spriteName =
	{
		L"DefalultNumber",
		L"wik",
		L"rtClash",
		L"IconList",
		L"rtBomb",
		L"rtExplosion",
	};

	hero.SetVertexData({ 380, 270, 420, 330 });
	m_Cam.m_vCameraPos = { 0.0f,0.0f };
	for (int iNpc = 0; iNpc < iLevel; iNpc++)
	{
		TNpc npc;
		T_Math::FVector2 pos;
		pos.X = randstep(0.0f, g_xClientSize);
		pos.Y = randstep(0.0f, g_yClientSize);
		npc.Create(m_pd3dDevice.Get(), m_pContext,
			{ (LONG)pos.X, (LONG)pos.Y,(LONG)(pos.X + 67.0f), (LONG)(pos.Y + 78.0f) },
			L"../../data/Sprite/bitmap1Alpha.bmp",
			L"Alphablend.hlsl");

		npc.SetAnim(1.0f, I_Sprite.GetPtr(spriteName[m_iLevel% spriteName.size()]));
		m_npcList.push_back(npc);
	}
	m_iNpcCounter = m_npcList.size();
}
void   Sample::Init()
{
	I_Sprite.Load(L"../../data/Sprite/SpriteInfo.txt");
	SetSound();
	SetAlphaBlendState();
	SetUI();
	SetPlayer();
	LevelUp(m_iLevel);
}

void    Sample::Frame()
{
	I_Sound.Frame();
	if (m_iNpcCounter <= 9)
	{
		if (I_Input.KeyCheck(VK_F1) == KEY_PUSH)
		{
			TNpc npc;
			T_Math::FVector2 pos;
			pos.X = randstep(0.0f, g_xClientSize);
			pos.Y = randstep(0.0f, g_yClientSize);
			npc.Create(m_pd3dDevice.Get(), m_pContext,
				{ (LONG)pos.X, (LONG)pos.Y,(LONG)(pos.X + 67.0f), (LONG)(pos.Y + 78.0f) },
				L"../../data/Sprite/bitmap1Alpha.bmp",
				L"Alphablend.hlsl");
			npc.SetAnim(1.0f, I_Sprite.GetPtr(L"DefalultNumber"));
			m_npcList.push_back(npc);
			m_iNpcCounter = m_npcList.size();
		}
	}	
	
	//T_Math::FVector2 vPos = objScreen.m_vPos;
	//T_Math::FVector2 vScale = { (float)cos(g_fGameTime) * 0.5f + 0.5f, (float)cos(g_fGameTime) * 0.5f + 0.5f };
	//T_Math::FVector2 vCenter = { -800.0f * 0.5f, -600.0f * 0.5f };

	////objScreen.SetCenterMove(vCenter);
	////objScreen.SetScale(vScale);
	////objScreen.SetRotate(g_fGameTime);
	////objScreen.SetTrans(vPos);
	objScreen.Frame();


	for (auto& ui : m_UIList)
	{
		if(TCollision::RectToPt(ui.m_rt, I_Input.m_ptMousePos))
		{
			T_Math::FVector2 vScale = { 0.7f+(float)cos(g_fGameTime*5) * 0.5f + 0.5f, 
										0.7f+(float)cos(g_fGameTime*5) * 0.5f + 0.5f };
			ui.SetScale(vScale);
			ui.SetRotate(g_fGameTime);
			ui.SetTrans(ui.m_vPos);
		}
		ui.Frame();
	}

	for (auto& npc : m_npcList)
	{
		npc.Frame();
		if (npc.m_bDead==false && TCollision::RectToRect(npc.m_rt, hero.m_rt))
		{
			npc.m_bDead = true;
			m_iNpcCounter = max(0, m_iNpcCounter-1);
		}
	}
	for (auto iter = m_npcList.begin(); iter != m_npcList.end(); )
	{
		if ((*iter).m_bDead == true )
		{
			iter = m_npcList.erase(iter);
		}
		else
		{
			iter++;
		}
	}

	m_Cam.Up();
	hero.Frame();
	
	m_Cam.Left(-hero.m_vOffset.X);// +hero.m_vOffset;
	m_Cam.m_vCameraPos.Y = m_Cam.m_vCameraPos.Y + -hero.m_vOffset.Y;
	m_Cam.Frame();
	
}
void    Sample::Render() 
{ 		
	I_Sound.Render();
	m_pContext->OMSetBlendState(m_pAlphaBlend.Get(), 0, -1);

	objScreen.SetViewTransform(m_Cam.GetMatrix());
	objScreen.Render(m_pContext);

	m_UIList[0].PreRender(m_pContext);
	m_pContext->PSSetShaderResources(0, 1,
		m_UIList[0].m_pSprite->GetSRV(m_iNpcCounter).GetAddressOf());
	m_UIList[0].PostRender(m_pContext);

	//for (auto obj : m_UIList)
	for ( int iUI = 1; iUI < m_UIList.size(); iUI++)
	{
		m_UIList[iUI].UpdateSprite();
		// 화면 고정( 뷰 변환 생략 )
		//obj.SetViewTransform(m_Cam.GetMatrix());
		m_UIList[iUI].Render(m_pContext);
	}	
	
	/// <summary>
	/// 
	/// </summary>
	hero.SetViewTransform(m_Cam.GetMatrix());
	hero.Render(m_pContext);

	bool bGameEnding = true;
	
	for_each(begin(m_npcList), end(m_npcList), [&](auto& obj)
		{
			if (!obj.m_bDead)
			{				
				obj.UpdateSprite();
				obj.SetViewTransform(m_Cam.GetMatrix());
				obj.Render(m_pContext);
				bGameEnding = false;			
			}
		});
	//m_bGameRun = !bGameEnding;

	// 하단 화면을 벗어나면.
	if (g_yClientSize <= hero.m_vList[2].p.Y)
	{
		m_bGameRun = false;
	}

	// 적 격퇴
	if (bGameEnding)
	{
		m_iLevel++;
		LevelUp(m_iLevel);
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

	I_Sound.Release();
	I_Shader.Release();
	I_Tex.Release();
}

T_GAME_START(800, 600);