#pragma once
#include "tstd.h"
#include <fbxsdk.h>
#include "TFbxObject.h"
#pragma comment(lib, "libfbxsdk-md.lib")
#pragma comment(lib, "libxml2-md.lib")
#pragma comment(lib, "zlib-md.lib")


struct TFbxNode
{	
	std::wstring szName;
	std::wstring szParentName;
	BOOL         isMesh = FALSE;
	FbxNode* pFbxNode = nullptr;
	FbxNode* pFbxParentNode = nullptr;
	std::vector<T::TMatrix> pAnimationMatrix;	
};

class TFbxLoader
{
public:
	int						m_iNumNodeCount = 0;
	TScene					m_SceneInfo;
	static FbxManager*		m_pManager;
	FbxImporter*	m_pImporter = nullptr;
	FbxScene*		m_pScene = nullptr;
	FbxNode*		m_pRootNode = nullptr;

	std::vector<FbxMesh*>		m_pFbxMeshList;	
	std::vector<std::shared_ptr<TFbxNode>>		m_pFbxMeshNodeList;
	// 전체 트리 노드 구조
	std::vector<FbxNode*>		m_pFbxNodeList;
	// 전체 트리 노드가 본 좌표계로 변환되는 행렬
	std::vector<T::TMatrix>		m_pFbxNodeBindPoseMatrixList;
public:
	int    GetFbxNodeIndex(FbxNode* fbxNode);
	bool   ParseMeshSkinning(FbxMesh* fbxMesh, TKgcFileFormat& model);
public:
	void   Init();
	bool   Load(C_STR filename, TKgcFileFormat* model);	
	void   PreProcess(FbxNode* node);
	void   LoadMesh(int iMesh, TKgcFileFormat& );
	void   LoadAnimation(TKgcFileFormat* );
	void   LoadAnimationNode(TKgcFileHeader	, TFbxNode*, TKgcFileFormat* );
	FbxVector2  GetUV(	FbxMesh* fbxMesh, 
						FbxLayerElementUV* uv,
						int iVertexPosIndex, 
						int iVertexUVIndex);
	FbxColor   GetColor(FbxMesh* fbxMesh,
						FbxLayerElementVertexColor* uv,
						int iVertexPosIndex,
						int iVertexColorIndex); 
	FbxVector4  GetNormal(FbxMesh* fbxMesh,
						FbxLayerElementNormal* VertexColorSet,
						int iVertexPosIndex,
						int iVertexColorIndex);
	T::TMatrix  ConvertFbxAMatrix(FbxAMatrix& m);
	int   GetSubMaterialPolygonIndex(int iPoly, 
									 FbxLayerElementMaterial* pMaterialaterial);
	void   Release();
	void   GenBuffer(std::vector<PNCT_Vertex>& vList, std::vector<DWORD>& iList, PNCT_Vertex& v1);
	TFbxLoader() = default;
};


