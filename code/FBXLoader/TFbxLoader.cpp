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
FbxVector2  TFbxLoader::GetUV(
	FbxMesh* fbxMesh, FbxLayerElementUV* uvSet, int iVertexPosIndex, int iVertexUVIndex)
{
	FbxVector2 ret;
	// � ������� ����
	switch (uvSet->GetMappingMode())
	{
		case FbxLayerElementUV::eByControlPoint:
		{
			// � ���� ����� �ľ�
			switch (uvSet->GetReferenceMode())
			{
				case FbxLayerElementUV::eDirect: 
				{
				}break;
				case FbxLayerElementUV::eIndexToDirect:
				{
					ret.mData[0] = uvSet->GetDirectArray().GetAt(iVertexPosIndex).mData[0];
					ret.mData[1] = uvSet->GetDirectArray().GetAt(iVertexPosIndex).mData[1];
				}break;
			}
		}break;
		case FbxLayerElementUV::eByPolygonVertex:
		{			
			switch (uvSet->GetReferenceMode())
			{
				case FbxLayerElementUV::eDirect: // fbx 5����
				case FbxLayerElementUV::eIndexToDirect:
				{
					ret.mData[0] = uvSet->GetDirectArray().GetAt(iVertexUVIndex).mData[0];
					ret.mData[1] = uvSet->GetDirectArray().GetAt(iVertexUVIndex).mData[1];
				}break;
			}
		}break;
	}
	return ret;
}
void   TFbxLoader::LoadMesh(int iMesh, std::vector<TFbxModel*>& model)
{	
	FbxMesh* fbxMesh = m_pFbxMeshList[iMesh];
	FbxNode* pFbxNode = fbxMesh->GetNode();

	TFbxModel* pModel = new TFbxModel;
	// Layer :  ���̾� ȸ����ŭ �������Ѵ�.
	//int iLayerCount = fbxMesh->GetLayerCount();
	FbxLayerElementUV* VertexUVLayer=nullptr;
	FbxLayer* pFbxLayer = fbxMesh->GetLayer(0);
	if (pFbxLayer)
	{
		VertexUVLayer = pFbxLayer->GetUVs();
	}
	// material
	int iNumMtrl = pFbxNode->GetMaterialCount();
	for (int iMtrl = 0; iMtrl < iNumMtrl; iMtrl++)
	{
		FbxSurfaceMaterial* pSurfaceMtrl = pFbxNode->GetMaterial(iMtrl);
		if (pSurfaceMtrl)
		{
			std::string texPathName;
			auto Property = pSurfaceMtrl->FindProperty(FbxSurfaceMaterial::sDiffuse);
			if (Property.IsValid())
			{
				FbxFileTexture* tex = Property.GetSrcObject<FbxFileTexture>(0);
				texPathName = tex->GetFileName();
			}
			pModel->m_szTexFileName = texPathName;
		}
	}
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
			int iVertexPositionIndex[3];
			iVertexPositionIndex[0] = fbxMesh->GetPolygonVertex(iPoly,0);
			iVertexPositionIndex[1] = fbxMesh->GetPolygonVertex(iPoly, iFace+2);
			iVertexPositionIndex[2] = fbxMesh->GetPolygonVertex(iPoly, iFace+1);

			int iVertexUVIndex[3];
			iVertexUVIndex[0] = fbxMesh->GetTextureUVIndex(iPoly, 0);
			iVertexUVIndex[1] = fbxMesh->GetTextureUVIndex(iPoly, iFace + 2);
			iVertexUVIndex[2] = fbxMesh->GetTextureUVIndex(iPoly, iFace + 1);

			for (int iVertex = 0; iVertex < 3; iVertex++)
			{
				// ���� ��ġ
				PNCT_Vertex v;
				FbxVector4 fbxV = pVertexPositions[iVertexPositionIndex[iVertex]];
				v.p.X = fbxV.mData[0];
				v.p.Y = fbxV.mData[2];
				v.p.Z = fbxV.mData[1];
				// ���� �ؽ�ó ��ǥ				
				FbxVector2 uv = GetUV(fbxMesh, VertexUVLayer,
					iVertexPositionIndex[iVertex], iVertexUVIndex[iVertex]);
				v.t.X = uv.mData[0];
				v.t.Y = 1.0f-uv.mData[1];
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