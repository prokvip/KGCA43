#include "Sample.h"
void   Sample::Init()
{
	TMapDesc desc = { 5,5, 1.0f, 1.0f,
		L"../../data/20200428_185613.jpg", 
		L"../../data/shader/pnct.hlsl"};

	if (!m_Map.Load(desc))
	{
		return;
	}
}
void    Sample::Frame()
{
}
void    Sample::Render()
{
	m_Map.SetMatrix(nullptr, &m_MainCamera.m_matView, &m_matProj);
	m_Map.Render(TDevice::m_pContext);
}
void    Sample::Release()
{

}

T_GAME_START(800, 600);
