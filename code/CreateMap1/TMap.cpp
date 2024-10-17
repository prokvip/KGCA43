#include "TMap.h"

bool TMap::CreateMap(TMapDesc& desc)
{
	m_mapDesc = desc;
	m_iNumCols = desc.iNumCols;
	m_iNumRows = desc.iNumRows;
	m_iNumCellCols = m_iNumCols - 1;
	m_iNumCellRows = m_iNumRows - 1;
	m_iNumVertices = m_iNumCols * m_iNumRows;
	m_iNumFace		= m_iNumCellCols * m_iNumCellRows* 2.0f;
	m_fCellDistance = desc.fCellDistance;


	return Create(m_mapDesc.strTextureFile,
				  m_mapDesc.strShaderFile);
}
bool TMap::Load(TMapDesc& desc)
{
	if (CreateMap(desc))
	{
		return true;
	}
	return false;
}
void	 TMap::SetVertexData()
{
	m_vVertexList.resize(m_iNumVertices);
	float fHalfCol = (m_iNumCols-1) / 2.0f;
	float fHalfRow = (m_iNumRows - 1) / 2.0f;
	float fTexOffsetU = 1.0f / (m_iNumCols - 1);
	float fTexOffsetV = 1.0f / (m_iNumRows - 1);

	for (int iRow = 0; iRow < m_iNumRows; iRow++)
	{
		for (int iCol = 0; iCol < m_iNumCols; iCol++)
		{
			int iIndex = iRow * m_iNumCols + iCol;
			m_vVertexList[iIndex].p.x = (iCol-fHalfCol) * m_fCellDistance;			
			m_vVertexList[iIndex].p.z = -((iRow- fHalfRow) * m_fCellDistance);
			m_vVertexList[iIndex].p.y = m_HeightList[iIndex]* m_mapDesc.fScaleHeight;

			m_vVertexList[iIndex].t.x = fTexOffsetU * iCol;
			m_vVertexList[iIndex].t.y = fTexOffsetV * iRow;

			m_vVertexList[iIndex].n = T::TVector3(0,0,0);
			m_vVertexList[iIndex].c = T::TVector4(1, 1, 1,1);			
		}
	}
}
void	 TMap::SetIndexData()
{
	m_vIndexList.resize(m_iNumFace*3);
	int iCurIndex = 0;
	for (int iRow = 0; iRow < m_iNumCellRows; iRow++)
	{
		for (int iCol = 0; iCol < m_iNumCellCols; iCol++)
		{
			int iNextRow = iRow + 1;
			int iNextCol = iCol + 1;
			m_vIndexList[iCurIndex + 0] = iRow * m_iNumCols + iCol;
			m_vIndexList[iCurIndex + 1] = iRow * m_iNumCols + iNextCol;
			m_vIndexList[iCurIndex + 2] = iNextRow * m_iNumCols + iCol;
			
			m_vIndexList[iCurIndex + 3] = m_vIndexList[iCurIndex + 2];
			m_vIndexList[iCurIndex + 4] = m_vIndexList[iCurIndex + 1];
			m_vIndexList[iCurIndex + 5] = iNextRow * m_iNumCols + iNextCol;

			iCurIndex += 6;
		}
	}

	ComputeVertexNormal();
}
void TMap::ComputeVertexNormal()
{
	CreateFaceNormals();
	SetVertexNormals();
}
void TMap::CreateFaceNormals()
{
	m_FaceInfoList.resize(m_iNumFace);
	m_VertrexInfoList.resize(m_iNumVertices);

	for (int iFace = 0; iFace < m_iNumFace; iFace++)
	{
		UINT i0 = m_vIndexList[iFace * 3 + 0];
		UINT i1 = m_vIndexList[iFace * 3 + 1];
		UINT i2 = m_vIndexList[iFace * 3 + 2];
		T::TVector3 v0 = m_vVertexList[i0].p;
		T::TVector3 v1 = m_vVertexList[i1].p;
		T::TVector3 v2 = m_vVertexList[i2].p;
		T::TVector3 e0 = v1 - v0;
		T::TVector3 e1 = v2 - v0;
		
		D3DXVec3Cross(&m_FaceInfoList[iFace].vNormal, &e0, &e1);
		D3DXVec3Normalize(&m_FaceInfoList[iFace].vNormal,
						  &m_FaceInfoList[iFace].vNormal);

		m_VertrexInfoList[i0].m_FaceIndexList.push_back(iFace);
		m_VertrexInfoList[i1].m_FaceIndexList.push_back(iFace);
		m_VertrexInfoList[i2].m_FaceIndexList.push_back(iFace);
	}
}
void TMap::SetVertexNormals()
{
	for (int v = 0; v < m_vVertexList.size(); v++)
	{
		for (int n = 0; n < m_VertrexInfoList[v].m_FaceIndexList.size(); n++)
		{
			UINT iFaceID = m_VertrexInfoList[v].m_FaceIndexList[n];
			m_vVertexList[v].n += m_FaceInfoList[iFaceID].vNormal;
		}
		m_vVertexList[v].n.Normalize();		

		T::TVector3 vLight = { 100, 100.0f, 0.0f };
		D3DXVec3Normalize(&vLight, &vLight);
		//vLight *= -1.0f;
		float fDot = D3DXVec3Dot(&m_vVertexList[v].n, &vLight);
		m_vVertexList[v].c = { fDot, fDot, fDot, 1.0f };
	}

	m_pContext->UpdateSubresource(m_pVertexBuffer, 0, NULL, &m_vVertexList.at(0), 0, 0);
}