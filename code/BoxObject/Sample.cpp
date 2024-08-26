#include "Sample.h"


void   Sample::Init()
{
	this->m_obj.Create(L"../../data/1234.jpg", L"pnct.hlsl");
	this->m_objFar.Create(L"../../data/obj.jpg", L"pnct.hlsl");	

	m_obj.m_vPos = { 0.0f,0.0f, 0.0f };
	m_objFar.m_vPos = { 5.0f,0.0f, 5.0f };

	m_obj.m_vScale = { 3.0f,3.0f,3.0f };
	m_objFar.m_vScale = { 5.0f,5.0f, 5.0f };

	FVector3 eye = { 0.0f, 0.0f, -15.0f };
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
	this->m_obj.Frame();
	this->m_objFar.Frame();
}
void    Sample::Render()
{	
	this->m_obj.SetMatrix(nullptr, &m_matView, &m_matProj);
	this->m_obj.Render(TDevice::m_pContext);

	this->m_objFar.SetMatrix(nullptr, &m_matView, &m_matProj);
	this->m_objFar.Render(TDevice::m_pContext);
}
void    Sample::Release()
{
	this->m_obj.Release();
	this->m_objFar.Release();
}

T_GAME_START(800, 600);
