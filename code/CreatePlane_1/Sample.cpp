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
		// 0,0    -     800,0

		//  |     X      |

		// 0,600  -     800,600        
		
		objScreen.m_vList.resize(6);
		// �ð�������� ����Ǿ�� �Ѵ�.
		objScreen.m_vList[0].v = T_Math::FVector2(-1.0f, +1.0f);
		objScreen.m_vList[1].v = T_Math::FVector2(1.0f, +1.0f);
		objScreen.m_vList[2].v = T_Math::FVector2(1.0f, -1.0f);
		objScreen.m_vList[0].c = T_Math::FVector4(1,0,0,1);
		objScreen.m_vList[1].c = T_Math::FVector4(0,1,0,1);
		objScreen.m_vList[2].c = T_Math::FVector4(0,0,1,1);

		objScreen.m_vList[3].v = T_Math::FVector2(+1.0f, -1.0f);
		objScreen.m_vList[4].v = T_Math::FVector2(-1.0f, -1.0f);
		objScreen.m_vList[5].v = T_Math::FVector2(-1.0f, +1.0f);
		objScreen.m_vList[3].c = T_Math::FVector4(1, 0, 0, 1);
		objScreen.m_vList[4].c = T_Math::FVector4(0, 1, 0, 1);
		objScreen.m_vList[5].c = T_Math::FVector4(0, 0, 1, 1);
		//objScreen.m_vList[0].v = T_Math::FVector2(0.0f, 0.0f);
		//objScreen.m_vList[1].v = T_Math::FVector2(800.0f, 0.0f);
		//objScreen.m_vList[2].v = T_Math::FVector2(800.0f, 600.0f);
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