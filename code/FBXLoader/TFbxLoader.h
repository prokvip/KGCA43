#pragma once
#include <tstd.h>
#include <fbxsdk.h>
#include "TDxObj3D.h"
#pragma comment(lib, "libfbxsdk-md.lib")
#pragma comment(lib, "libxml2-md.lib")
#pragma comment(lib, "zlib-md.lib")

class TFbxModel : public TDxObject3D
{
public:
	std::string m_szTexFileName;
};
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
	void   Release();
	TFbxLoader() = default;
};


