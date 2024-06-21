#pragma once
#include "TDxObject.h"
struct tRECT
{
	float    left;
	float    top;
	float    right;
	float    bottom;
};
// �ֿ���
// 1) �ؽ�ó ��ü, UI��ü : ������ �ð����� 
// 2) ������ ������ ��ü
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

