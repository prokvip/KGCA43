#include "TSprite.h"


void  TSprite::LoadA(std::wstring path)
{	
	for (int iTex = 0; iTex < 10; iTex++)
	{
		std::wstring texFileName = path + L"¼ýÀÚ";
		texFileName += std::to_wstring(iTex) + L".png";
		TTexture* pTex = I_Tex.Load(texFileName).get();
		m_pSRVList.push_back(pTex->m_pSRV);
	}
}
void  TSprite::LoadB(std::wstring path)
{
	std::wstring iconList[] =
	{
		L"../../data/Icons/S_KBSJoint.PNG",
		L"../../data/Icons/S_KHinge.PNG",
		L"../../data/Icons/S_KPrismatic.PNG",
		L"../../data/Icons/S_LevelSequence.PNG",
		L"../../data/Icons/S_NavP.PNG",

		L"../../data/Icons/S_Note.PNG",
		L"../../data/Icons/S_Pawn.PNG",
		L"../../data/Icons/S_Player.PNG",
		L"../../data/Icons/S_PortalActorIcon2.PNG",
		L"../../data/Icons/S_RadForce.PNG",

		L"../../data/Icons/S_SkyAtmosphere.PNG",
		L"../../data/Icons/S_SceneCaptureIcon.PNG",
		L"../../data/Icons/S_LevelSequence.PNG",
		L"../../data/Icons/S_SkyLight.PNG",
		L"../../data/Icons/S_Solver.PNG",

		L"../../data/Icons/S_TargetPoint.PNG",
		L"../../data/Icons/S_Terrain.PNG",
		L"../../data/Icons/S_Thruster.PNG",
		L"../../data/Icons/S_VectorFieldVol.PNG",
	};


	UINT iNumberIcons = _countof(iconList);
	std::wstring texPath;
	texPath = L"../../data/Icons/";
	for (int iTex = 0; iTex < iNumberIcons; iTex++)
	{
		TTexture* pTex = I_Tex.Load(iconList[iTex]).get();
		m_pSRVList.push_back(pTex->m_pSRV);
	}
}
void  TSprite::LoadC(std::wstring path)
{
	std::wstring iconList[] =
	{
		L"../../data/Effect/bla00.dds",
		L"../../data/Effect/bla01.dds",
		L"../../data/Effect/bla02.dds",
		L"../../data/Effect/bla03.dds",
		L"../../data/Effect/bla04.dds",
		L"../../data/Effect/bla05.dds",
		L"../../data/Effect/bla06.dds",
		L"../../data/Effect/bla07.dds",
		L"../../data/Effect/bla08.dds",
		L"../../data/Effect/bla09.dds",
		L"../../data/Effect/bla10.dds",
		L"../../data/Effect/bla11.dds",
		L"../../data/Effect/bla12.dds",
		L"../../data/Effect/bla13.dds",
		L"../../data/Effect/bla14.dds",
		L"../../data/Effect/bla15.dds",
		L"../../data/Effect/bla16.dds",
		L"../../data/Effect/bla17.dds",
		L"../../data/Effect/bla18.dds",

		L"../../data/Effect/bla19.dds",
		L"../../data/Effect/bla20.dds",
		L"../../data/Effect/bla21.dds",
		L"../../data/Effect/bla22.dds",
		L"../../data/Effect/bla23.dds",
	};
	UINT iNumberIcons = _countof(iconList);
	for (int iTex = 0; iTex < iNumberIcons; iTex++)
	{
		TTexture* pTex = I_Tex.Load(iconList[iTex]).get();
		m_pSRVList.push_back(pTex->m_pSRV);
	}
}
void  TSprite::LoadD(std::wstring path)
{
	pTex = I_Tex.Load(path).get();
	m_pSRVList.push_back(pTex->m_pSRV);

	UINT wSize = pTex->td.Width;
	UINT hSize = pTex->td.Height;

	UINT Count = 4;
	for (int iRow = 0; iRow < Count; iRow++)
	{
		for (int iCol = 0; iCol < Count; iCol++)
		{
			RECT rt;
			tRECT tRT;
			rt.left = iCol * pTex->td.Width/ Count;
			rt.top = iRow * pTex->td.Width / Count;
			rt.right = rt.left  + pTex->td.Width / Count;
			rt.bottom = rt.top + pTex->td.Width / Count;
			m_pUVList.push_back(rt);

			tRT.left = (float)rt.left / (float)wSize;
			tRT.top = (float)rt.top / (float)hSize;
			tRT.right = (float)rt.right / (float)wSize;
			tRT.bottom = (float)rt.bottom / (float)hSize;

			m_tUVList.push_back(tRT);
		}
	}
}

void   TSprite::Update()
{
	UINT wSize = pTex->td.Width;
	UINT hSize = pTex->td.Height;
	static int m_iAnimIndex = 0;
	static float fAnimationTimer = 0.0f;
	fAnimationTimer += g_fSecondPerFrame;
	float ftime = 1.0f / (4.0f * 4.0f);
	if (fAnimationTimer > ftime)
	{
		m_iAnimIndex++;
		fAnimationTimer -= ftime;
		if (m_iAnimIndex >= (4.0f * 4.0f-1))
		{
			m_iAnimIndex = 0;
		}
	}

	m_vList[0].t.X = m_tUVList[m_iAnimIndex].left;
	m_vList[0].t.Y = m_tUVList[m_iAnimIndex].top;
	m_vList[1].t.X = m_tUVList[m_iAnimIndex].right;
	m_vList[1].t.Y = m_tUVList[m_iAnimIndex].top;
	m_vList[2].t.X = m_tUVList[m_iAnimIndex].right;
	m_vList[2].t.Y = m_tUVList[m_iAnimIndex].bottom;

	m_vList[3].t = m_vList[2].t;

	m_vList[4].t.X = m_tUVList[m_iAnimIndex].left;
	m_vList[4].t.Y = m_tUVList[m_iAnimIndex].bottom;

	m_vList[5].t = m_vList[0].t;



	/*if (m_pVertexBuffer != nullptr)
	{
		m_pContext->UpdateSubresource(m_pVertexBuffer, 0, NULL, &m_vListNDC.at(0), 0, 0);
	}*/
}

TSprite::TSprite()
{
	m_vList.resize(6);
}