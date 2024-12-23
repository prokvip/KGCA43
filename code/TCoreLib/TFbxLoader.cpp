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
	m_iNumNodeCount++;
	if (node == nullptr) return;

	TFbxNode tNode;
	wcscpy_s( tNode.szName, _countof(tNode.szName), to_mw(node->GetName()).c_str());
	tNode.isMesh = FALSE;

	FbxNode* pParentNode = node->GetParent();
	wcscpy_s(tNode.szParentName, _countof(tNode.szParentName), L"null");
	if (pParentNode)
	{
		wcscpy_s(tNode.szParentName, _countof(tNode.szParentName),to_mw(pParentNode->GetName()).c_str());
	}

	FbxMesh* mesh = node->GetMesh();
	if (mesh != nullptr)
	{
		m_pFbxMeshList.push_back(mesh);
		tNode.isMesh = TRUE;
		m_pFbxMeshNodeList.emplace_back(node);
	}	
	m_pFbxNodeList.emplace_back(node);
	m_pTNodeList.emplace_back(tNode);
	
	int iNumChild = node->GetChildCount();
	for (int iNode = 0; iNode < iNumChild; iNode++)
	{
		FbxNode* pChild = node->GetChild(iNode);
		//// 헬퍼오브젝트 + 지오메트리 오브젝트
		//if (pChild->GetNodeAttribute() != nullptr)
		//{
		//	FbxNodeAttribute::EType type = pChild->GetNodeAttribute()->GetAttributeType();
		//	if (type == FbxNodeAttribute::eMesh ||
		//		type == FbxNodeAttribute::eSkeleton ||
		//		type == FbxNodeAttribute::eNull)
		//	{
		//		PreProcess(pChild);
		//	}
		//}
		//else
		{
			PreProcess(pChild);
		}		
	}
}

bool   TFbxLoader::Load(C_STR filename, 
				TKgcFileFormat* tKgcFileFormat)
{
	tKgcFileFormat->m_szFileName = to_mw(filename);

	m_pManager = FbxManager::Create();	
	m_pImporter = FbxImporter::Create(m_pManager, "");
	m_pScene = FbxScene::Create(m_pManager, "");

	bool bRet;
	// 파일을 로딩하고 
	bRet = m_pImporter->Initialize(filename.c_str());
	// 로딩된 정보를 m_pScene에 체운다.
	bRet = m_pImporter->Import(m_pScene);

	FbxAxisSystem::MayaZUp.ConvertScene(m_pScene);

	m_pRootNode = m_pScene->GetRootNode();
	PreProcess(m_pRootNode);	
	tKgcFileFormat->m_ptNodeList = m_pTNodeList;
	tKgcFileFormat->m_Header.iNumNodeCounter = m_pFbxNodeList.size();


	LoadNodeAnimation(tKgcFileFormat);
	
	for (int iMesh = 0; iMesh < m_pFbxMeshList.size(); iMesh++)
	{
		LoadMesh(iMesh, *tKgcFileFormat);
	}	
	tKgcFileFormat->m_matBindPose = m_matBindPose;	
	//LoadAnimation(tKgcFileFormat);

	m_pFbxMeshList.clear();
	m_pFbxMeshNodeList.clear();

	if (m_pScene) m_pScene->Destroy();
	if (m_pImporter) m_pImporter->Destroy();
	if (m_pManager) m_pManager->Destroy();

	m_pManager = nullptr;
	m_pScene = nullptr;
	m_pImporter = nullptr;
	return true;
}
// 어떤 방식(GetMappingMode)으로 매핑되었다.
	/*  eNone,				// 매핑이 결정되지 않았다.
		eByControlPoint,	// 제어점(정점)에 1개의 매핑 정보가 있다.
		eByPolygonVertex,	// 각 정점 당 1개의 매핑정보가 있다.
		eByPolygon,			// 1개의 면에 1개의 매핑정보가 있다.
		eByEdge,			// 에지 당 1개의 매핑정보가 있다.
		eAllSame*/			// 전체 면에 1개의 매핑정보가 있다.
		// GetReferenceMode()
		//	eDirect,			// 정점리스트의  n번째 위치에 매핑정보가 있다.
		//	eIndex,				// 6.0 이후로 eIndexToDirect 병합
		//	eIndexToDirect		// 정점리스트[n번째 위치]에  매핑정보가 있다.
FbxVector2  TFbxLoader::GetUV(
	FbxMesh* fbxMesh, FbxLayerElementUV* uvSet, int iVertexPosIndex, int iVertexUVIndex)
{
	FbxVector2 ret;
	// 어떤 방식으로 적용
	switch (uvSet->GetMappingMode())
	{
		case FbxLayerElementUV::eByControlPoint:
		{
			// 어떤 곳에 저장된 파악
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
				case FbxLayerElementUV::eDirect: // fbx 5이하
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
		// 어떤 곳에 저장된 파악
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
		// 제어점 당 1개의 정보가 있다.
	case FbxGeometryElement::eByControlPoint:
	{
		// 어떤 곳에 저장된 파악
		switch (VertexNormalSet->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			float x = VertexNormalSet->GetDirectArray().GetAt(iVertexPosIndex).mData[0];
			float z = VertexNormalSet->GetDirectArray().GetAt(iVertexPosIndex).mData[1];
			float y = VertexNormalSet->GetDirectArray().GetAt(iVertexPosIndex).mData[2];
		}break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int iIndex = VertexNormalSet->GetIndexArray().GetAt(iVertexPosIndex);
			ret = VertexNormalSet->GetDirectArray().GetAt(iIndex);
		}break;
		}
	}break;
		// 정점 당 1개의 정보가 있다.
	case FbxGeometryElement::eByPolygonVertex:
	{
		switch (VertexNormalSet->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			ret = VertexNormalSet->GetDirectArray().GetAt(iVertexNormalIndex);
		}break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int iColorIndex = VertexNormalSet->GetIndexArray().GetAt(iVertexNormalIndex);
			ret = VertexNormalSet->GetDirectArray().GetAt(iColorIndex);
		}break;
		}
	}break;
	}
	return ret;
}
// 폴리곤 당 사용하는 서브매터리얼 인덱스 반환.
int   TFbxLoader::GetSubMaterialPolygonIndex(int iPoly, 
	FbxLayerElementMaterial* pMaterial)
{
	int ret;
	switch (pMaterial->GetMappingMode())
	{
		case FbxLayerElement::eByPolygon:
		{
			// 어떤 곳에 저장된 파악
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
// 1) skinning 여부 ->  정보 얻기
// 2) object anim , skinning -> animation
// 3) object anim -> skinning 처리 
// 4) skinning    ->    ""
void   TFbxLoader::LoadMesh(int iMesh, TKgcFileFormat& model)
{	
	FbxMesh* fbxMesh = m_pFbxMeshList[iMesh];
	FbxNode* pFbxNode = fbxMesh->GetNode();

	//// 월드 변환 행렬
	FbxTime s;
	FbxLongLong tFrame = 0;
	s.SetFrame(tFrame);
	FbxAMatrix matWorld = pFbxNode->EvaluateGlobalTransform(s);
	std::shared_ptr<TKgcFileFormat>  pModel = std::make_shared<TKgcFileFormat>();
	pModel->m_matWorld = ConvertFbxAMatrix(matWorld);

	pModel->m_matBindPose.resize(m_pFbxNodeList.size());
	bool    bSkinned = ParseMeshSkinning(fbxMesh, *pModel.get());
	
	int  iBoneIndex = GetFbxNodeIndex(pFbxNode);

	// 로칼 변환 행렬
	FbxAMatrix geom;
	FbxVector4 trans	= pFbxNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxVector4 rot		= pFbxNode->GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 scale	= pFbxNode->GetGeometricScaling(FbxNode::eSourcePivot);
	geom.SetT(trans);
	geom.SetR(rot);
	geom.SetS(scale);
	// 노말 변환 행렬
	FbxAMatrix normalMatrix = geom;
	normalMatrix = normalMatrix.Inverse();
	normalMatrix = normalMatrix.Transpose();

	
	
		

	// Layer :  레이어 회수만큼 랜더링한다.
	std::vector<FbxLayerElementUV*>				VertexUVLayer;
	std::vector<FbxLayerElementVertexColor*>	VertexColorLayer;
	std::vector<FbxLayerElementNormal*>			VertexNormalLayer;
	std::vector<FbxLayerElementTangent*>		VertexTangentLayer;
	std::vector<FbxLayerElementMaterial*>		VertexMaterialLayer;
	int iLayerCount = fbxMesh->GetLayerCount();

	// 정점노말 (재)계산
	if (iLayerCount == 0 || fbxMesh->GetLayer(0)->GetNormals() == nullptr )
	{
		fbxMesh->InitNormals();
#if (FBXSDK_VERSION_MAJOR >= 2015)
		fbxMesh->GenerateNormals();
#else
		pFbxMesh->ComputeVertexNormals();
#endif
	}

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

	

	// material
	int iNumMtrl = pFbxNode->GetMaterialCount();
	if (iNumMtrl > 1)
	{
		pModel->m_vSubMeshVertexList.resize(iNumMtrl);
		pModel->m_vSubMeshIndexList.resize(iNumMtrl);
		pModel->m_vSubMeshIWVertexList.resize(iNumMtrl);
	}
	
	

	for (int iMtrl = 0; iMtrl < iNumMtrl; iMtrl++)
	{
		FbxSurfaceMaterial* pSurfaceMtrl = pFbxNode->GetMaterial(iMtrl);
		if (pSurfaceMtrl)
		{
			/*std::map<int, FbxProperty> prop;
			int iIndex = 0;
			prop[iIndex++] = pSurfaceMtrl->FindProperty(FbxSurfaceMaterial::sDiffuse);
			prop[iIndex++] = pSurfaceMtrl->FindProperty(FbxSurfaceMaterial::sDiffuseFactor);
			prop[iIndex++] = pSurfaceMtrl->FindProperty(FbxSurfaceMaterial::sReflection);*/

			std::string texPathName;
			auto Property = pSurfaceMtrl->FindProperty(FbxSurfaceMaterial::sDiffuse);
			if (Property.IsValid())
			{
				//auto iNumTex = Property.GetSrcObject<FbxFileTexture>();
				FbxFileTexture* tex = Property.GetSrcObject<FbxFileTexture>(0);
				if (tex)
				{
					texPathName = tex->GetFileName();
				}
			}
			pModel->m_szTexFileList.emplace_back(to_mw(texPathName));
		}
	}
	// 삼각형 몇개 
	// 폴리곤 = 사각형(삼각형2개) or 삼각형
	int iNumPolyCount = fbxMesh->GetPolygonCount();
	// 정점의 위치가 저장된 배열의 시작주소를 반환.
	FbxVector4* pVertexPositions = fbxMesh->GetControlPoints();
	int iNumControlPoint = fbxMesh->GetControlPointsCount();
	int iBasePolyIndex = 0;
	for (int iPoly = 0; iPoly < iNumPolyCount; iPoly++)
	{
		// 서브매터리얼 인덱스
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
			iVertexPositionIndex[0] = fbxMesh->GetPolygonVertex(iPoly, 0);
			iVertexPositionIndex[1] = fbxMesh->GetPolygonVertex(iPoly, iFace + 2);
			iVertexPositionIndex[2] = fbxMesh->GetPolygonVertex(iPoly, iFace + 1);

			int iVertexUVIndex[3];
			iVertexUVIndex[0] = fbxMesh->GetTextureUVIndex(iPoly, iVertexIndex[0]);
			iVertexUVIndex[1] = fbxMesh->GetTextureUVIndex(iPoly, iVertexIndex[1]);
			iVertexUVIndex[2] = fbxMesh->GetTextureUVIndex(iPoly, iVertexIndex[2]);

			for (int iVertex = 0; iVertex < 3; iVertex++)
			{
				// 정점 위치
				PNCT_Vertex v;
				FbxVector4 fbxV = pVertexPositions[iVertexPositionIndex[iVertex]];
				// neFbxV = fbxV * (geom * matWorld)
				fbxV = geom.MultT(fbxV);
				//fbxV = matWorld.MultT(fbxV);
				v.p.x = fbxV.mData[0];
				v.p.y = fbxV.mData[2];
				v.p.z = fbxV.mData[1];
				// 정점 텍스처 좌표			
				if (VertexUVLayer.size() > 0)
				{
					FbxVector2 uv = GetUV(fbxMesh, VertexUVLayer[0],
						iVertexPositionIndex[iVertex], iVertexUVIndex[iVertex]);
					v.t.x = uv.mData[0];
					v.t.y = 1.0f - uv.mData[1];
				}
				// 정점 컬러
				FbxColor color = FbxColor(1, 1, 1, 1);
				if (VertexColorLayer.size())
				{
					color = GetColor(fbxMesh, VertexColorLayer[0],
						iVertexPositionIndex[iVertex], 
						iBasePolyIndex + iVertexIndex[iVertex]);
				}
				v.c.x = color.mRed;
				v.c.y = color.mGreen;
				v.c.z = color.mBlue;
				v.c.w = color.mAlpha;
				// 정점노말

				FbxVector4 vFbxNormal = { 0,0,0 };
				if (VertexNormalLayer.size())
				{
					vFbxNormal = GetNormal(fbxMesh, VertexNormalLayer[0],
						iVertexPositionIndex[iVertex],
						iBasePolyIndex + iVertexIndex[iVertex]);
					vFbxNormal = normalMatrix.MultT(vFbxNormal);
					vFbxNormal.Normalize();
				}
				v.n.x = vFbxNormal.mData[0];
				v.n.y = vFbxNormal.mData[2];
				v.n.z = vFbxNormal.mData[1];

				// skinning
				IW_Vertex iwVertex;
				if (bSkinned)
				{
					TWeight& pWeight = pModel->m_WeightList[iVertexPositionIndex[iVertex]];
					for (int i = 0; i < pWeight.Index.size(); i++)
					{
						if (i < 4)
						{
							iwVertex.i0[i] = pWeight.Index[i];
							iwVertex.w0[i] = pWeight.weight[i];
						}
						else
						{
							iwVertex.i1[i-4] = pWeight.Index[i];
							iwVertex.w1[i-4] = pWeight.weight[i];
						}
					}
				}
				else
				{
					iwVertex.i0[0] = iBoneIndex;
					iwVertex.w0[0] = 1.0f;
				}

				if (pModel->m_vSubMeshVertexList.size() == 0)
				{
					//pModel->m_vVertexList.push_back(v);
					//pModel->m_vIWVertexList.push_back(iwVertex);
					bool bAddIWVertex = GenBuffer(pModel->m_vVertexList, pModel->m_vIndexList, v);
					if (bAddIWVertex)
					{
						pModel->m_vIWVertexList.emplace_back(iwVertex);
					}
				}
				else
				{
					pModel->m_vSubMeshVertexList[iSubMtrl].push_back(v);
					pModel->m_vSubMeshIWVertexList[iSubMtrl].push_back(iwVertex);
					/*bool bAddIWVertex = GenBuffer(pModel->m_vSubMeshVertexList[iSubMtrl],
							  pModel->m_vSubMeshIndexList[iSubMtrl], v);
					if (bAddIWVertex)
					{
						pModel->m_vSubMeshIWVertexList[iSubMtrl].emplace_back(iwVertex);
					}*/
				}
			}
		}

		iBasePolyIndex += iPolySize;
	}
	// 정점 몇개
	model.m_ChildList.emplace_back(pModel);
}
bool   TFbxLoader::GenBuffer(std::vector<PNCT_Vertex>& vList, 
						     std::vector<DWORD>& iList, PNCT_Vertex& v1)
{
	bool  bIWVertex = false;
	int iVertexIndex = -1;
	for (int iArray = 0; iArray < vList.size(); iArray++)
	{
		PNCT_Vertex v2 = vList[iArray];
		if (v1 == v2)
		{
			iVertexIndex = iArray;
			break;
		}
	}
	if (iVertexIndex < 0)
	{
		vList.emplace_back(v1);
		bIWVertex = true;
		iVertexIndex = vList.size() - 1;
	}
	iList.emplace_back(iVertexIndex);
	return bIWVertex;
}

void   TFbxLoader::Release()
{
	if (m_pManager) m_pManager->Destroy();
	m_pManager = nullptr;
}