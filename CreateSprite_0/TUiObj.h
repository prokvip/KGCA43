#pragma once
#include "TPlaneShape.h"

class TUiObj 
{
public:
	std::vector<TVertex>	m_VertexList;  // 시스템 메모리
	TTexture*	m_ptTex = nullptr;
public:
	virtual bool    Init() {
		return true;
	};
	virtual bool    Load(std::wstring texFileName) {
		m_ptTex = TTextureMgr::Get().Load(texFileName);
		if (m_ptTex != nullptr)
			return true;
		return false;
	};
	virtual bool    PreRender() {
		return true;
	};
	virtual bool    Render() {
		return true;
	};
	virtual bool    PostRender() {
		return true;
	};
	virtual bool    RenderChange() {
		return true;
	};
	virtual bool    Release() {
		return true;
	};
};

class TUiNumber : public TUiObj
{
public:
	std::vector<TTexture*>  m_pTexArray;
	
public:
	bool    Init() {
		return true;
	};
	bool    Load(std::wstring texFileName) {
		return TUiObj::Load(texFileName);
	};
	bool    PreRender() {
		return true;
	};
	bool    Render() {
		return true;
	};
	bool    PostRender() {
		return true;
	};
	virtual bool    Release() 
	{
		TUiObj::Release();		
		return true;
	}
};