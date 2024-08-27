#include "Sample.h"
#include "TMath.h"

void   Sample::Init()
{
	m_Plane.Create(L"../../data/1234.jpg", L"pnct.hlsl");
	m_Box.Create(L"../../data/obj.jpg", L"pnct.hlsl");	

	m_Plane.m_vPos = { 0.0f,0.0f, 0.0f };
	m_Box.m_vPos = { 0.0f,0.0f, 0.0f };
	m_Plane.m_vScale = { 1.0f,1.0f,1.0f };
	m_Box.m_vScale = { 1.0f,1.0f, 1.0f };


	for (int iObj = 0; iObj < 10; iObj++)
	{
		FMatrix matScale, matRotate, matTrans;
		matScale.Scale(randstep(0.5f, 2.0f), randstep(0.5f, 2.0f), randstep(0.5f, 2.0f));
		matRotate.rotateY(randstep(0.0f, TBASIS_PI* 2.0f));
		matTrans.Translation(-15.0f + iObj * 3.0f, 0, 0);
		auto pObj = std::make_shared<TMapObj>();
		pObj->m_matWorld = matScale * matRotate * matTrans;
		if (iObj % 2 == 0)
		{
			pObj->m_matWorld = matScale * matTrans;
			pObj->m_pBaseShape = &m_Plane;
		}
		else
		{
			pObj->m_matWorld = matScale * matRotate * matTrans;
			pObj->m_pBaseShape = &m_Box;
		}
		m_ShapeList.push_back(pObj);
	}


	FVector3 eye = { 0.0f, 20.0f, -15.0f };
	FVector3 target = { 0.0f, 0.0f, 0.0f };
	FVector3 up = { 0.0f, 1.0f, 0.0f };
	m_matView = FMatrix::CreateViewTransform(eye, target, up);
	m_matProj = FMatrix::CreateProjTransform(1.0f, 100.0f,
		TBASIS_PI * 0.25f, (float)g_xClientSize / (float)g_yClientSize);
	
}
void  Sample::PreRender()
{
}
void    Sample::Frame()
{
	m_Plane.Frame();
	m_Box.Frame();
}
void    Sample::Render()
{	
	//m_Plane.SetMatrix(nullptr, &m_matView, &m_matProj);
	//m_Plane.Render(TDevice::m_pContext);

	//m_Box.SetMatrix(nullptr, &m_matView, &m_matProj);
	//m_Box.Render(TDevice::m_pContext);

	for (int iObj = 0; iObj < m_ShapeList.size(); iObj++)
	{
		FMatrix matWorld = m_ShapeList[iObj]->m_matWorld;
		m_ShapeList[iObj]->m_pBaseShape->SetMatrix(&matWorld, &m_matView, &m_matProj);
		m_ShapeList[iObj]->m_pBaseShape->Render(TDevice::m_pContext);
	}
}
void    Sample::Release()
{
	m_Plane.Release();
	m_Box.Release();
}

T_GAME_START(800, 600);
