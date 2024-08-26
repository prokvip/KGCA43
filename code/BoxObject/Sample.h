#pragma once
#include "TCore.h"
#include "TPlaneObj.h"

class Sample : public TCore
{
	TPlaneObj	m_obj;
	TPlaneObj	m_objFar;
	FMatrix		m_matView;
	FMatrix		m_matProj;
public:
	void   Init() override;
	void   PreRender()override;
	void   Frame() override;
	void   Render() override;
	void   Release() override;
};