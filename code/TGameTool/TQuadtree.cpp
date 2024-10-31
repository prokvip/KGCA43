#include "pch.h"
#include "TQuadtree.h"
void   TQuadtree::Init()
{
	m_pRootNode = new TNode;
	m_pRootNode->iCornerIndex[0] = 0; // TL
	m_pRootNode->iCornerIndex[1] = m_pMap->m_iNumCols-1; // TR
	m_pRootNode->iCornerIndex[2] = 
			(m_pMap->m_iNumRows-1) * m_pMap->m_iNumCols; // BL
	m_pRootNode->iCornerIndex[3] = m_pMap->m_iNumRows*m_pMap->m_iNumCols-1;//BR
}
TNode* TQuadtree::CreateNode(TNode* pNode, UINT TL, UINT TR, UINT BL, UINT BR)
{
	TNode* pChild = new TNode;	
	pChild->iCornerIndex[0] = TL;
	pChild->iCornerIndex[1] = TR;
	pChild->iCornerIndex[2] = BL;
	pChild->iCornerIndex[3] = BR;
	pChild->iDepth = pNode->iDepth + 1;
	return pChild;
}
bool   TQuadtree::SubDivide(TNode* pNode)
{
	UINT iHalf_X =  pNode->iCornerIndex[1] -
					pNode->iCornerIndex[0];
	UINT iHalf_Y = (pNode->iCornerIndex[2] -
					pNode->iCornerIndex[0]) /
					m_pMap->m_iNumCols;
	if (iHalf_X > 1 && iHalf_Y > 1)
	{
		// 0(TL)   1(CT)      2(TR)
		// 3(CL)   4(CC)	  5(CR)
		// 6(BL)   7(CB)      8(BR)
		UINT  TL = pNode->iCornerIndex[0];
		UINT  TR = pNode->iCornerIndex[1];
		UINT  BL = pNode->iCornerIndex[2];
		UINT  BR = pNode->iCornerIndex[3];
		UINT  CC = (TL + BR) / 2;
		UINT  CT = (TR + TL) / 2;
		UINT  CR = (TR + BR) / 2;
		UINT  CB = (BL + BR) / 2;
		UINT  CL = (TL + BL) / 2;
		// 0(TL)   1(CT)      2(TR)
		// 3(CL)   4(CC)	  5(CR)
		// 6(BL)   7(CB)      8(BR)
		pNode->pChild[0] = CreateNode(pNode, TL, CT, CL, CC);
		pNode->pChild[1] = CreateNode(pNode, CT, TR, CC, CR);
		pNode->pChild[2] = CreateNode(pNode, CL, CC, BL, CB);
		pNode->pChild[3] = CreateNode(pNode, CC, CR, CB, BR);		
		return true;
	}
	pNode->isLeaf = TRUE;
	return false;
}
void   TQuadtree::BuildTree(TNode* pNode)
{
	if (SubDivide(pNode))
	{
		for (int iNode = 0; iNode < 4; iNode++)
		{
			TNode* pChild = pNode->pChild[iNode];
			BuildTree(pChild);
		}
	}
}
void   TQuadtree::Frame()
{

}
void   TQuadtree::Render()
{

}
void   TQuadtree::Release()
{
}