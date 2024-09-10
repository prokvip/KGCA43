#pragma once
#include "TDxObj3D.h"
class TFbxModel : public TDxObject3D
{
public:
	std::vector<std::string> m_szTexFileName;
	std::vector<ComPtr<ID3D11ShaderResourceView>>	m_pSubMeshSRV;
	std::vector<TTexture*>							m_pSubMeshTexture;

	using vList = std::vector<PNCT_Vertex>;
	using iList = std::vector<DWORD>;
	std::vector<vList>		m_vSubMeshVertexList;
	std::vector<iList>		m_vSubMeshIndexList;
	std::vector<ComPtr<ID3D11Buffer>>   m_pSubMeshVertexBuffer;
	std::vector<ComPtr<ID3D11Buffer>>   m_pSubMeshIndexBuffer;
public:
	virtual void	 LoadTexture(std::wstring szPath) override;
	virtual bool     CreateVertexBuffer(ID3D11Device* pd3dDevice)override;
	virtual bool     CreateIndexBuffer(ID3D11Device* pd3dDevice) override;
	virtual void     Render(ID3D11DeviceContext* pContext)override;
	virtual void     Release() override;
	virtual void     SetVertexData() override;
};

struct TKGCAFileFormat 
{
	std::wstring			m_szFileName;
	T::TMatrix				m_matWorld;
	std::vector<std::string> m_szTexFileName;
	using vList = std::vector<PNCT_Vertex>;
	using iList = std::vector<DWORD>;
	vList			m_vVertexList;		 // ������ ȭ�� ����
	iList			m_vIndexList;
	std::vector<vList>		m_vSubMeshVertexList;
	std::vector<iList>		m_vSubMeshIndexList;
	std::vector<TKGCAFileFormat> m_ChildList;
};
