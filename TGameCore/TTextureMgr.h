#pragma once
#include "TStd.h"
//#include "../../include/DirectXTex.h"
//프로젝트 속성에서 C++-> 추가 포함 디렉토리에 ../../include 한다.
#include "DirectXTex.h"

class TTexture
{
public:
	std::unique_ptr<DirectX::ScratchImage> m_tex;
	ID3D11ShaderResourceView* m_pTextureSRV = nullptr;
public:
	bool		Load(ID3D11Device* pd3dDevice,
					ID3D11DeviceContext* pd3dContext, 
					std::wstring loadfilename);
	bool		Release();
	TTexture() {}
	virtual ~TTexture() {}
};
class TTextureMgr
{
	ID3D11Device* m_pd3dDevice = nullptr;
	ID3D11DeviceContext* m_pd3dContext = nullptr;	
public:
	static TTextureMgr& Get()
	{
		static TTextureMgr mgr;
		return mgr;
	}
	std::map<std::wstring, std::shared_ptr<TTexture>> m_list;
	TTexture*   Load(std::wstring loadfilename);
	bool   Release();
	void  Set(ID3D11Device* pd3dDevice,
		ID3D11DeviceContext* pd3dContext)
	{
		m_pd3dDevice = pd3dDevice;
		m_pd3dContext = pd3dContext;
	}
private:
	TTextureMgr() {}
public:
	~TTextureMgr();
};

