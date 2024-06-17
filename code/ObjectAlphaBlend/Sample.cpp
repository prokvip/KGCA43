#include "Sample.h"
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
void   Sample::Init()
{
	SetAlphaBlendState();

	std::wstring iconList[] =
	{
		L"../../data/S_KBSJoint.PNG",
		L"../../data/S_KHinge.PNG",
		L"../../data/S_KPrismatic.PNG",
		L"../../data/S_LevelSequence.PNG",
		L"../../data/S_NavP.PNG",
		L"../../data/S_Note.PNG",
		L"../../data/S_Pawn.PNG",
		L"../../data/S_Player.PNG",
		L"../../data/S_PortalActorIcon2.PNG",
		L"../../data/S_RadForce.PNG",
		L"../../data/S_SkyAtmosphere.PNG",
		L"../../data/S_SceneCaptureIcon.PNG",
		L"../../data/S_LevelSequence.PNG",
		L"../../data/S_SkyLight.PNG",
		L"../../data/S_Solver.PNG",
		L"../../data/S_TargetPoint.PNG",
		L"../../data/S_Terrain.PNG",
		L"../../data/S_Thruster.PNG",
		L"../../data/S_VectorFieldVol.PNG",
	};
	std::wstring texPath = L"../../data/"; 
	for (int iTex = 0; iTex < 10; iTex++)
	{	
		//std::wstring texFileName = texPath + L"숫자";
		//texFileName += std::to_wstring(0) + L".png";
		ComPtr<ID3D11ShaderResourceView> texSRV;
		//ComPtr<ID3D11Resource> m_pTexture = nullptr;
		HRESULT hr =
			DirectX::CreateWICTextureFromFile(
				m_pd3dDevice.Get(),
				iconList[iTex].c_str(),
				nullptr,
				texSRV.GetAddressOf());
		m_pNumber.push_back(texSRV);
		
	}


	T_Math::FVector2 a = { 10.0f, 0.0f };
	T_Math::FVector2 b = { 0.0f, 10.0f };
	float fDot = a | b;
	float fAngle = a.Angle(b);

	RECT rtBk = { -1000, -1000, 1000.0f, 1000.0f };
	objScreen.Create(m_pd3dDevice.Get(), m_pContext, rtBk, 
		L"../../data/1234.jpg",
		L"../../data/shader/Default.txt");

	m_UIList.resize(3);

	m_UIList[0].Create(m_pd3dDevice.Get(), m_pContext, { 0, 0, 100, 100 }, 
		L"../../data/kgca1.png",
		L"Alphablend.hlsl");
	m_UIList[1].Create(m_pd3dDevice.Get(), m_pContext, { 700, 0, 800, 100 },
		L"../../data/kgca2.png",
		L"Alphablend.hlsl");
	m_UIList[2].Create(m_pd3dDevice.Get(), m_pContext, { 700, 500, 800, 600 }, 
		L"../../data/kgca3.png",
		L"Alphablend.hlsl");
	
	//DrawRect = { 91, 1, 91+40, 1+60 }
	hero.Create(m_pd3dDevice.Get(), m_pContext, { 380, 270, 420, 330 }, 
		L"../../data/bitmap1Alpha.bmp",
		L"Alphablend.hlsl");
	hero.m_fSpeed = 500.0f;

	for (int iNpc = 0; iNpc < 10; iNpc++)
	{
		TNpc npc;
		T_Math::FVector2 pos;
		pos.X = randstep(0.0f, g_xClientSize);
		pos.Y = randstep(0.0f, g_yClientSize);
		npc.Create(m_pd3dDevice.Get(), m_pContext,
			{ (LONG)pos.X, (LONG)pos.Y,(LONG)(pos.X + 67.0f), (LONG)(pos.Y + 78.0f) }, 
			L"../../data/bitmap1Alpha.bmp",
			L"Alphablend.hlsl");
		m_npcList.push_back(npc);
	}
	m_iNpcCounter = m_npcList.size();

}
void    Sample::Frame()
{
	T_Math::FVector2 vCameraPos;
	m_Cam.SetTransform(vCameraPos);


	T_Math::FVector2 vPos = objScreen.m_vPos;
	T_Math::FVector2 vScale = { (float)cos(g_fGameTime) * 0.5f + 0.5f, (float)cos(g_fGameTime) * 0.5f + 0.5f };
	T_Math::FVector2 vCenter = { -800.0f * 0.5f, -600.0f * 0.5f };

	//objScreen.SetCenterMove(vCenter);
	//objScreen.SetScale(vScale);
	//objScreen.SetRotate(g_fGameTime);
	//objScreen.SetTrans(vPos);
	objScreen.Frame();


	for (auto& ui : m_UIList)
	{
		if(TCollision::RectToPt(ui.m_rt, m_Input.m_ptMousePos))
		{
			T_Math::FVector2 vScale = { 0.7f+(float)cos(g_fGameTime*5) * 0.5f + 0.5f, 
										0.7f+(float)cos(g_fGameTime*5) * 0.5f + 0.5f };
			ui.SetScale(vScale);
			//ui.SetRotate(g_fGameTime);
			ui.SetTrans(ui.m_vPos);
		}
		ui.Frame();
	}
	for (auto& npc : m_npcList)
	{
		if (npc.m_bDead==false && TCollision::RectToRect(npc.m_rt, hero.m_rt))
		{
			npc.m_bDead = true;
			m_iNpcCounter = max(1, m_iNpcCounter-1);
		}
	}

	
	if (m_Input.KeyCheck('W') == KEY_HOLD)
	{
		m_Cam.Up();
		//hero.Move({ 0.0f, -1.0f });
		//hero.Front();
	}
	if (m_Input.KeyCheck('S') == KEY_HOLD)
	{
		m_Cam.Down();
		//hero.Move({ 0.0f, 1.0f });
		//hero.Back();
	}
	if (m_Input.KeyCheck('A') == KEY_HOLD)
	{
		hero.Move({ -1.0f, 0.0f });
		//hero.Left();
		m_Cam.Right(-hero.m_vOffset.X);// +hero.m_vOffset;

	}
	if (m_Input.KeyCheck('D') == KEY_HOLD)
	{
		hero.Move({ 1.0f, 0.0f });
		//hero.Right();		
		m_Cam.Right(-hero.m_vOffset.X);
	}	
	
	m_Cam.Frame();
	

	hero.Frame();
	
	for (int iNpc = 0; iNpc < m_npcList.size(); iNpc++)
	{
		m_npcList[iNpc].Frame();
	}
}
void    Sample::Render() 
{ 		
	//m_pContext->OMSetBlendState(m_pAlphaBlend, 0, -1);

	objScreen.SetTransform(m_Cam.GetMatrix());
	objScreen.Render(m_pContext);

	for_each(begin(m_UIList), end(m_UIList), [&](auto& obj) 
	{
		//if (!obj.m_bDead)
		{
			//obj.SetTransform(m_Cam.GetMatrix());
			obj.PreRender(m_pContext);
			m_pContext->PSSetShaderResources(0, 1, m_pNumber[m_iNpcCounter-1].GetAddressOf());
			obj.PostRender(m_pContext);
		}
	});	

	hero.SetTransform(m_Cam.GetMatrix());
	hero.Render(m_pContext);

	bool bGameEnding = true;
	for (int iNpc = 0; iNpc < m_npcList.size(); iNpc++)
	{
		if (!m_npcList[iNpc].m_bDead)
		{
			m_npcList[iNpc].SetTransform(m_Cam.GetMatrix());
			m_npcList[iNpc].Render(m_pContext);
			bGameEnding = false;
		}
	}
	m_bGameRun = !bGameEnding;
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