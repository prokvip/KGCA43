#include "TFbxLoader.h"

FbxManager* TFbxLoader::m_pManager = nullptr;

T::TMatrix TFbxLoader::ConvertFbxAMatrix(FbxAMatrix& m)
{
	T::TMatrix tMat;
	float* pRetArray = reinterpret_cast<float*>(&tMat);
	double* pSrcArray = reinterpret_cast<double*>(&m);
	for (int i = 0; i < 16; i++)
	{
		pRetArray[i] = pSrcArray[i];
	}
	T::TMatrix ret;
	ret._11 = tMat._11; ret._12 = tMat._13; ret._13 = tMat._12; ret._14 = tMat._14;
	ret._21 = tMat._31; ret._22 = tMat._33; ret._23 = tMat._32; ret._24 = tMat._34;
	ret._31 = tMat._21; ret._32 = tMat._23; ret._33 = tMat._22; ret._34 = tMat._24;	
	ret._41 = tMat._41; ret._42 = tMat._43; ret._43 = tMat._42; ret._44 = tMat._44;
	return ret;
};
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

	FbxAxisSystem::MayaZUp.ConvertScene(m_pScene);

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
// � ���(GetMappingMode)���� ���εǾ���.
	/*  eNone,				// ������ �������� �ʾҴ�.
		eByControlPoint,	// ������(����)�� 1���� ���� ������ �ִ�.
		eByPolygonVertex,	// �� ���� �� 1���� ���������� �ִ�.
		eByPolygon,			// 1���� �鿡 1���� ���������� �ִ�.
		eByEdge,			// ���� �� 1���� ���������� �ִ�.
		eAllSame*/			// ��ü �鿡 1���� ���������� �ִ�.
		// GetReferenceMode()
		//	eDirect,			// ��������Ʈ��  n��° ��ġ�� ���������� �ִ�.
		//	eIndex,				// 6.0 ���ķ� eIndexToDirect ����
		//	eIndexToDirect		// ��������Ʈ[n��° ��ġ]��  ���������� �ִ�.
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
				case FbxLayerElementVertexColor::eDirect:
				{
					ret = uvSet->GetDirectArray().GetAt(iVertexPosIndex);
				}break;
				case FbxLayerElementVertexColor::eIndexToDirect:
				{
					int iColorIndex = uvSet->GetIndexArray().GetAt(iVertexPosIndex);
					ret = uvSet->GetDirectArray().GetAt(iColorIndex);
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
FbxColor  TFbxLoader::GetColor(	FbxMesh* fbxMesh,
									FbxLayerElementVertexColor* VertexColorSet,
									int iVertexPosIndex,
									int iVertexColorIndex)
{
	FbxColor ret;
	switch (VertexColorSet->GetMappingMode())
	{
	case FbxLayerElementVertexColor::eByControlPoint:
	{
		// � ���� ����� �ľ�
		switch (VertexColorSet->GetReferenceMode())
		{
		case FbxLayerElementVertexColor::eDirect:
		{
			ret = VertexColorSet->GetDirectArray().GetAt(iVertexPosIndex);
		}break;
		case FbxLayerElementVertexColor::eIndexToDirect:
		{
			int iColorIndex = VertexColorSet->GetIndexArray().GetAt(iVertexPosIndex);
			ret = VertexColorSet->GetDirectArray().GetAt(iColorIndex);
		}break;
		}
	}break;
	case FbxLayerElementVertexColor::eByPolygonVertex:
	{
		switch (VertexColorSet->GetReferenceMode())
		{
		case FbxLayerElementVertexColor::eDirect: 
		{
			ret = VertexColorSet->GetDirectArray().GetAt(iVertexColorIndex);
		}break;
		case FbxLayerElementVertexColor::eIndexToDirect:
		{
			int iColorIndex = VertexColorSet->GetIndexArray().GetAt(iVertexColorIndex);
			ret = VertexColorSet->GetDirectArray().GetAt(iColorIndex);
		}break;
		}
	}break;
	}
	return ret;
}
FbxVector4  TFbxLoader::GetNormal(FbxMesh* fbxMesh,
	FbxLayerElementNormal* VertexNormalSet,
	int iVertexPosIndex,
	int iVertexNormalIndex)
{
	FbxVector4 ret;
	switch (VertexNormalSet->GetMappingMode())
	{
		// ������ �� 1���� ������ �ִ�.
	case FbxLayerElementVertexColor::eByControlPoint:
	{
		// � ���� ����� �ľ�
		switch (VertexNormalSet->GetReferenceMode())
		{
		case FbxLayerElementVertexColor::eDirect:
		{
			ret = VertexNormalSet->GetDirectArray().GetAt(iVertexPosIndex);
		}break;
		case FbxLayerElementVertexColor::eIndexToDirect:
		{
			int iIndex = VertexNormalSet->GetIndexArray().GetAt(iVertexPosIndex);
			ret = VertexNormalSet->GetDirectArray().GetAt(iIndex);
		}break;
		}
	}break;
		// ���� �� 1���� ������ �ִ�.
	case FbxLayerElementVertexColor::eByPolygonVertex:
	{
		switch (VertexNormalSet->GetReferenceMode())
		{
		case FbxLayerElementVertexColor::eDirect:
		{
			ret = VertexNormalSet->GetDirectArray().GetAt(iVertexNormalIndex);
		}break;
		case FbxLayerElementVertexColor::eIndexToDirect:
		{
			int iColorIndex = VertexNormalSet->GetIndexArray().GetAt(iVertexNormalIndex);
			ret = VertexNormalSet->GetDirectArray().GetAt(iColorIndex);
		}break;
		}
	}break;
	}
	return ret;
}
// ������ �� ����ϴ� ������͸��� �ε��� ��ȯ.
int   TFbxLoader::GetSubMaterialPolygonIndex(int iPoly, 
	FbxLayerElementMaterial* pMaterial)
{
	int ret;
	switch (pMaterial->GetMappingMode())
	{
		case FbxLayerElement::eByPolygon:
		{
			// � ���� ����� �ľ�
			switch (pMaterial->GetReferenceMode())
			{
			case FbxLayerElement::eIndex:
			{
				ret = iPoly;
			}break;
			case FbxLayerElement::eIndexToDirect:
			{
				ret = pMaterial->GetIndexArray().GetAt(iPoly);
			}break;
			}
		}break;
		default:
		{
			break;
		}break;
	}
	return ret;
}
void   TFbxLoader::LoadMesh(int iMesh, std::vector<TFbxModel*>& model)
{	
	FbxMesh* fbxMesh = m_pFbxMeshList[iMesh];
	FbxNode* pFbxNode = fbxMesh->GetNode();
	// ��Į ��ȯ ���
	FbxAMatrix geom;
	FbxVector4 trans	= pFbxNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxVector4 rot		= pFbxNode->GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 scale	= pFbxNode->GetGeometricScaling(FbxNode::eSourcePivot);
	geom.SetT(trans);
	geom.SetT(rot);
	geom.SetT(scale);
	// �븻 ��ȯ ���
	FbxAMatrix normalMatrix = geom;
	normalMatrix = normalMatrix.Inverse();
	normalMatrix = normalMatrix.Transpose();

	//// ���� ��ȯ ���
	FbxAMatrix matWorld  = pFbxNode->EvaluateGlobalTransform(0);

	TFbxModel* pModel = new TFbxModel;
	pModel->m_matWorld = ConvertFbxAMatrix(matWorld);

	// Layer :  ���̾� ȸ����ŭ �������Ѵ�.
	std::vector<FbxLayerElementUV*>				VertexUVLayer;
	std::vector<FbxLayerElementVertexColor*>	VertexColorLayer;
	std::vector<FbxLayerElementNormal*>			VertexNormalLayer;
	std::vector<FbxLayerElementTangent*>		VertexTangentLayer;
	std::vector<FbxLayerElementMaterial*>		VertexMaterialLayer;
	int iLayerCount = fbxMesh->GetLayerCount();
	for (int iLayer = 0; iLayer < iLayerCount; iLayer++)
	{
		FbxLayer* pFbxLayer = fbxMesh->GetLayer(iLayer);
		if (pFbxLayer->GetUVs())
		{
			VertexUVLayer.emplace_back(pFbxLayer->GetUVs());
		}
		if (pFbxLayer->GetVertexColors())
		{
			VertexColorLayer.emplace_back(pFbxLayer->GetVertexColors());
		}
		if (pFbxLayer->GetTangents())
		{
			VertexTangentLayer.emplace_back(pFbxLayer->GetTangents());
		}
		if (pFbxLayer->GetNormals())
		{
			VertexNormalLayer.emplace_back(pFbxLayer->GetNormals());
		}
		if (pFbxLayer->GetMaterials())
		{
			VertexMaterialLayer.emplace_back(pFbxLayer->GetMaterials());
		}
	}

	// �����븻 (��)���
	if (VertexNormalLayer.size() > 0)
	{
		fbxMesh->InitNormals();
#if (FBXSDK_VERSION_MAJOR >= 2015)
		fbxMesh->GenerateNormals();
#else
		pFbxMesh->ComputeVertexNormals();
#endif
	}

	// material
	int iNumMtrl = pFbxNode->GetMaterialCount();
	if (iNumMtrl > 1)
	{
		pModel->m_vSubMeshVertexList.resize(iNumMtrl);
	}
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
				if (tex)
				{
					texPathName = tex->GetFileName();
				}
			}
			pModel->m_szTexFileName.emplace_back(texPathName);
		}
	}
	// �ﰢ�� � 
	// ������ = �簢��(�ﰢ��2��) or �ﰢ��
	int iNumPolyCount = fbxMesh->GetPolygonCount();
	// ������ ��ġ�� ����� �迭�� �����ּҸ� ��ȯ.
	FbxVector4* pVertexPositions = fbxMesh->GetControlPoints();

	int iBasePolyIndex = 0;
	for (int iPoly = 0; iPoly < iNumPolyCount; iPoly++)
	{
		// ������͸��� �ε���
		int iSubMtrl = 0;
		if (iNumMtrl > 1 && VertexMaterialLayer.size() > 0)
		{
			iSubMtrl = GetSubMaterialPolygonIndex(iPoly, VertexMaterialLayer[0]);
		}
		int iPolySize = fbxMesh->GetPolygonSize(iPoly);
		int iNumFace = iPolySize - 2;// 3-2=1, 4-2=2
		for (int iFace = 0; iFace < iNumFace; iFace++)
		{
			int iVertexIndex[3] = { 0, iFace + 2, iFace + 1 };
			int iVertexPositionIndex[3];
			iVertexPositionIndex[0] = fbxMesh->GetPolygonVertex(iPoly, iVertexIndex[0]);
			iVertexPositionIndex[1] = fbxMesh->GetPolygonVertex(iPoly, iVertexIndex[1]);
			iVertexPositionIndex[2] = fbxMesh->GetPolygonVertex(iPoly, iVertexIndex[2]);

			int iVertexUVIndex[3];
			iVertexUVIndex[0] = fbxMesh->GetTextureUVIndex(iPoly, iVertexIndex[0]);
			iVertexUVIndex[1] = fbxMesh->GetTextureUVIndex(iPoly, iVertexIndex[1]);
			iVertexUVIndex[2] = fbxMesh->GetTextureUVIndex(iPoly, iVertexIndex[2]);

			for (int iVertex = 0; iVertex < 3; iVertex++)
			{
				// ���� ��ġ
				PNCT_Vertex v;
				FbxVector4 fbxV = pVertexPositions[iVertexPositionIndex[iVertex]];
				// neFbxV = fbxV * (geom * matWorld)
				fbxV = geom.MultT(fbxV);
				//fbxV = matWorld.MultT(fbxV);
				v.p.X = fbxV.mData[0];
				v.p.Y = fbxV.mData[2];
				v.p.Z = fbxV.mData[1];
				// ���� �ؽ�ó ��ǥ			
				if (VertexUVLayer.size() > 0)
				{
					FbxVector2 uv = GetUV(fbxMesh, VertexUVLayer[0],
						iVertexPositionIndex[iVertex], iVertexUVIndex[iVertex]);
					v.t.X = uv.mData[0];
					v.t.Y = 1.0f - uv.mData[1];
				}
				// ���� �÷�
				FbxColor color = FbxColor(1, 1, 1, 1);
				if (VertexColorLayer.size())
				{
					color = GetColor(fbxMesh, VertexColorLayer[0],
						iVertexPositionIndex[iVertex], 
						iBasePolyIndex + iVertexIndex[iVertex]);
				}
				v.c.X = color.mRed;
				v.c.Y = color.mGreen;
				v.c.Z = color.mBlue;
				v.c.W = color.mAlpha;
				// �����븻

				FbxVector4 vFbxNormal = { 0,0,0 };
				if (VertexNormalLayer.size())
				{
					vFbxNormal = GetNormal(fbxMesh, VertexNormalLayer[0],
						iVertexPositionIndex[iVertex],
						iBasePolyIndex + iVertexIndex[iVertex]);
					vFbxNormal = normalMatrix.MultT(vFbxNormal);
					vFbxNormal.Normalize();
				}
				v.n.X = vFbxNormal.mData[0];
				v.n.Y = vFbxNormal.mData[2];
				v.n.Z = vFbxNormal.mData[1];

				if (pModel->m_vSubMeshVertexList.size() == 0)
				{
					pModel->m_vVertexList.push_back(v);
				}
				else
				{
					pModel->m_vSubMeshVertexList[iSubMtrl].push_back(v);
				}
			}
		}

		iBasePolyIndex += iPolySize;
	}
	// ���� �
	model.push_back(pModel);
}
void   TFbxLoader::Release()
{
	if (m_pManager) m_pManager->Destroy();
	m_pManager = nullptr;
}