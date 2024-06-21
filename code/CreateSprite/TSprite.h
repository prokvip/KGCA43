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
class TSprite : public TDxObject
{
	TTexture* pTex;
public:
	std::vector<ComPtr<ID3D11ShaderResourceView>> m_pSRVList;
	std::vector<RECT> m_pUVList;
	std::vector<tRECT> m_tUVList;

	void   Update();
public:
	void  LoadA(std::wstring filename);
	void  LoadB(std::wstring filename);
	void  LoadC(std::wstring filename);
	void  LoadD(std::wstring filename);

	TSprite();
};

