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
			m_vVertexList[iIndex].p.X = (iCol-fHalfCol) * m_fCellDistance;			
			m_vVertexList[iIndex].p.Z = -((iRow- fHalfRow) * m_fCellDistance);
			m_vVertexList[iIndex].p.Y = m_HeightList[iIndex]* m_mapDesc.fScaleHeight;

			m_vVertexList[iIndex].t.X = fTexOffsetU * iCol;
			m_vVertexList[iIndex].t.Y = fTexOffsetV * iRow;

			m_vVertexList[iIndex].n = FVector3(0,1,0);
			m_vVertexList[iIndex].c = FVector4(1, 1, 1,1);			
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
}