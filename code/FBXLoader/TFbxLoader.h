#pragma once
#include <tstd.h>
#include <fbxsdk.h>
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

	std::vector<FbxMesh*>   m_pFbxMeshList;

public:
	bool   Load(C_STR filename);
	void   PreProcess(FbxNode* node);
	void   Init();
	void   Frame();
	void   Render();
	void   Release();
};

