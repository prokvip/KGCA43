#include "TFbxLoader.h"
int  TFbxLoader::GetFbxNodeIndex(FbxNode* fbxNode)
{
	for (int index = 0; index < m_pFbxNodeList.size(); index++)
	{
		if (m_pFbxNodeList[index] == fbxNode)
		{
			return index;
		}
	}
	return -1;
}
bool  TFbxLoader::ParseMeshSkinning(FbxMesh* fbxMesh, TKgcFileFormat& model)
{
	//model.m_WeightList.clear();
	// skin 
	int iDeformerCount = fbxMesh->GetDeformerCount(FbxDeformer::eSkin);
	if (iDeformerCount == 0) return false;

	// mesh iNumVertexCount == iVertexCount;
	int iVertexCount = fbxMesh->GetControlPointsCount();
	model.m_WeightList.resize(iVertexCount);
	

	for (int iDeformer = 0; iDeformer < iDeformerCount; iDeformer++)
	{
		FbxSkin* pSkin = 
			reinterpret_cast<FbxSkin*>(fbxMesh->GetDeformer(iDeformer, FbxDeformer::eSkin));
		int  iClusterCounter = pSkin->GetClusterCount();
		// FbxNode가 영향을 미치는 정점들의 덩어리
		for (int iCluster=0; iCluster < iClusterCounter; iCluster++)
		{
			auto pCluster = pSkin->GetCluster(iCluster);
			FbxNode* pFbxNode = pCluster->GetLink();
			std::wstring name = to_mw(pFbxNode->GetName());
			// FbxNode에 대한 전체 배열있어야
			// pFbxNode에 해당하는 노드의 인덱스를 얻어야 한다.
			int  iBoneIndex = GetFbxNodeIndex(pFbxNode);

			FbxAMatrix matXBindPose;
			FbxAMatrix matReferenceGlobalInitPosition;
			pCluster->GetTransformLinkMatrix(matXBindPose);
			pCluster->GetTransformMatrix(matReferenceGlobalInitPosition);
			FbxAMatrix matBindPose = matReferenceGlobalInitPosition.Inverse() * matXBindPose;

			T::TMatrix matDxBindPose = ConvertFbxAMatrix(matBindPose);
			model.m_pFbxNodeBindPoseMatrixList[iBoneIndex] = matDxBindPose.Invert();
			model.m_pUsedBoneIndexList.push_back(iBoneIndex);

			// 묶은에  포함된 정점의 개수가 
			int iClusterSize = pCluster->GetControlPointIndicesCount();
			int* iFbxNodeIndex = pCluster->GetControlPointIndices();
			double* pWeights = pCluster->GetControlPointWeights();
			for (int v = 0; v < iClusterSize; v++)
			{
				// v번 정점은 boneindex의 행렬에 weight값의 가중치로 영향을 받는다.
				int    iVertexIndex = iFbxNodeIndex[v];
				float  fWeight = pWeights[v];
				model.m_WeightList[iVertexIndex].Index.push_back(iBoneIndex);
				model.m_WeightList[iVertexIndex].weight.push_back(fWeight);
			}

		}
	}
	return true;
}
void   TFbxLoader::LoadAnimationNode(
	TKgcFileHeader header, 
	TFbxNode* pTFbxNode,
	TKgcFileFormat* tKgcFileFormat)
{
	FbxTime::EMode timeMode = FbxTime::GetGlobalTimeMode();
	FbxTime s;
	for (int	iFrame = header.iStartFrame;
		iFrame < header.iLastFrame;
		iFrame++)
	{
		s.SetFrame(iFrame, timeMode);
		// EvaluateGlobalTransform(최종월드행렬) = SelfAnimation(SRT보간) * ParentAnimation(SRT보간)
		FbxAMatrix matWorld = pTFbxNode->pFbxNode->EvaluateGlobalTransform(s);
		T::TMatrix matFrame = ConvertFbxAMatrix(matWorld);
		tKgcFileFormat->m_pAnimationMatrix.emplace_back(matFrame);
	}
}
void   TFbxLoader::LoadAnimation(TKgcFileFormat* model)
{
	FbxTime::SetGlobalTimeMode(FbxTime::eFrames30);
	FbxAnimStack* stack = m_pScene->GetSrcObject<FbxAnimStack>(0);
	if (stack == nullptr) return;
	FbxString TakeName = stack->GetName();
	FbxTakeInfo* TakeInfo = m_pScene->GetTakeInfo(TakeName);
	if (TakeInfo)
	{
		FbxTime start = TakeInfo->mLocalTimeSpan.GetStart();
		FbxTime end = TakeInfo->mLocalTimeSpan.GetStop();

		FbxTime::EMode timeMode = FbxTime::GetGlobalTimeMode();
		FbxLongLong sFrame = start.GetFrameCount(timeMode);
		FbxLongLong eFrame = end.GetFrameCount(timeMode);
		FbxTime s;

		model->m_Header.iStartFrame = sFrame;
		model->m_Header.iLastFrame = eFrame;
		model->m_Header.iFrameSpeed = 30;
	}
	for (int iNode = 0; iNode < model->m_ChildList.size(); iNode++)
	{
		LoadAnimationNode(	model->m_Header, 
							m_pFbxMeshNodeList[iNode].get(), 
							model->m_ChildList[iNode].get());
	}
}