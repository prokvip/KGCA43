#pragma once
#include "tstd.h"
#include <fbxsdk.h>
#include "TFbxObject.h"
#pragma comment(lib, "libfbxsdk-md.lib")
#pragma comment(lib, "libxml2-md.lib")
#pragma comment(lib, "zlib-md.lib")



class TFbxLoader
{
public:
	int						m_iNumNodeCount = 0;
	TScene					m_SceneInfo;
	static FbxManager*		m_pManager;
	FbxImporter*			m_pImporter = nullptr;
	FbxScene*				m_pScene = nullptr;
	FbxNode*				m_pRootNode = nullptr;
	
	std::vector<TFbxNode>		m_pTNodeList;
	std::vector<FbxMesh*>		m_pFbxMeshList;
	std::vector<FbxNode*>		m_pFbxMeshNodeList;
	std::vector<FbxNode*>		m_pFbxNodeList;	
	std::vector<T::TMatrix>		m_matBindPose;
public:
	int    GetFbxNodeIndex(FbxNode* fbxNode);
	bool   ParseMeshSkinning(FbxMesh* fbxMesh, TKgcFileFormat& model);
public:
	void   Init();
	bool   Load(C_STR filename, TKgcFileFormat* model);	
	void   PreProcess(FbxNode* node);
	void   LoadMesh(int iMesh, TKgcFileFormat& );

	void   LoadNodeAnimation(TKgcFileFormat*);

	void   LoadAnimation(TKgcFileFormat* );
	void   LoadAnimationNode(TKgcFileHeader	, FbxNode* pNode, TKgcFileFormat* );
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
	bool   GenBuffer(std::vector<PNCT_Vertex>& vList, std::vector<DWORD>& iList, PNCT_Vertex& v1);
	TFbxLoader() = default;
};


