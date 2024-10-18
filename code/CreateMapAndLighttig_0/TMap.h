#pragma once
#include "TDxObj3D.h"

struct TMapDesc
{
	int				iNumCols;
	int				iNumRows;
	float			fCellDistance;
	float			fScaleHeight;
	T_STR			strTextureFile;
	T_STR			strShaderFile;
};

struct TFaceInfo
{
	// 평면노말
	T::TVector3  vNormal;
	// 에지 인덱스
	// 정점 인덱스	
};
struct TVertexInfo
{	
	std::vector< UINT >   m_FaceIndexList;
};
class TMap : public TDxObject3D
{
public:
	std::vector<float>   m_HeightList;
	D3D11_TEXTURE2D_DESC m_HeightMapDesc;
public:
	TMapDesc		m_mapDesc;
	int				m_iNumCols;
	int				m_iNumRows;
	int				m_iNumCellCols;
	int				m_iNumCellRows;
	int				m_iNumVertices;
	int				m_iNumFace;
	float			m_fCellDistance;
	std::vector< TFaceInfo>		m_FaceInfoList;
	std::vector< TVertexInfo >  m_VertrexInfoList;
public:
	bool    Load(TMapDesc& desc);
	bool	CreateMap(TMapDesc& desc);
	virtual void	 SetVertexData() override;
	virtual void	 SetIndexData() override;
public:
	void ComputeVertexNormal();
	void CreateFaceNormals();
	void SetVertexNormals();
};

