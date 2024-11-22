#pragma once
#include "TDxObj3D.h"
#define MAX_BONE_MATRICES 255

struct TScene
{
	int		iStartFrame = 0;
	int		iLastFrame = 0;
	int		iFrameSpeed = 30;
};
struct TFbxNode
{
	BOOL         isMesh = FALSE;
	WCHAR        szName[32];
	WCHAR        szParentName[32];
};

// 정점 단위로 생성한다.
struct TWeight
{
	std::vector<int>   Index;
	std::vector<float> weight;
	void Insert(int iIndex, float fWeight)
	{
		for (DWORD i = 0; i < Index.size(); ++i)
		{
			if (fWeight > weight[i])
			{
				for (DWORD j = (Index.size() - 1); j > i; --j)
				{
					Index[j] = Index[j - 1];
					weight[j] = weight[j - 1];
				}
				Index[i] = iIndex;
				weight[i] = fWeight;
				break;
			}
		}
	}
	TWeight()
	{
		Index.resize(8);
		weight.resize(8);
	}
};
struct TKgcFileHeader
{
	int				iVersion = 100;
	int				iNumNodeCounter = 0;
	int				isSubMesh = 0; // 서브메터리얼 유무
	int				iStartFrame = 0;
	int				iLastFrame = 0;
	int				iFrameSpeed = 30;
	int				iLength = 0;
	int				iNumTrack = 0;
	int				iChildNodeCounter = 0;
	int				iSubVertexBufferCounter = 0;
	int				iSubIndexBufferCounter = 0;
	int				iSubIWVertexBufferCounter = 0;
	T::TMatrix		matWorld;
};
struct TKgcFileTrack
{
	std::vector<T::TMatrix> pAnimationMatrix;
};
struct TTexFileHeader
{
	int				iLength = 0;
	int				iType = 0;
};
struct  TBoneMatrix
{
	T::TMatrix  matBone[MAX_BONE_MATRICES];
};

class TFbxModel;

class TKgcFileFormat
{
public:
	TKgcFileHeader			m_Header;
	std::wstring			m_szFileName;
	T::TMatrix				m_matWorld;
	std::vector<std::wstring> m_szTexFileList;
	using vList = std::vector<PNCT_Vertex>;
	using iList = std::vector<DWORD>;
	using iwList = std::vector<IW_Vertex>;
	vList			m_vVertexList;		 // 프레임 화면 정보
	iList			m_vIndexList;
	std::vector<vList>		m_vSubMeshVertexList;
	std::vector<iList>		m_vSubMeshIndexList;
	std::vector<T::TMatrix> m_pAnimationMatrix;
	std::vector<std::shared_ptr<TKgcFileFormat>> m_ChildList;
	// 모든 행렬에 대한 에니메이션 프레임 저장
	using boneFrameMatrix = std::vector<T::TMatrix>;
	std::vector<boneFrameMatrix> m_pBoneAnimMatrix;
	// Skinning : 전체 트리 노드가 본 좌표계로 변환되는 행렬
	std::vector<T::TMatrix>		m_matBindPose;
	std::vector<int>			m_pUsedBoneIndexList;
	std::vector<TFbxNode>		m_ptNodeList;

	std::vector< TWeight>       m_WeightList;
	using vIWList = std::vector<IW_Vertex>;
	vIWList						m_vVertexListIndexWegiht;
	std::vector<vIWList>		m_vSubMeshIWVertexList;
	std::vector<IW_Vertex>		m_vIWVertexList;


	static bool Export(TKgcFileFormat* tFile, std::wstring szFileName);
	static bool Import(std::wstring szFileName, std::wstring szShaderFile,
		std::shared_ptr<TFbxModel>& tFbxModel);
};