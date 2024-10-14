#include "Sample.h"
void   Sample::Init()
{
	TMapDesc desc = { 65,65, 1.0f, 1.0f,
		L"../../data/map/001.jpg", 
		L"../../data/shader/pnct.hlsl"};

	if (!m_Map.Load(desc))
	{
		return;
	}

	T::TVector3 eye = { 0.0f, 100.0f, -100.0f };
	T::TVector3 target = { 0.0f, 0.0f, 0.0f };
	T::TVector3 up = { 0.0f, 1.0f, 0.0f };
	// 이항 '=': 오른쪽 피연산자로 'T_Math::FMatrix' 형식을 사용하는 연산자가 없거나 허용되는 변환이 없습니다.
	m_MainCamera.SetView(eye, target, up);
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
