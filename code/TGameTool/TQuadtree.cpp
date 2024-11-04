#include "pch.h"
#include "TQuadtree.h"
#include "TDxObject.h"
void   TQuadtree::Init()
{
	m_pRootNode = CreateNode(nullptr, 
		0, m_pMap->m_iNumCols - 1, 
		(m_pMap->m_iNumRows - 1) * m_pMap->m_iNumCols,
		m_pMap->m_iNumRows * m_pMap->m_iNumCols - 1);	

	m_Line.Create(L"../../data/obj.jpg", L"../../data/shader/line.hlsl");
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

	for (UINT iRow = iStartRow; iRow < IEndRow+1; iRow++)
	{
		for (UINT iCol = iStartCol; iCol < iEndCol+1; iCol++)
		{
			pChild->m_VertexIndex.push_back(iRow * iNumWidth + iCol);
		}
	}
	pChild->m_pIndexBuffer.Attach(
		DX::CreateIndexBuffer(TDevice::m_pd3dDevice.Get(), 
			&pChild->m_IndexList.at(0), iNumIndex, sizeof(UINT)));


	// bounding box
	pChild->m_Box.vMin = { 99999.0f, 99999.0f, 99999.0f };
	pChild->m_Box.vMax = { -99999.0f, -99999.0f, -99999.0f };

	for (int i = 0; i < pChild->m_VertexIndex.size(); i++)
	{
		T::TVector3 v = m_pMap->m_vVertexList[pChild->m_VertexIndex[i]].p;
		if (v.x > pChild->m_Box.vMax.x)
		{
			pChild->m_Box.vMax.x = v.x;
		}
		if (v.y > pChild->m_Box.vMax.y)
		{
			pChild->m_Box.vMax.y = v.y;
		}
		if (v.z > pChild->m_Box.vMax.z)
		{
			pChild->m_Box.vMax.z = v.z;
		}
		if (v.x < pChild->m_Box.vMin.x)
		{
			pChild->m_Box.vMin.x = v.x;
		}
		if (v.y < pChild->m_Box.vMin.y)
		{
			pChild->m_Box.vMin.y = v.y;
		}
		if (v.z < pChild->m_Box.vMin.z)
		{
			pChild->m_Box.vMin.z = v.z;
		}
	}

	pChild->m_Sphere.vCenter = (pChild->m_Box.vMax + pChild->m_Box.vMin) * 0.5f;
	pChild->m_Sphere.m_fRadius = (pChild->m_Box.vMax - pChild->m_Sphere.vCenter).Length();
	return pChild;
}
void     TQuadtree::DrawNodeBB(TNode* pNode)
{
	T_Box& box = pNode->m_Box;
	//m_Line.SetMatrix(nullptr, &m_MainCamera.m_matView, &m_matProj);
	T::TVector3 v[8];
	v[0] = { box.vMin.x, box.vMin.y, box.vMin.z};
	v[1] = { box.vMin.x, box.vMax.y, box.vMin.z };
	v[2] = { box.vMax.x, box.vMax.y, box.vMin.z };
	v[3] = { box.vMax.x, box.vMin.y, box.vMin.z };

	v[4] = { box.vMin.x, box.vMin.y, box.vMax.z };
	v[5] = { box.vMin.x, box.vMax.y, box.vMax.z };
	v[6] = { box.vMax.x, box.vMax.y, box.vMax.z };
	v[7] = { box.vMax.x, box.vMin.y, box.vMax.z };

	T::TVector4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	//T::TVector4 color = { 1.0f, 0.0f, 0.0f, 1.0f };
	m_Line.Draw(v[0], v[1], color);
	m_Line.Draw(v[1], v[2], color);
	m_Line.Draw(v[2], v[3], color);
	m_Line.Draw(v[3], v[0], color);


	m_Line.Draw(v[4], v[5], color);
	m_Line.Draw(v[5], v[6], color);
	m_Line.Draw(v[6], v[7], color);
	m_Line.Draw(v[7], v[4], color);


	m_Line.Draw(v[0], v[4], color);
	m_Line.Draw(v[1], v[5], color);
	m_Line.Draw(v[2], v[6], color);
	m_Line.Draw(v[3], v[7], color);
}
void     TQuadtree::DrawBB(TNode* pNode)
{
	if (pNode != nullptr)
	{
		DrawNodeBB(pNode);
		for (int iNode = 0; iNode < 4; iNode++)
		{
			TNode* pChild = pNode->pChild[iNode];
			DrawBB(pChild);
		}
	}
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
	
	if (pNode->iDepth < m_iMaxDepth &&iHalf_X > 1 && iHalf_Y > 1)
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
	//TNode* pNode = m_pRootNode->pChild[2];
	for (auto pNode : m_LeafNodes)
	{		
		TDevice::m_pContext->IASetIndexBuffer(
			pNode->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		PostRender(TDevice::m_pContext, pNode);
		//break;
	}
}
void   TQuadtree::Release()
{
	m_Line.Release();
	delete m_pRootNode;
}