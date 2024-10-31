#include "pch.h"
#include "TQuadtree.h"
#include "TDxObject.h"
void   TQuadtree::Init()
{
	m_pRootNode = CreateNode(nullptr, 
		0, m_pMap->m_iNumCols - 1, 
		(m_pMap->m_iNumRows - 1) * m_pMap->m_iNumCols,
		m_pMap->m_iNumRows * m_pMap->m_iNumCols - 1);	
}
TNode* TQuadtree::CreateNode(TNode* pNode, UINT TL, UINT TR, UINT BL, UINT BR)
{
	TNode* pChild = new TNode;	
	pChild->iCornerIndex[0] = TL; 	pChild->iCornerIndex[1] = TR;
	pChild->iCornerIndex[2] = BL;	pChild->iCornerIndex[3] = BR;
	if (pNode != nullptr)
	{
		pChild->iDepth = pNode->iDepth + 1;
	}
	// 0      2      4
	// 5      7      9 
	// 10 11 12 13  14
	// 15    17 18  19
	// 20    22     24
	
	UINT iNumCols = (TR - TL);
	UINT iNumWidth = m_pMap->m_iNumRows;
	UINT iNumFace = iNumCols * ((BL - TL)/ iNumWidth) * 2;
	UINT iNumIndex = iNumFace * 3;
	pChild->m_IndexList.resize(iNumIndex);

	// vb->map,  node->ib
	
	UINT iStartRow = TL / iNumWidth;
	UINT IEndRow = BL  / iNumWidth;
	UINT iStartCol = TL % iNumWidth;
	UINT iEndCol = TR % iNumWidth;

	UINT iIndex = 0;
	for (UINT iRow = iStartRow; iRow < IEndRow; iRow++)
	{
		for (UINT iCol = iStartCol; iCol < iEndCol; iCol++)
		{
			UINT iNextRow = iRow + 1;
			UINT iNextCol = iCol + 1;
			pChild->m_IndexList[iIndex++] = iRow* iNumWidth + iCol;
			pChild->m_IndexList[iIndex++] = iRow * iNumWidth + iNextCol;
			pChild->m_IndexList[iIndex++] = iNextRow* iNumWidth + iCol;

			pChild->m_IndexList[iIndex++] = iNextRow * iNumWidth + iCol;
			pChild->m_IndexList[iIndex++] = iRow * iNumWidth + iNextCol;
			pChild->m_IndexList[iIndex++] = iNextRow * iNumWidth + iNextCol;
		}
	}

	pChild->m_pIndexBuffer.Attach(
		DX::CreateIndexBuffer(TDevice::m_pd3dDevice.Get(), 
			&pChild->m_IndexList.at(0), iNumIndex, sizeof(UINT)));
	return pChild;
}
void     TQuadtree::PostRender(ID3D11DeviceContext* pContext, TNode* pNode)
{
	if (pNode->m_pIndexBuffer != nullptr)
	{
		pContext->DrawIndexed(pNode->m_IndexList.size(), 0, 0);
	}
	else
	{
		// ONLY VB
		//pContext->Draw(pNode->m_VertexList.size(), 0);
	}
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
	m_LeafNodes.push_back(pNode);
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
	for (auto node : m_LeafNodes)
	{		
		TDevice::m_pContext->IASetIndexBuffer(
			node->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		PostRender(TDevice::m_pContext, node);
	}
}
void   TQuadtree::Release()
{
	delete m_pRootNode;
}