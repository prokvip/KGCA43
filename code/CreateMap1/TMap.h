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
public:
	bool    Load(TMapDesc& desc);
	bool	CreateMap(TMapDesc& desc);
	virtual void	 SetVertexData() override;
	virtual void	 SetIndexData() override;
};

