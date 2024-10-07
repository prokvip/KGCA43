#pragma once
#include <tstd.h>
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
	//std::vector<FbxNode*> pFbxChildNode;
};
class TFbxLoader
{
public:
	int						m_iNumNodeCount = 0;
	static FbxManager*		m_pManager;
	FbxImporter*	m_pImporter = nullptr;
	FbxScene*		m_pScene = nullptr;
	FbxNode*		m_pRootNode = nullptr;

	std::vector<FbxMesh*>		m_pFbxMeshList;	
	std::vector<TFbxNode*>		m_pFbxNodeList;
public:
	void   Init();
	bool   Load(C_STR filename, TKgcFileFormat& model);
	bool   Import(C_STR filename, TKgcFileFormat& model);
	void   PreProcess(FbxNode* node);
	void   LoadMesh(int iMesh, TKgcFileFormat& m_pFbxfileList);
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


