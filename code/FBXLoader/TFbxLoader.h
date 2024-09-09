#pragma once
#include <tstd.h>
#include <fbxsdk.h>
#include "TFbxObject.h"
#pragma comment(lib, "libfbxsdk-md.lib")
#pragma comment(lib, "libxml2-md.lib")
#pragma comment(lib, "zlib-md.lib")

class TFbxLoader
{
public:
	static FbxManager*		m_pManager;
	FbxImporter*	m_pImporter = nullptr;
	FbxScene*		m_pScene = nullptr;
	FbxNode*		m_pRootNode = nullptr;

	std::vector<FbxMesh*>		m_pFbxMeshList;	
public:
	void   Init();
	bool   Load(C_STR filename, std::vector<TFbxModel*>& model);
	void   PreProcess(FbxNode* node);
	void   LoadMesh(int iMesh, std::vector<TFbxModel*>& m_pModelList);
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
	TFbxLoader() = default;
};


