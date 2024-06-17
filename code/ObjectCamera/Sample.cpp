#include "Sample.h"

void   Sample::Init()
{
	T_Math::FMatrix m1;
	T_Math::FMatrix m2;
	T_Math::FMatrix m3 = m1 * m2;
	T_Math::FMatrix matTrans;
	T_Math::FMatrix matScale;
	matTrans.Translation({ 100, 20 });
	matScale.Scale({ 2, 0.5f});
	T_Math::FMatrix matRotate;
	matRotate.rotate(TBASIS_PI/2.0f);


	std::wstring texPath = L"../../data/Number/"; //0.bmp
	for (int iTex = 0; iTex < 10; iTex++)
	{	
		std::wstring texFileName = texPath +
		std::to_wstring(iTex) + L".bmp";
		ComPtr<ID3D11ShaderResourceView> texSRV;
		//ComPtr<ID3D11Resource> m_pTexture = nullptr;
		HRESULT hr =
			DirectX::CreateWICTextureFromFile(
				m_pd3dDevice.Get(),
				texFileName.c_str(),
				nullptr,
				texSRV.GetAddressOf());
		m_pNumber.push_back(texSRV);		
	}


	T_Math::FVector2 a = { 10.0f, 0.0f };
	T_Math::FVector2 b = { 0.0f, 10.0f };
	float fDot = a | b;
	float fAngle = a.Angle(b);

	RECT rtBk = { 0, 0, 10000.0f, g_yClientSize };
	objScreen.Create(m_pd3dDevice.Get(), m_pContext, rtBk,	L"../../data/bk.bmp");

	m_UIList.resize(3);

	m_UIList[0].Create(m_pd3dDevice.Get(), m_pContext, { 0, 0, 100, 100 },	L"../../data/kgca1.png");
	m_UIList[1].Create(m_pd3dDevice.Get(), m_pContext, { 700, 0, 800, 100 },L"../../data/kgca2.png");
	m_UIList[2].Create(m_pd3dDevice.Get(), m_pContext, { 700, 500, 800, 600 }, L"../../data/kgca3.png");
	hero.Create(m_pd3dDevice.Get(), m_pContext, { 50, 250, 150, 350 }, 	L"../../data/obj.jpg");
	hero.m_fSpeed = 500.0f;

	for (int iNpc = 0; iNpc < 10; iNpc++)
	{
		TNpc npc;
		T_Math::FVector2 pos;
		pos.X = randstep(0.0f, g_xClientSize);
		pos.Y = randstep(0.0f, g_yClientSize);
		npc.Create(m_pd3dDevice.Get(), m_pContext,
			{ (LONG)pos.X, (LONG)pos.Y,(LONG)(pos.X + 50.0f), (LONG)(pos.Y + 50.0f) },
			L"../../data/bump_512.bmp");
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
	objScreen.SetViewTransform(m_Cam.GetMatrix());
	objScreen.Render(m_pContext);

	// 화면 고정( 뷰 변환 생략 )
	//m_UIList[0].SetViewTransform(m_Cam.GetMatrix());
	m_UIList[0].PreRender(m_pContext);
	m_pContext->PSSetShaderResources(0, 1, m_pNumber[m_iNpcCounter - 1].GetAddressOf());
	m_UIList[0].PostRender(m_pContext);

	for (int iNpc = 1; iNpc < m_UIList.size(); iNpc++)
	{
		// 화면 고정( 뷰 변환 생략 )
		//m_UIList[0].SetViewTransform(m_Cam.GetMatrix());
		m_UIList[iNpc].Render(m_pContext);
	}

	hero.SetViewTransform(m_Cam.GetMatrix());
	hero.Render(m_pContext);

	bool bGameEnding = true;

	for_each(begin(m_npcList), end(m_npcList), [&](auto& obj)
		{
			if (!obj.m_bDead)
			{
				obj.SetViewTransform(m_Cam.GetMatrix());
				obj.Render(m_pContext);
				bGameEnding = false;
			}
		});
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