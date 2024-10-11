#include "TFbxLoader.h"
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
							m_pFbxNodeList[iNode].get(), 
							model->m_ChildList[iNode].get());
	}
}