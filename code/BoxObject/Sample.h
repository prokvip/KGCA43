#pragma once
#include "TCore.h"
#include "TPlaneObj.h"
#include "TBoxObj.h"
class Sample : public TCore
{
	TPlaneObj	m_Plane;
	TBoxObj		m_Box;
	FMatrix		m_matView;
	FMatrix		m_matProj;
public:
	void   Init() override;
	void   PreRender()override;
	void   Frame() override;
	void   Render() override;
	void   Release() override;
};