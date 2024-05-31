#include "TCore.h"
#include "TDxObject.h"
class Sample: public TCore{

	TDxObject objScreen; // �ﰢ��
	TDxObject objNDC;
public:
	void   Init() override
	{
		// ȭ����ǥ��
		// 0,0    -     800,0

		//  |     X      |

		// 0,600  -     800,600        
		
		objScreen.m_vList.resize(6);
		// �ð�������� ����Ǿ�� �Ѵ�.
		objScreen.m_vList[0].v = T_Math::FVector2(-1.0f, +1.0f);
		objScreen.m_vList[1].v = T_Math::FVector2(1.0f, +1.0f);
		objScreen.m_vList[2].v = T_Math::FVector2(1.0f, -1.0f);

		objScreen.m_vList[3].v = T_Math::FVector2(-1.0f, +1.0f);
		objScreen.m_vList[4].v = T_Math::FVector2(1.0f, -1.0f);
		objScreen.m_vList[5].v = T_Math::FVector2(-1.0f, -1.0f);

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
		m_ViewPort.TopLeftX = 0;
		m_ViewPort.TopLeftY = 0;
		m_ViewPort.Width = m_xWindowSize / 2;
		m_ViewPort.Height = m_yWindowSize / 2;
		m_pContext->RSSetViewports(1, &m_ViewPort);
		objScreen.Render(m_pContext);

		m_ViewPort.TopLeftX = m_xWindowSize / 2;
		m_ViewPort.TopLeftY = 0;
		m_pContext->RSSetViewports(1, &m_ViewPort);
		m_pContext->Draw(objScreen.m_vList.size(), 0);

		m_ViewPort.TopLeftX = 0;
		m_ViewPort.TopLeftY = m_yWindowSize / 2;
		m_pContext->RSSetViewports(1, &m_ViewPort);
		m_pContext->Draw(objScreen.m_vList.size(), 0);

		/*m_ViewPort.TopLeftX = _xWindowSize / 2;
		m_ViewPort.TopLeftY = m_yWindowSize / 2;
		m_pContext->RSSetViewports(1, &m_ViewPort);
		m_pContext->Draw(objScreen.m_vList.size(), 0);*/

	}
	void   Release() override
	{
		objScreen.Release();
	}

};
T_GAME_START(800, 600);