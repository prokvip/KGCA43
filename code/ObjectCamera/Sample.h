#pragma once
#include "TCore.h"
#include "THero.h"
#include "TNpc.h"
class TCamera
{
public:
	T_Math::FMatrix     m_matCamera;
	T_Math::FVector2    m_vCameraPos;
	void Frame()
	{
		m_matCamera.Translation(m_vCameraPos);
	}
	T_Math::FMatrix&     GetMatrix()
	{
		return m_matCamera;
	}
	void  SetTransform(T_Math::FVector2   pos)
	{
		m_matCamera.Translation(pos);
	}
	void Up()
	{
		T_Math::FVector2 vCamDirection = { 0.0f, 1.0f };
		m_vCameraPos = m_vCameraPos + vCamDirection * 100.0f * g_fSecondPerFrame;
	}
	void Down()
	{
		T_Math::FVector2 vCamDirection = { 0.0f, -1.0f };
		m_vCameraPos = m_vCameraPos + vCamDirection * 100.0f * g_fSecondPerFrame;
	}
	void Right(float fValue)
	{
		m_vCameraPos.X = m_vCameraPos.X + fValue;
	}
	void Left(float fValue)
	{
		m_vCameraPos.X = m_vCameraPos.X + fValue;
	}
};
class Sample : public TCore
{
	TCamera				m_Cam;
	TActor				objScreen; // »ï°¢Çü
	std::vector<TActor> m_UIList;
	std::vector<TNpc>	m_npcList;
	THero hero;	
	UINT  m_iNpcCounter=0;
	std::vector<ComPtr<ID3D11ShaderResourceView>> m_pNumber;
public:
	void   Init() override;
	void   Frame() override;
	void   Render() override;
	void   Release() override;
};