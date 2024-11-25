#pragma once
#include "TKgcFormat.h"
struct TInstance
{
	T::TMatrix matInstance;
};
class TFbxModel : public TDxObject3D
{
public:
	float			m_fFrameAnimation = 0.0f;
	float			m_fStartFrame = 0;
	float			m_fEndFrame = 0;
	void			SetAnimFrame(float s, float e);

	TKgcFileHeader	m_FileHeader;
	std::vector< std::shared_ptr<TFbxModel> > m_ChildModel;

	std::vector<std::wstring> m_szTexFileList;
	std::vector<ComPtr<ID3D11ShaderResourceView>>	m_pSubMeshSRV;
	std::vector<TTexture*>							m_pSubMeshTexture;

	using vList = std::vector<PNCT_Vertex>;
	using iList = std::vector<DWORD>;
	using vIWList = std::vector<IW_Vertex>;

	std::vector< TWeight>       m_WeightList;
	std::vector<IW_Vertex>		m_vIWVertexList;
	ComPtr<ID3D11Buffer>		m_pIWVertexBuffer = nullptr;

	vIWList						m_vVertexListIndexWegiht;
	std::vector<vIWList>		m_vSubMeshIWVertexList;
	std::vector<vList>			m_vSubMeshVertexList;
	std::vector<iList>			m_vSubMeshIndexList;
	std::vector<ComPtr<ID3D11Buffer>>   m_pSubMeshVertexBuffer;
	std::vector<ComPtr<ID3D11Buffer>>   m_pSubMeshIndexBuffer;
	std::vector<ComPtr<ID3D11Buffer>>   m_pSubMeshIWVertexBuffer;
	std::vector<T::TMatrix>				m_pAnimationMatrix;

	//skinning
	std::vector<T::TMatrix>		m_matBindPose;
	ComPtr<ID3D11Buffer>        m_pBoneCB;
	TBoneMatrix					m_matBoneList;
	using boneFrameMatrix = std::vector<T::TMatrix>;
	std::vector<boneFrameMatrix> m_pBoneAnimMatrix;
	std::vector<TFbxNode>		m_pTNodeList;

	// used texbuffer
	ComPtr<ID3D11Buffer>			  m_pBoneBuffer;
	ComPtr<ID3D11ShaderResourceView>  m_pBoneBufferRV;
	void   SetBoneMatrices(ID3D11DeviceContext* pContext, float& fAnimFrame, TFbxModel* pAnim=nullptr);

	std::vector<std::vector<TTrack>>    m_matAnimTrack;
	bool     RebuildAnimation(TFbxModel* pModel);

	// Instance buffer
	std::vector<TInstance>			  m_InstanceData;
	ComPtr<ID3D11Buffer>			  m_pInstanceBuffer;
public:
	virtual bool     CreateIWVertexBuffer(ID3D11Device* pd3dDevice) override;
	T::TMatrix		 GetBoneMatrix(TFbxModel* pAnim, std::wstring name, int iFrame);
public:
	virtual void	 LoadTexture(std::wstring szPath) override;
	virtual bool     CreateVertexBuffer(ID3D11Device* pd3dDevice)override;
	virtual bool     CreateIndexBuffer(ID3D11Device* pd3dDevice) override;
	virtual bool	 CreateConstantBuffer(ID3D11Device* pd3dDevice) override;
	virtual bool	 CreateBoneSRV(ID3D11Device* pd3dDevice);
	virtual void     Frame()override;
	virtual void     AnimFrame(float& fAnimFrame);
	virtual void     AnimFrame(float& fAnimFrame, TFbxModel* pAnim);
	virtual void     Render(ID3D11DeviceContext* pContext)override;
	virtual void     Release() override;
	virtual void     SetVertexData() override;
	virtual bool     CreateInputLayout(ID3D11Device* pd3dDevice) override;
};
class TKgcObject : public TDxObject3D
{
	std::shared_ptr<TFbxModel>	  m_pdxObj = nullptr;
public:
	std::shared_ptr<TFbxModel> Get() { return m_pdxObj; }
	TFbxModel* GetPtr() { return m_pdxObj.get(); }
	bool  Load(TLoadData ld);
	void  Release() override;
};