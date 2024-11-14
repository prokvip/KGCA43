#pragma once
#include "TDxObj3D.h"

struct TScene
{
	int		iStartFrame = 0;
	int		iLastFrame = 0;
	int		iFrameSpeed = 30;
};
// 정점 단위로 생성한다.
struct TWeight
{
	std::vector<int>   Index;
	std::vector<float> weight;
};
struct TKgcFileHeader
{
	int				iVersion = 100;
	int				isSubMesh = 0; // 서브메터리얼 유무
	int				iStartFrame = 0;
	int				iLastFrame = 0;
	int				iFrameSpeed = 30;
	int				iLength = 0;
	int				iNumTrack = 0;
	int				iChildNodeCounter = 0;
	int				iSubVertexBufferCounter = 0;
	int				iSubIndexBufferCounter = 0;
	T::TMatrix		matWorld;
};
struct TKgcFileTrack
{
	std::vector<T::TMatrix> pAnimationMatrix;
};
struct TTexFileHeader
{
	int				iLength = 0;
	int				iType = 0;
};

class TFbxModel : public TDxObject3D
{
public:
	float			m_fFrameAnimation = 0.0f;
	TKgcFileHeader	m_FileHeader;
	std::vector< std::shared_ptr<TFbxModel> > m_ChildModel;

	std::vector<std::wstring> m_szTexFileList;
	std::vector<ComPtr<ID3D11ShaderResourceView>>	m_pSubMeshSRV;
	std::vector<TTexture*>							m_pSubMeshTexture;

	using vList = std::vector<PNCT_Vertex>;
	using iList = std::vector<DWORD>;
	std::vector<vList>		m_vSubMeshVertexList;
	std::vector<iList>		m_vSubMeshIndexList;
	std::vector<ComPtr<ID3D11Buffer>>   m_pSubMeshVertexBuffer;
	std::vector<ComPtr<ID3D11Buffer>>   m_pSubMeshIndexBuffer;
	std::vector<T::TMatrix>				m_pAnimationMatrix;

	//skinning
	std::vector<T::TMatrix>		m_pFbxNodeBindPoseMatrixList;
	std::vector< TWeight>       m_WeightList;
	using vIWList = std::vector<IW_Vertex>;
	vIWList						m_vVertexListIndexWegiht;
	std::vector<vIWList>		m_vSubMeshIWVertexList;
public:
	virtual void	 LoadTexture(std::wstring szPath) override;
	virtual bool     CreateVertexBuffer(ID3D11Device* pd3dDevice)override;
	virtual bool     CreateIndexBuffer(ID3D11Device* pd3dDevice) override;
	virtual void     Render(ID3D11DeviceContext* pContext)override;
	virtual void     Release() override;
	virtual void     SetVertexData() override;
};


class TKgcFileFormat 
{
public:
	TKgcFileHeader			m_Header;
	std::wstring			m_szFileName;
	T::TMatrix				m_matWorld;
	std::vector<std::wstring> m_szTexFileList;
	using vList = std::vector<PNCT_Vertex>;
	using iList = std::vector<DWORD>;
	vList			m_vVertexList;		 // 프레임 화면 정보
	iList			m_vIndexList;
	std::vector<vList>		m_vSubMeshVertexList;
	std::vector<iList>		m_vSubMeshIndexList;
	std::vector<std::shared_ptr<TKgcFileFormat>> m_ChildList;

	std::vector<T::TMatrix> m_pAnimationMatrix;

	// Skinning : 전체 트리 노드가 본 좌표계로 변환되는 행렬
	std::vector<T::TMatrix>		m_pFbxNodeBindPoseMatrixList;
	std::vector<int>			m_pUsedBoneIndexList;

	std::vector< TWeight>       m_WeightList;
	using vIWList = std::vector<IW_Vertex>;
	vIWList						m_vVertexListIndexWegiht;
	std::vector<vIWList>		m_vSubMeshIWVertexList;


	static bool Export(TKgcFileFormat* tFile, std::wstring szFileName);
	static bool Import( std::wstring szFileName, std::wstring szShaderFile,
						 std::shared_ptr<TFbxModel>& tFbxModel);
};


class TKgcObject : public TDxObject3D
{
	std::shared_ptr<TFbxModel>	  m_pdxObj = nullptr;
public:
	std::shared_ptr<TFbxModel> Get() { return m_pdxObj; }
	TFbxModel* GetPtr() { return m_pdxObj.get(); }
	bool  Load(T_STR filename)
	{
		m_pdxObj = std::make_shared<TFbxModel>();
		if (TKgcFileFormat::Import(filename, L"../../data/shader/Lightting.hlsl", m_pdxObj))
		{
			return true;
		}
		return true;
	}
	void  Release() override
	{
		m_pdxObj->Release();
		return;
	}
};