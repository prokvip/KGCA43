#pragma once
#include "TDxObj3D.h"
class TFbxModel : public TDxObject3D
{
public:
	std::vector<std::string> m_szTexFileName;
	std::vector<ComPtr<ID3D11ShaderResourceView>>	m_pSubMeshSRV;
	std::vector<TTexture*>							m_pSubMeshTexture;

	using vList = std::vector<PNCT_Vertex>;
	std::vector<vList>					m_vSubMeshVertexList;
	std::vector<ComPtr<ID3D11Buffer>>   m_pSubMeshVertexBuffer;
public:
	virtual void	 LoadTexture(std::wstring szPath) override;
	virtual bool     CreateVertexBuffer(ID3D11Device* pd3dDevice)override;
	virtual void     Render(ID3D11DeviceContext* pContext)override;
	virtual void     Release() override;
	virtual void     SetVertexData() override;
};
