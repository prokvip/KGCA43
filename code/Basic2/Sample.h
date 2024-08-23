#pragma once
#include "TCore.h"
#include "TDxObject.h"
using namespace T_Math;

struct cb // �������
{
	FMatrix g_matWorld;
	FMatrix g_matView;
	FMatrix g_matProj;
};
class TObj3D : public TDxObject3D
{
	ComPtr<ID3D11Buffer>   m_pConstantBuffer;
	cb					   m_cb;
public:
	virtual bool     CreateConstantBuffer(ID3D11Device* pd3dDevice);
	virtual void	 SetVertexData()
	{
		//local
		m_vList.resize(6);
		// 0,5    1

		// 4     2,3
		m_vList[0].p = T_Math::FVector3(-1.0f, 1.0f, 0.0f);
		m_vList[0].n = T_Math::FVector3(0.0f, 0.0f, 0.0f);
		m_vList[0].c = T_Math::FVector4(1.0f, 1.0f, 1.0f, 1.0f);
		m_vList[0].t = T_Math::FVector2(0.0f, 0.0f);

		m_vList[1].p = T_Math::FVector3(1.0f, 1.0f, 0.0f);
		m_vList[1].n = T_Math::FVector3(0.0f, 0.0f, 0.0f);
		m_vList[1].c = T_Math::FVector4(1.0f, 1.0f, 1.0f, 1.0f);
		m_vList[1].t = T_Math::FVector2(1.0f, 0.0f);

		m_vList[2].p = T_Math::FVector3(1.0f, -1.0f, 0.0f);
		m_vList[2].n = T_Math::FVector3(0.0f, 0.0f, 0.0f);
		m_vList[2].c = T_Math::FVector4(1.0f, 1.0f, 1.0f, 1.0f);
		m_vList[2].t = T_Math::FVector2(1.0f, 1.0f);


		m_vList[3].p = T_Math::FVector3(1.0f, -1.0f, 0.0f);
		m_vList[3].n = T_Math::FVector3(0.0f, 0.0f, 0.0f);
		m_vList[3].c = T_Math::FVector4(1.0f, 1.0f, 1.0f, 1.0f);
		m_vList[3].t = T_Math::FVector2(1.0f, 1.0f);

		m_vList[4].p = T_Math::FVector3(-1.0f, -1.0f, 0.0f);
		m_vList[4].n = T_Math::FVector3(0.0f, 0.0f, 0.0f);
		m_vList[4].c = T_Math::FVector4(1.0f, 1.0f, 1.0f, 1.0f);
		m_vList[4].t = T_Math::FVector2(0.0f, 1.0f);

		m_vList[5].p = T_Math::FVector3(-1.0f, 1.0f, 0.0f);
		m_vList[5].n = T_Math::FVector3(0.0f, 0.0f, 0.0f);
		m_vList[5].c = T_Math::FVector4(1.0f, 1.0f, 1.0f, 1.0f);
		m_vList[5].t = T_Math::FVector2(0.0f, 0.0f);

	}
	bool   Create(std::wstring texName, std::wstring hlsl)
	{
		m_pd3dDevice = TDevice::m_pd3dDevice.Get();
		m_pContext = TDevice::m_pContext;

		m_szShaderFilename = hlsl;

		m_pTexture = I_Tex.Load(texName).get();
		if (m_pTexture != nullptr)
		{
			m_pSRV = m_pTexture->m_pSRV;
		}

		SetVertexData();

		if (CreateVertexBuffer(m_pd3dDevice) == false)
		{
			Release();
			return false;
		}
		if (LoadShader(m_szShaderFilename) == false)
		{
			Release();
			return false;
		}
		if (CreateInputLayout(m_pd3dDevice) == false)
		{
			Release();
			return false;
		}
		m_cb.g_matWorld.Translation(0.1f, 0.0f, 0);
		m_cb.g_matWorld.Transpose();
		if (CreateConstantBuffer(m_pd3dDevice) == false)
		{
			Release();
			return false;
		}		
		return true;
	}
	virtual void     PreRender(ID3D11DeviceContext* pContext)
	{
		TDxObject3D::PreRender(pContext);
		pContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
	}
};
class Sample : public TCore
{
	TObj3D		m_obj;
public:
	void   Init() override;
	void   Frame() override;
	void   Render() override;
	void   Release() override;
};