#include "TMap.h"

struct TNode
{
	UINT   iDepth;
	BOOL   isLeaf;
	UINT   iCornerIndex[4];
	TNode*  pChild[4];
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
};
class TQuadtree
{
public:
	TMap*	m_pMap = nullptr;
	TNode*	m_pRootNode= nullptr;
public:
	void   BuildTree(TNode* pNode);
	bool   SubDivide(TNode* pNode);
	TNode* CreateNode(TNode* pNode, UINT TL, UINT TR, UINT BL, UINT BR);
	void   Init();
	void   Frame();
	void   Render();
	void   Release();
};

