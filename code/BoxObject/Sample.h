#pragma once
#include "TCore.h"
#include "TPlaneObj.h"
#include "TBoxObj.h"
#include "TLineObj.h"
#include "TCamera.h"
class TMapObj
{
public:
	FMatrix		m_matWorld;
	TShapeObj*  m_pBaseShape = nullptr;
};
class Sample : public TCore
{
	float m_fPitch = 0.0f;
	float m_fYaw = 0.0f;
	float m_fRoll = 0.0f;
	float m_fDistance = 0.0f;
	TBoxObj    m_Box;
	TPlaneObj  m_Plane;

	std::vector<std::shared_ptr<TMapObj>>	m_ShapeList;
	TCamera		m_MainCamera;
	FMatrix		m_matProj;

	TLineObj    m_Line;
public:
	void   Init() override;
	void   PreRender()override;
	void   Frame() override;
	void   Render() override;
	void   Release() override;
	void   Reset() override;	
};