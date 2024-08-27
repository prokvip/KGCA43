#pragma once
#include "TCore.h"
#include "TPlaneObj.h"
#include "TBoxObj.h"
class TMapObj
{
public:
	FMatrix		m_matWorld;
	TShapeObj*  m_pBaseShape = nullptr;
};
class Sample : public TCore
{
	TBoxObj    m_Box;
	TPlaneObj  m_Plane;

	std::vector<std::shared_ptr<TMapObj>>	m_ShapeList;
	FMatrix		m_matView;
	FMatrix		m_matProj;
public:
	void   Init() override;
	void   PreRender()override;
	void   Frame() override;
	void   Render() override;
	void   Release() override;
};