#include "TCore.h"
#include "TDxObject.h"
//../../dxtk/include  �⺻��η� �����Ѵ�. (�Ӽ�)
#include "WicTextureLoader.h"
#ifdef _DEBUG
	#pragma comment(lib, "DirectXTK_D.lib")
#else
	#pragma comment(lib, "DirectXTK_R.lib")
#endif
//#include "../../dxtk/include/WicTextureLoader.h"
class Sample: public TCore{

	TDxObject objScreen; // �ﰢ��
	TDxObject objNDC;
	// ������ ���̺����(�Ƚ����̴�)�� ���� ������
	ID3D11ShaderResourceView*	m_pSRV = nullptr;
	// �ؽ�ó(�̹���) �ε� ������
	ID3D11Resource*				m_pTexture = nullptr;
public:
	void   Init() override
	{		
		HRESULT hr = 
			DirectX::CreateWICTextureFromFile(
				m_pd3dDevice,
				L"bk.jpg",
				&m_pTexture,
				&m_pSRV);
		// ȭ����ǥ��
		// 0,0[0,0]    -     800,0[1,0]

		//  |     X      |

		// 0,600[0,1]  -     800,600[1,1]        
		
		objScreen.m_vList.resize(6);
		// �ð�������� ����Ǿ�� �Ѵ�.
		objScreen.m_vList[0].p = T_Math::FVector2(m_rtClient.left, m_rtClient.top);
		objScreen.m_vList[1].p = T_Math::FVector2(m_rtClient.right/2, m_rtClient.top);
		objScreen.m_vList[2].p = T_Math::FVector2(m_rtClient.right/2, m_rtClient.bottom/2);
		objScreen.m_vList[0].c = T_Math::FVector4(1,0,0,1);
		objScreen.m_vList[1].c = T_Math::FVector4(0,1,0,1);
		objScreen.m_vList[2].c = T_Math::FVector4(0,0,1,1);
		objScreen.m_vList[0].t = T_Math::FVector2(0, 0);
		objScreen.m_vList[1].t = T_Math::FVector2(1, 0);
		objScreen.m_vList[2].t = T_Math::FVector2(1, 1);


		objScreen.m_vList[3].p = T_Math::FVector2(m_rtClient.right/2, m_rtClient.bottom/2);
		objScreen.m_vList[4].p = T_Math::FVector2(m_rtClient.left, m_rtClient.bottom/2);
		objScreen.m_vList[5].p = T_Math::FVector2(m_rtClient.left, m_rtClient.top);
		objScreen.m_vList[3].c = T_Math::FVector4(1, 0, 0, 1);
		objScreen.m_vList[4].c = T_Math::FVector4(0, 1, 0, 1);
		objScreen.m_vList[5].c = T_Math::FVector4(0, 0, 1, 1);
		objScreen.m_vList[3].t = T_Math::FVector2(1, 1);
		objScreen.m_vList[4].t = T_Math::FVector2(0, 1);
		objScreen.m_vList[5].t = T_Math::FVector2(0, 0);
		

		// ȭ����ǥ�踦  NDC��ǥ �����Ѵ�.

		for (auto& V : objScreen.m_vList)
		{
			// ȭ����ǥ��
			T_Math::FVector2  v = V.p;
			// 0~ 800 -> 0 ~ 1
			v.X = v.X / m_rtClient.right;
			v.Y = v.Y / m_rtClient.bottom;
			//NDC ��ǥ��
			// 0 ~ 1  -> -1 ~ +1
			V.p.X = v.X * 2.0f - 1.0f;
			V.p.Y = -(v.Y * 2.0f - 1.0f);

			int k = 0;
			// -1 ~ 1  -> 0 ~ +1
			/*v.X = v.X * 0.5f + 0.5f;
			v.Y = v.Y * 0.5f + 0.5f;*/
		}



		if (objScreen.CreateVertexBuffer(m_pd3dDevice) == false)
		{
			objScreen.Release();
			return ;
		}
		if (objScreen.LoadShader(m_pd3dDevice)==false)
		{
			objScreen.Release();
			return ;
		}
		if (objScreen.CreateInputLayout(m_pd3dDevice) == false)
		{
			objScreen.Release();
			return;
		}

		// ����ȭ�� ��ġ ��ǥ��(NDC) : (������ǥ�� ���)
		// *� ��ǥ�踦 ����ϴ��� ������� 
		// ���������δ� NDC ������ �Ǿ�� ȭ�鿡 �������ȴ�.
		// 
		// -1 ~ +1 ������ �����ϴ� ��ǥ��
		// -1,+1   -         +1,+1
		//    -0.5      +0.5
		//         0,0

		// -1,-1    -        +1,-1     
		/*
		objNDC.v[0] = T_Math::FVector2(-1.0f, +1.0f);
		objNDC.v[1] = T_Math::FVector2( 1.0f, +1.0f);
		objNDC.v[2] = T_Math::FVector2( 1.0f, -1.0f);
		objNDC.v[3] = T_Math::FVector2(-1.0f, -1.0f);*/
		/*T_Math::FVector2 v1(1,1);
		T_Math::FVector2 v2 = v1;
		T_Math::FVector2 v3 = v1 * 10.0f;
		T_Math::FVector2 v4;
		float fLength1 = v4.Length();
		bool  bNorm = v4.IsZero();
		float fLength2= v1.Length();
		float fLength3 = v3.Length();
		T_Math::FVector2 vNormalize = v1.Normalize();
		float fLength4 = vNormalize.Length();*/
		
		int k = 0;
	}
	void   Render() override
	{ 
		// 0�� �������� �ؽ�ó ����
		m_pContext->PSSetShaderResources(   0, 1, &m_pSRV);
		//Texture2D g_txTexture : register(t0);
		objScreen.Render(m_pContext);
	}
	void   Release() override
	{
		if (m_pSRV)
		{
			m_pSRV->Release();
			m_pSRV = nullptr;
		}
		if (m_pTexture)
		{
			m_pTexture->Release();
			m_pTexture = nullptr;
		}
		objScreen.Release();
	}

};
T_GAME_START(800, 600);