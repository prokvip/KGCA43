#include "TFbxLoader.h"

FbxManager* TFbxLoader::m_pManager = nullptr;

void   TFbxLoader::Init()
{
	if (m_pManager == nullptr)
	{
		m_pManager = FbxManager::Create();
	}
}
void   TFbxLoader::PreProcess(FbxNode* node)
{
	if (node == nullptr) return;
	FbxMesh* mesh = node->GetMesh();
	if (mesh != nullptr)
	{
		m_pFbxMeshList.push_back(mesh);
	}
	int iNumChild = node->GetChildCount();
	for (int iNode = 0; iNode < iNumChild; iNode++)
	{
		FbxNode* child = node->GetChild(iNode);
		PreProcess(child);
	}
}
bool   TFbxLoader::Load(C_STR filename, std::vector<TFbxModel*>& model)
{
	std::vector<TFbxModel*>& m_pModelList = model;
	m_pImporter = FbxImporter::Create(m_pManager, "");
	m_pScene = FbxScene::Create(m_pManager, "");

	bool bRet;
	// ������ �ε��ϰ� 
	bRet = m_pImporter->Initialize(filename.c_str());
	// �ε��� ������ m_pScene�� ü���.
	bRet = m_pImporter->Import(m_pScene);
	m_pRootNode = m_pScene->GetRootNode();
	PreProcess(m_pRootNode);

	for (int iMesh = 0; iMesh < m_pFbxMeshList.size(); iMesh++)
	{
		LoadMesh(iMesh, model);
	}
	
	m_pFbxMeshList.clear();
	if (m_pScene) m_pScene->Destroy();
	if (m_pImporter) m_pImporter->Destroy();
	m_pScene = nullptr;
	m_pImporter = nullptr;
	return true;
}
void   TFbxLoader::LoadMesh(int iMesh, std::vector<TFbxModel*>& model)
{
	FbxMesh* fbxMesh = m_pFbxMeshList[iMesh];
	TFbxModel* pModel = new TFbxModel;
	// �ﰢ�� � 
	// ������ = �簢��(�ﰢ��2��) or �ﰢ��
	int iNumPolyCount = fbxMesh->GetPolygonCount();
	// ������ ��ġ�� ����� �迭�� �����ּҸ� ��ȯ.
	FbxVector4* pVertexPositions = fbxMesh->GetControlPoints();
	for (int iPoly = 0; iPoly < iNumPolyCount; iPoly++)
	{
		int iPolySize = fbxMesh->GetPolygonSize(iPoly);
		int iNumFace = iPolySize - 2;// 3-2=1, 4-2=2
		for (int iFace = 0; iFace < iNumFace; iFace++)
		{
			int iIndex[3];
			iIndex[0] = fbxMesh->GetPolygonVertex(iPoly,0);
			iIndex[1] = fbxMesh->GetPolygonVertex(iPoly, iFace+2);
			iIndex[2] = fbxMesh->GetPolygonVertex(iPoly, iFace+1);
			for (int iVertex = 0; iVertex < 3; iVertex++)
			{
				PNCT_Vertex v;
				FbxVector4 fbxV = pVertexPositions[iIndex[iVertex]];				
				v.p.X = fbxV.mData[0];
				v.p.Y = fbxV.mData[2];
				v.p.Z = fbxV.mData[1];
				pModel->m_vVertexList.push_back(v);
			}
		}
	}
	// ���� �
	model.push_back(pModel);
}
void   TFbxLoader::Release()
{
	if (m_pManager) m_pManager->Destroy();
	m_pManager = nullptr;
}