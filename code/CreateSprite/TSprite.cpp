#include "TSprite.h"

ComPtr<ID3D11ShaderResourceView> TSprite::GetSRV()
{
	return m_pSRVList[m_iAnimIndex];
}
ComPtr<ID3D11ShaderResourceView> TSprite::GetSRV(UINT index)
{
	return m_pSRVList[index];
}
void  TSprite::Load( std::wstring* iconList, UINT iCount )
{	
	for (int iTex = 0; iTex < iCount; iTex++)
	{
		TTexture* pTex = I_Tex.Load(iconList[iTex]).get();
		if (pTex)
		{
			m_pSRVList.push_back(pTex->m_pSRV);
		}
	}
	m_fTexCounter = iCount;
}

void  TSprite::Load(std::wstring path, UINT xSize, UINT ySize)
{
	m_pTex = I_Tex.Load(path).get();
	m_pSRVList.push_back(m_pTex->m_pSRV);

	UINT wSize = m_pTex->td.Width;
	UINT hSize = m_pTex->td.Height;
	m_fTexCounter = xSize * ySize;
	UINT offsetX = wSize / xSize;;
	UINT offsetY = hSize / xSize;;
	for (int iRow = 0; iRow < ySize; iRow++)
	{
		for (int iCol = 0; iCol < xSize; iCol++)
		{
			RECT rt;
			tRECT tRT;
			rt.left		= iCol * offsetX;
			rt.top		= iRow * offsetY;
			rt.right	= rt.left  + offsetX;
			rt.bottom	= rt.top + offsetY;
			m_pUVList.push_back(rt);

			tRT.left = (float)rt.left / (float)wSize;
			tRT.top = (float)rt.top / (float)hSize;
			tRT.right = (float)rt.right / (float)wSize;
			tRT.bottom = (float)rt.bottom / (float)hSize;

			m_tUVList.push_back(tRT);
		}
	}
	m_vList.resize(6);
}

void   TSprite::Update()
{
	m_fPlayTimer += g_fSecondPerFrame;
	if (m_fPlayTimer > m_fChangeTime)
	{
		m_iAnimIndex++;
		m_fPlayTimer -= m_fChangeTime;
		if (m_iAnimIndex >= m_fTexCounter)
		{
			m_iAnimIndex = 0;
		}
	}

	if (m_tUVList.size() > 0)
	{
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
	}
}

TSprite::TSprite()
{
	
}