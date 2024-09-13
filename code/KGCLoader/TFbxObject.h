#pragma once
#include "TDxObj3D.h"



class TFbxModel : public TDxObject3D
{
public:
	std::vector<std::wstring> m_szTexFileList;
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

struct TKgcFileHeader
{
	int				iVersion =100;
	int				isSubMesh = 0; // 서브메터리얼 유무
	int				iLength = 0;
	int				iChildNodeCounter=0;
	int				iSubVertexBufferCounter = 0;
	int				iSubIndexBufferCounter = 0;
	T::TMatrix		matWorld;
};
struct TTexFileHeader
{	
	int				iLength = 0;
	int				iType = 0;	
};
class TKgcFileFormat 
{
	TKgcFileHeader			m_Header;
public:
	std::wstring			m_szFileName;
	T::TMatrix				m_matWorld;
	std::vector<std::wstring> m_szTexFileList;
	using vList = std::vector<PNCT_Vertex>;
	using iList = std::vector<DWORD>;
	vList			m_vVertexList;		 // 프레임 화면 정보
	iList			m_vIndexList;
	std::vector<vList>		m_vSubMeshVertexList;
	std::vector<iList>		m_vSubMeshIndexList;
	std::vector<TKgcFileFormat*> m_ChildList;

	static bool Export(TKgcFileFormat* tFile, std::wstring szFileName);
	static bool Import(std::wstring szFileName, std::vector<TFbxModel*>& tFbxModel);
};


