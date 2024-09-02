#include "TBoxObj.h"

void   TBoxObj::SetVertexData()
{
	//local
	m_vVertexList.resize(24);
	// »ó´Ü
	// 5    6
	// 1    2
	// ÇÏ´Ü
	// 4    7
	// 0    3  
	// ¾Õ¸é
	m_vVertexList.resize(24);
	m_vVertexList[0] = PNCT_Vertex(FVector3(-1.0f, 1.0f, -1.0f), FVector3(0.0f, 0.0f, -1.0f), FVector4(1.0f, 0.0f, 0.0f, 1.0f), FVector2(0.0f, 0.0f));
	m_vVertexList[1] = PNCT_Vertex(FVector3(1.0f, 1.0f, -1.0f), FVector3(0.0f, 0.0f, -1.0f), FVector4(1.0f, 0.0f, 0.0f, 1.0f), FVector2(1.0f, 0.0f));
	m_vVertexList[2] = PNCT_Vertex(FVector3(1.0f, -1.0f, -1.0f), FVector3(0.0f, 0.0f, -1.0f), FVector4(1.0f, 0.0f, 0.0f, 1.0f), FVector2(1.0f, 1.0f));
	m_vVertexList[3] = PNCT_Vertex(FVector3(-1.0f, -1.0f, -1.0f), FVector3(0.0f, 0.0f, -1.0f), FVector4(1.0f, 0.0f, 0.0f, 1.0f), FVector2(0.0f, 1.0f));
	// µÞ¸é
	m_vVertexList[4] = PNCT_Vertex(FVector3(1.0f, 1.0f, 1.0f), FVector3(0.0f, 0.0f, 1.0f), FVector4(0.0f, 0.0f, 0.0f, 1.0f), FVector2(0.0f, 0.0f));
	m_vVertexList[5] = PNCT_Vertex(FVector3(-1.0f, 1.0f, 1.0f), FVector3(0.0f, 0.0f, 1.0f), FVector4(0.0f, 1.0f, 0.0f, 1.0f), FVector2(1.0f, 0.0f));
	m_vVertexList[6] = PNCT_Vertex(FVector3(-1.0f, -1.0f, 1.0f), FVector3(0.0f, 0.0f, 1.0f), FVector4(0.0f, 1.0f, 0.0f, 1.0f), FVector2(1.0f, 1.0f));
	m_vVertexList[7] = PNCT_Vertex(FVector3(1.0f, -1.0f, 1.0f), FVector3(0.0f, 0.0f, 1.0f), FVector4(0.0f, 1.0f, 0.0f, 1.0f), FVector2(0.0f, 1.0f));

	// ¿À¸¥ÂÊ
	m_vVertexList[8] = PNCT_Vertex(FVector3(1.0f, 1.0f, -1.0f), FVector3(1.0f, 0.0f, 0.0f), FVector4(0.0f, 0.0f, 1.0f, 1.0f), FVector2(0.0f, 0.0f));
	m_vVertexList[9] = PNCT_Vertex(FVector3(1.0f, 1.0f, 1.0f), FVector3(1.0f, 0.0f, 0.0f), FVector4(0.0f, 0.0f, 1.0f, 1.0f), FVector2(1.0f, 0.0f));
	m_vVertexList[10] = PNCT_Vertex(FVector3(1.0f, -1.0f, 1.0f), FVector3(1.0f, 0.0f, 0.0f), FVector4(0.0f, 0.0f, 1.0f, 1.0f), FVector2(1.0f, 1.0f));
	m_vVertexList[11] = PNCT_Vertex(FVector3(1.0f, -1.0f, -1.0f), FVector3(1.0f, 0.0f, 0.0f), FVector4(0.0f, 0.0f, 1.0f, 1.0f), FVector2(0.0f, 1.0f));

	// ¿ÞÂÊ
	m_vVertexList[12] = PNCT_Vertex(FVector3(-1.0f, 1.0f, 1.0f), FVector3(-1.0f, 0.0f, 0.0f), FVector4(1.0f, 1.0f, 0.0f, 1.0f), FVector2(0.0f, 0.0f));
	m_vVertexList[13] = PNCT_Vertex(FVector3(-1.0f, 1.0f, -1.0f), FVector3(-1.0f, 0.0f, 0.0f), FVector4(1.0f, 1.0f, 0.0f, 1.0f), FVector2(1.0f, 0.0f));
	m_vVertexList[14] = PNCT_Vertex(FVector3(-1.0f, -1.0f, -1.0f), FVector3(-1.0f, 0.0f, 0.0f), FVector4(1.0f, 1.0f, 0.0f, 1.0f), FVector2(1.0f, 1.0f));
	m_vVertexList[15] = PNCT_Vertex(FVector3(-1.0f, -1.0f, 1.0f), FVector3(-1.0f, 0.0f, 0.0f), FVector4(1.0f, 1.0f, 0.0f, 1.0f), FVector2(0.0f, 1.0f));

	// À­¸é
	m_vVertexList[16] = PNCT_Vertex(FVector3(-1.0f, 1.0f, 1.0f), FVector3(0.0f, 1.0f, 0.0f), FVector4(1.0f, 0.5f, 1.0f, 1.0f), FVector2(0.0f, 0.0f));
	m_vVertexList[17] = PNCT_Vertex(FVector3(1.0f, 1.0f, 1.0f), FVector3(0.0f, 1.0f, 0.0f), FVector4(1.0f, 0.5f, 1.0f, 1.0f), FVector2(1.0f, 0.0f));
	m_vVertexList[18] = PNCT_Vertex(FVector3(1.0f, 1.0f, -1.0f), FVector3(0.0f, 1.0f, 0.0f), FVector4(1.0f, 0.5f, 1.0f, 1.0f), FVector2(1.0f, 1.0f));
	m_vVertexList[19] = PNCT_Vertex(FVector3(-1.0f, 1.0f, -1.0f), FVector3(0.0f, 1.0f, 0.0f), FVector4(1.0f, 0.5f, 1.0f, 1.0f), FVector2(0.0f, 1.0f));

	// ¾Æ·§¸é
	m_vVertexList[20] = PNCT_Vertex(FVector3(-1.0f, -1.0f, -1.0f), FVector3(0.0f, -1.0f, 0.0f), FVector4(0.0f, 1.0f, 1.0f, 1.0f), FVector2(0.0f, 0.0f));
	m_vVertexList[21] = PNCT_Vertex(FVector3(1.0f, -1.0f, -1.0f), FVector3(0.0f, -1.0f, 0.0f), FVector4(0.0f, 1.0f, 1.0f, 1.0f), FVector2(1.0f, 0.0f));
	m_vVertexList[22] = PNCT_Vertex(FVector3(1.0f, -1.0f, 1.0f), FVector3(0.0f, -1.0f, 0.0f), FVector4(0.0f, 1.0f, 1.0f, 1.0f), FVector2(1.0f, 1.0f));
	m_vVertexList[23] = PNCT_Vertex(FVector3(-1.0f, -1.0f, 1.0f), FVector3(0.0f, -1.0f, 0.0f), FVector4(0.0f, 1.0f, 1.0f, 1.0f), FVector2(0.0f, 1.0f));


}
void   TBoxObj::SetIndexData()
{
	m_vIndexList.resize(36);
	int iIndex = 0;
	m_vIndexList[iIndex++] = 0; 	m_vIndexList[iIndex++] = 1; 	m_vIndexList[iIndex++] = 2; 	m_vIndexList[iIndex++] = 0;	m_vIndexList[iIndex++] = 2; 	m_vIndexList[iIndex++] = 3;
	m_vIndexList[iIndex++] = 4; 	m_vIndexList[iIndex++] = 5; 	m_vIndexList[iIndex++] = 6; 	m_vIndexList[iIndex++] = 4;	m_vIndexList[iIndex++] = 6; 	m_vIndexList[iIndex++] = 7;
	m_vIndexList[iIndex++] = 8; 	m_vIndexList[iIndex++] = 9; 	m_vIndexList[iIndex++] = 10; m_vIndexList[iIndex++] = 8;	m_vIndexList[iIndex++] = 10; m_vIndexList[iIndex++] = 11;
	m_vIndexList[iIndex++] = 12; m_vIndexList[iIndex++] = 13; m_vIndexList[iIndex++] = 14; m_vIndexList[iIndex++] = 12;	m_vIndexList[iIndex++] = 14; m_vIndexList[iIndex++] = 15;
	m_vIndexList[iIndex++] = 16; m_vIndexList[iIndex++] = 17; m_vIndexList[iIndex++] = 18; m_vIndexList[iIndex++] = 16;	m_vIndexList[iIndex++] = 18; m_vIndexList[iIndex++] = 19;
	m_vIndexList[iIndex++] = 20; m_vIndexList[iIndex++] = 21; m_vIndexList[iIndex++] = 22; m_vIndexList[iIndex++] = 20;	m_vIndexList[iIndex++] = 22; m_vIndexList[iIndex++] = 23;

}
void   TBoxObj::Frame()
{
	/*static FVector3 vPos = { 0, 0, 0 };
	vPos.X = cosf(g_fGameTime) * 2.0f;*/

	T::TMatrix  matScale, matTrans, matRotate;
	T::D3DXMatrixTranslation(&matTrans, m_vPos.x, m_vPos.y, m_vPos.z);
	T::D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	T::D3DXMatrixRotationY(&matRotate, g_fGameTime);
	/*matTrans.Translation(m_vPos);
	matScale..Scale(m_vScale);
	matRotate.rotateY(g_fGameTime);*/
	m_matWorld = matScale * matRotate * matTrans;
}