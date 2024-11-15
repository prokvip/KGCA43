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
		// FbxNode�� ������ ��ġ�� �������� ���
		for (int iCluster=0; iCluster < iClusterCounter; iCluster++)
		{
			auto pCluster = pSkin->GetCluster(iCluster);
			FbxNode* pFbxNode = pCluster->GetLink();
			std::wstring name = to_mw(pFbxNode->GetName());
			// FbxNode�� ���� ��ü �迭�־��
			// pFbxNode�� �ش��ϴ� ����� �ε����� ���� �Ѵ�.
			int  iBoneIndex = GetFbxNodeIndex(pFbxNode);

			FbxAMatrix matXBindPose;
			FbxAMatrix matReferenceGlobalInitPosition;
			pCluster->GetTransformLinkMatrix(matXBindPose);
			pCluster->GetTransformMatrix(matReferenceGlobalInitPosition);
			FbxAMatrix matBindPose = matReferenceGlobalInitPosition.Inverse() * matXBindPose;

			T::TMatrix matDxBindPose = ConvertFbxAMatrix(matBindPose);
			model.m_pFbxNodeBindPoseMatrixList[iBoneIndex] = matDxBindPose.Invert();
			model.m_pUsedBoneIndexList.push_back(iBoneIndex);

			// ������  ���Ե� ������ ������ 
			int iClusterSize = pCluster->GetControlPointIndicesCount();
			int* iFbxNodeIndex = pCluster->GetControlPointIndices();
			double* pWeights = pCluster->GetControlPointWeights();
			for (int v = 0; v < iClusterSize; v++)
			{
				// v�� ������ boneindex�� ��Ŀ� weight���� ����ġ�� ������ �޴´�.
				int    iVertexIndex = iFbxNodeIndex[v];
				float  fWeight = pWeights[v];
				model.m_WeightList[iVertexIndex].Insert(iBoneIndex, fWeight);
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
		// EvaluateGlobalTransform(�����������) = SelfAnimation(SRT����) * ParentAnimation(SRT����)
		FbxAMatrix matWorld = pTFbxNode->pFbxNode->EvaluateGlobalTransform(s);
		T::TMatrix matFrame = ConvertFbxAMatrix(matWorld);
		tKgcFileFormat->m_pAnimationMatrix.emplace_back(matFrame);
	}
}

void   TFbxLoader::LoadNodeAnimation(TKgcFileFormat* model)
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
		model->m_Header.iStartFrame = sFrame;
		model->m_Header.iLastFrame = eFrame;
		model->m_Header.iFrameSpeed = 30;

		int iNumAnimFrame = model->m_Header.iLastFrame - model->m_Header.iStartFrame;
		// 71 * iNumAnimFrame;
		std::vector<FbxTime>  s;
		s.resize(iNumAnimFrame);
		for (int iFrame = sFrame; iFrame < eFrame; iFrame++)
		{
			s[iFrame].SetFrame(iFrame, timeMode);
		}
		// bone 71
		model->m_pBoneAnimMatrix.resize(m_pFbxNodeList.size());
		m_pFbxNodeBindPoseMatrixList.resize(m_pFbxNodeList.size());
		// biped + bone + dummy + mesh
		for (int fbxnode = 0; fbxnode < m_pFbxNodeList.size(); fbxnode++)
		{
			// bone per frame
			model->m_pBoneAnimMatrix[fbxnode].resize(iNumAnimFrame);
			for (int iFrame = sFrame; iFrame < eFrame; iFrame++)
			{
				FbxAMatrix matWorld = m_pFbxNodeList[fbxnode]->EvaluateGlobalTransform(s[iFrame]);
				T::TMatrix matFrame = ConvertFbxAMatrix(matWorld);
				model->m_pBoneAnimMatrix[fbxnode][iFrame] = matFrame;
			}
			m_pFbxNodeBindPoseMatrixList[fbxnode] = model->m_pBoneAnimMatrix[fbxnode][sFrame];
		}		
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
		model->m_Header.iStartFrame = sFrame;
		model->m_Header.iLastFrame = eFrame;
		model->m_Header.iFrameSpeed = 30;
	}

	for (int iNode = 0; iNode < model->m_ChildList.size(); iNode++)
	{
		LoadAnimationNode(model->m_Header,
			m_pFbxMeshNodeList[iNode].get(),
			model->m_ChildList[iNode].get());
	}
}