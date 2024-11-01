#include "TMap.h"
#include "TCamera.h"
#include "TLineObj.h"
struct T_AABB
{
	T::TVector3 vMin;
	T::TVector3 vMax;
};
struct T_OBB
{
	T::TVector3 vCenter;
	T::TVector3 vAxis[3];
	float       fDistance[3];
};
struct TBox
{
	/*T_AABB     aabb;
	T_OBB      obb;*/
	// AABB
	T::TVector3 vMin;
	T::TVector3 vMax;
	// OBB
	T::TVector3 vCenter;
	T::TVector3 vAxis[3];
	float       fDistance[3];
};
struct TSphere
{
	T::TVector3 vCenter;
	float m_fRadius;
};
struct TNode
{
	UINT   iDepth;
	BOOL   isLeaf;
	UINT   iCornerIndex[4];
	TBox    m_Box;
	TSphere m_Sphere;
	TNode*  pChild[4];
	std::vector<UINT>  m_VertexIndex;
	std::vector<UINT> m_IndexList;
	ComPtr<ID3D11Buffer> m_pIndexBuffer;
	TNode()
	{
		iDepth = 0;
		isLeaf = FALSE;
		iCornerIndex[0] = 0;
		iCornerIndex[1] = 0;
		iCornerIndex[2] = 0;
		iCornerIndex[3] = 0;
		pChild[0] = nullptr;
		pChild[1] = nullptr;
		pChild[2] = nullptr;
		pChild[3] = nullptr;
	}
	~TNode()
	{
		if( pChild[0] != nullptr) delete pChild[0];
		if (pChild[1] != nullptr) delete pChild[1];
		if (pChild[2] != nullptr) delete pChild[2];
		if (pChild[3] != nullptr) delete pChild[3];
	}
};
class TQuadtree
{
public:
	TLineObj		m_Line;
	TMap*	m_pMap = nullptr;
	TCamera* m_pCamera = nullptr;
	TNode*  m_pRootNode = nullptr;
	std::vector<TNode*>  m_LeafNodes;
	int		m_iMaxDepth = 3;
public:
	void   BuildTree(TNode* pNode);
	bool   SubDivide(TNode* pNode);
	TNode* CreateNode(TNode* pNode, UINT TL, UINT TR, UINT BL, UINT BR);
	void   PostRender(ID3D11DeviceContext* pContext, TNode* pNode);
	void   Init();
	void   Frame();
	void   Render();
	void   Release();

	void   DrawNodeBB(TNode* pNode);
	void   DrawBB(TNode* pNode);
};

