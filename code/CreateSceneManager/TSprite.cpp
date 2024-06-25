#include "TSprite.h"

ComPtr<ID3D11ShaderResourceView> TSprite::GetSRV()
{
	if (m_pSRVList.size() > m_SpriteData.m_iAnimIndex)
	{
		return m_pSRVList[m_SpriteData.m_iAnimIndex];
	}
	return m_pTex->m_pSRV;
}
ComPtr<ID3D11ShaderResourceView> TSprite::GetSRV(UINT index)
{
	if (m_pSRVList.size() > index)
	{
		return m_pSRVList[index];
	}
	return m_pTex->m_pSRV;
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
	m_SpriteData.m_fTexCounter = iCount;
}
void  TSprite::Load(STRING_VECTOR& iconList)
{
	for (int iTex = 0; iTex < iconList.size(); iTex++)
	{
		TTexture* pTex = I_Tex.Load(iconList[iTex]).get();
		if (pTex)
		{
			m_pSRVList.push_back(pTex->m_pSRV);
		}
	}
	m_SpriteData.m_fTexCounter = m_pSRVList.size();
}
void  TSprite::Load(std::wstring path, RECT_ARRAY& list)
{
	m_pTex = I_Tex.Load(path).get();

	UINT wSize = m_pTex->td.Width;
	UINT hSize = m_pTex->td.Height;
	m_SpriteData.m_fTexCounter = list.size();

	for (int iRow = 0; iRow < list.size(); iRow++)
	{
		RECT rt;
		tRECT tRT;
		rt.left = list[iRow].left;
		rt.top = list[iRow].top;
		rt.right = list[iRow].left+list[iRow].right;
		rt.bottom = list[iRow].top+list[iRow].bottom;
		m_pUVList.push_back(rt);

		tRT.left = (float)rt.left / (float)wSize;
		tRT.top = (float)rt.top / (float)hSize;
		tRT.right = (float)rt.right / (float)wSize;
		tRT.bottom = (float)rt.bottom / (float)hSize;

		m_tUVList.push_back(tRT);		
	}
	m_vList.resize(6);
}
void  TSprite::Load(std::wstring path, UINT xSize, UINT ySize)
{
	m_pTex = I_Tex.Load(path).get();

	UINT wSize = m_pTex->td.Width;
	UINT hSize = m_pTex->td.Height;
	
	m_SpriteData.m_fTexCounter = xSize * ySize;
	
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

void   TSprite::Update(TSpriteData& data, float fReverse )
{
	data.m_fPlayTimer += g_fSecondPerFrame;
	if (data.m_fPlayTimer > data.m_fChangeTime)
	{
		data.m_iAnimIndex = data.m_iAnimIndex + fReverse;
		data.m_fPlayTimer -= data.m_fChangeTime;
		if (data.m_iAnimIndex >= data.m_fTexCounter)
		{
			data.m_iAnimIndex = 0;
		}
		if (data.m_iAnimIndex < 0)
		{
			data.m_iAnimIndex = data.m_fTexCounter-1;
		}
	}
	if (m_tUVList.size() > 0)
	{
		m_vList[0].t.X = m_tUVList[data.m_iAnimIndex].left;
		m_vList[0].t.Y = m_tUVList[data.m_iAnimIndex].top;
		m_vList[1].t.X = m_tUVList[data.m_iAnimIndex].right;
		m_vList[1].t.Y = m_tUVList[data.m_iAnimIndex].top;
		m_vList[2].t.X = m_tUVList[data.m_iAnimIndex].right;
		m_vList[2].t.Y = m_tUVList[data.m_iAnimIndex].bottom;

		m_vList[3].t = m_vList[2].t;

		m_vList[4].t.X = m_tUVList[data.m_iAnimIndex].left;
		m_vList[4].t.Y = m_tUVList[data.m_iAnimIndex].bottom;

		m_vList[5].t = m_vList[0].t;
	}
}

TSprite::TSprite()
{
	
}


bool TSpriteManager::GameDataLoad(const TCHAR* pszLoad)
{
	TCHAR pBuffer[256] = { 0 };
	TCHAR pTemp[256] = { 0 };
	TCHAR pTexturePath[256] = { 0 };
	TCHAR pMaskTexturePath[256] = { 0 };
	TCHAR pShaderPath[256] = { 0 };

	int iNumSprite = 0;
	FILE* fp_src;
	_wfopen_s(&fp_src, pszLoad, _T("rt"));
	if (fp_src == NULL) return false;

	_fgetts(pBuffer, _countof(pBuffer), fp_src);
	_stscanf_s(pBuffer, _T("%s"), pTemp, (unsigned int)_countof(pTemp));
	//m_rtSpriteList.resize(iNumSprite);

	int iIsTexAnimation = 0;
	for (;;)
	{
		int iNumFrame = 0;
		_fgetts(pBuffer, _countof(pBuffer), fp_src);
		_stscanf_s(pBuffer, _T("%s %d%d %s%s%s"),
			pTemp, (unsigned int)_countof(pTemp), &iNumFrame,
			&iIsTexAnimation,
			pTexturePath, (unsigned int)_countof(pTexturePath),
			pMaskTexturePath, (unsigned int)_countof(pMaskTexturePath),
			pShaderPath, (unsigned int)_countof(pShaderPath));

		W_STR name = pTemp;
		if (name == L"#END")
		{
			break;
		}
		m_iSpriteTypeList.push_back(iIsTexAnimation);
		m_SpriteNameList.push_back(pTemp);
		m_TextureNameList.push_back(pTexturePath);
		m_MaskTextureNameList.push_back(pMaskTexturePath);
		m_ShaderNameList.push_back(pShaderPath);

		int iReadFrame = 0;
		if (iIsTexAnimation == 0)
		{
			RECT_ARRAY rtList;
			RECT rt;
			for (int iFrame = 0; iFrame < iNumFrame; iFrame++)
			{
				_fgetts(pBuffer, _countof(pBuffer), fp_src);
				_stscanf_s(pBuffer, _T("%d %d %d %d %d"),
					&iReadFrame,
					&rt.left, &rt.top, &rt.right, &rt.bottom);
				rtList.push_back(rt);
			}
			m_rtSpriteList.push_back(rtList);
		}
		else if(iIsTexAnimation == 1)
		{
			STRING_VECTOR list;
			ZeroMemory(pTemp, sizeof(TCHAR)*256);
			for (int iFrame = 0; iFrame < iNumFrame; iFrame++)
			{
				_fgetts(pBuffer, _countof(pBuffer), fp_src);
				_stscanf_s(pBuffer, _T("%d %s"),
					&iReadFrame,
					pTemp, (unsigned int)_countof(pTemp));
				list.push_back(pTemp);
			}
			m_szSpriteList.push_back(list);
		}
		else if (iIsTexAnimation == 2)
		{
			ZeroMemory(pTemp, sizeof(TCHAR) * 256);
			POINT size;
			_fgetts(pBuffer, _countof(pBuffer), fp_src);
			_stscanf_s(pBuffer, _T("%d %d %d"),	&iReadFrame, &size.x, &size.y);			
			m_ptSpriteList.push_back(size);
		}
	}
	fclose(fp_src);


	return true;
}

bool TSpriteManager::Load(const TCHAR* pszLoad)
{
	if (GameDataLoad(pszLoad))
	{
		UINT uvCounter = 0;
		UINT texCounter = 0;
		UINT iGrid = 0;
		for (int iSp = 0; iSp < m_SpriteNameList.size(); iSp++)
		{
			std::unique_ptr<TSprite> pNewSprite = nullptr;	

			if (m_iSpriteTypeList[iSp] == 0) // uv
			{
				pNewSprite = std::make_unique<TSpriteUV>();
				pNewSprite->Load(m_TextureNameList[iSp],
					m_rtSpriteList[uvCounter++]);
			}
			else if(m_iSpriteTypeList[iSp] == 1)// tex
			{
				pNewSprite = std::make_unique<TSpriteTex>();
				pNewSprite->Load(m_szSpriteList[texCounter]);				
				texCounter++;
			}
			else if (m_iSpriteTypeList[iSp] == 2)// grid
			{
				pNewSprite = std::make_unique<TSpriteTex>();
				pNewSprite->Load(m_TextureNameList[iSp], m_ptSpriteList[iGrid].x, m_ptSpriteList[iGrid].y);
				iGrid++;
			}
			pNewSprite->m_csName = m_SpriteNameList[iSp];
			m_List.insert(std::make_pair(m_SpriteNameList[iSp],
				std::move(pNewSprite)));


			
		}
	}
	return true;
}


TSprite* TSpriteManager::GetPtr(W_STR name)
{
	auto iter = m_List.find(name);
	if (iter != m_List.end())
	{
		TSprite* pData = iter->second.get();
		return pData;
	}
	return nullptr;
}