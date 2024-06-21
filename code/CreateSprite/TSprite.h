#pragma once
#include "TDxObject.h"
struct tRECT
{
	float    left;
	float    top;
	float    right;
	float    bottom;
};
// 주요기능
// 1) 텍스처 교체, UI교체 : 정해진 시간동안 
// 2) 렌더링 영역의 교체
class TSprite 
{
public:
	std::vector<TVertex>  m_vList;		 // 프레임 화면 정보
	TTexture*		m_pTex = nullptr;
	int		m_iAnimIndex = 0;
	float	m_fPlayTimer = 0.0f;
	float	m_fAnimationTimer = 0.0f;
	float	m_fChangeTime = 0.0f;
	float   m_fTexCounter = 1.0f;
public:
	std::vector<ComPtr<ID3D11ShaderResourceView>> m_pSRVList;
	std::vector<RECT> m_pUVList;
	std::vector<tRECT> m_tUVList;

	ComPtr<ID3D11ShaderResourceView> GetSRV();
	ComPtr<ID3D11ShaderResourceView> GetSRV(UINT index);
	void   SetAnim(float fAnimationTimer)
	{
		m_iAnimIndex = 0;
		m_fAnimationTimer = fAnimationTimer;
		m_fChangeTime = m_fAnimationTimer / m_fTexCounter;
	}
	void   Update();
public:
	void  Load(std::wstring* iconList, UINT iCount);
	void  Load(std::wstring path, UINT xSize, UINT ySize);
public:
	TSprite();
};

