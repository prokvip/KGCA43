#pragma once
#include "TPlaneShape.h"
class TEffect
{
public:
	W_STR                   m_csName;
	RECT					m_rtRect;
	UINT					m_iNumHeight;
	UINT					m_iNumWidth;
	tRECT					m_uvIndex;
	UINT					m_iApplyIndex = 0;
	float					m_fTimer = 0.0f;
	float					m_fAnimationTime = 1.0f;
	float					m_fInitAnimationTime = 1.0f;
	float					m_fStepTime = 0.0f;

	std::vector<tRECT>		m_pUVArray;
	std::vector<TTexture*>  m_pTexArray;
	std::vector<TVertex>	m_VertexList;  // 시스템 메모리
	TTexture*				m_ptTex = nullptr;
public:
	virtual bool    Init();
	virtual bool    Create(W_STR name) {
		return true;
	};
	virtual bool    Create(W_STR name, T_STR_VECTOR texFileName);
	virtual bool    Create(W_STR name, W_STR texFileName, 
							UINT iNumWidth=1, 
							UINT iNumHeight=1,
							float fAnimTime = 1.0f);
	virtual bool	LoadTexture(T_STR_VECTOR texArray);
	virtual bool    LoadTexture(std::wstring texFileName);
	virtual bool    PreRender();
	virtual bool    Frame(float fElapsTime);
	virtual bool    Render(ID3D11DeviceContext* pd3dContext);
	virtual bool    PostRender();
	virtual bool    Release();
};

