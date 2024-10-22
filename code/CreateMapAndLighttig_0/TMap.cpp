#include "TMap.h"
float TMap::GetHeightmap(int row, int col)
{
	return m_vVertexList[row * m_iNumRows + col].p.y;
}
	//T::TVector3 pos
float TMap::GetHeight(T::TVector3 vPos)
{
	float fPosX = vPos.x;
	float fPosZ = vPos.z;
	// fPosX/fPosZ의 위치에 해당하는 높이맵셀을 찾는다.
	// m_iNumCols와m_iNumRows은 가로/세로의 실제 크기값임.
	float fCellX = (float)(m_iNumCellCols * m_fCellDistance / 2.0f + fPosX);
	float fCellZ = (float)(m_iNumCellRows * m_fCellDistance / 2.0f - fPosZ);

	// 셀의 크기로 나누어 0~1 단위의 값으로 바꾸어 높이맵 배열에 접근한다.
	fCellX /= (float)m_fCellDistance;
	fCellZ /= (float)m_fCellDistance;

	// fCellX, fCellZ 값보다 작거나 같은 최대 정수( 소수부분을 잘라낸다.)
	float fVertexCol = ::floorf(fCellX);
	float fVertexRow = ::floorf(fCellZ);

	// 높이맵 범위를 벗어나면 강제로 초기화 한다.
	if (fVertexCol < 0.0f)  fVertexCol = 0.0f;
	if (fVertexRow < 0.0f)  fVertexRow = 0.0f;
	if ((float)(m_iNumCols - 2) < fVertexCol)	fVertexCol = (float)(m_iNumCols - 2);
	if ((float)(m_iNumRows - 2) < fVertexRow)	fVertexRow = (float)(m_iNumRows - 2);

	// 계산된 셀의 플랜을 구성하는 4개 정점의 높이값을 찾는다. 
	//  A   B
	//  *---*
	//  | / |
	//  *---*  
	//  C   D
	float A = GetHeightmap((int)fVertexRow, (int)fVertexCol);
	float B = GetHeightmap((int)fVertexRow, (int)fVertexCol + 1);
	float C = GetHeightmap((int)fVertexRow + 1, (int)fVertexCol);
	float D = GetHeightmap((int)fVertexRow + 1, (int)fVertexCol + 1);

	// A정점의 위치에서 떨어진 값(변위값)을 계산한다. 0 ~ 1.0f
	float fDeltaX = fCellX - fVertexCol;
	float fDeltaZ = fCellZ - fVertexRow;
	// 보간작업를 위한 기준 페잇스를 찾는다. 
	float fHeight = 0.0f;
	// 윗페이스를 기준으로 보간한다.
	// fDeltaZ + fDeltaX < 1.0f
	if (fDeltaZ < (1.0f - fDeltaX))  //ABC
	{
		float uy = B - A; // A->B
		float vy = C - A; // A->C	
						  // 두 정점의 높이값의 차이를 비교하여 델타X의 값에 따라 보간값을 찾는다.		
		fHeight = A + Lerp(0.0f, uy, fDeltaX) + Lerp(0.0f, vy, fDeltaZ);
	}
	// 아래페이스를 기준으로 보간한다.
	else // DCB
	{
		float uy = C - D; // D->C
		float vy = B - D; // D->B
						  // 두 정점의 높이값의 차이를 비교하여 델타Z의 값에 따라 보간값을 찾는다.		
		fHeight = D + Lerp(0.0f, uy, 1.0f - fDeltaX) + Lerp(0.0f, vy, 1.0f - fDeltaZ);
	}
	return fHeight;
}
float TMap::Lerp(float fStart, float fEnd, float fTangent)
{
	return fStart - (fStart * fTangent) + (fEnd * fTangent);
}

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