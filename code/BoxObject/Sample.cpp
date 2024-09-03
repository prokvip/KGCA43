#include "Sample.h"
#include "TInput.h"

void   Sample::Init()
{	
	m_Plane.Create(L"../../data/1234.jpg", L"pnct.hlsl");
	m_Box.Create(L"../../data/obj.jpg", L"pnct.hlsl");	
	m_Line.Create(L"../../data/obj.jpg", L"line.hlsl");

	m_Plane.m_vPos = { 0.0f,0.0f, 0.0f };
	m_Box.m_vPos = { 0.0f,0.0f, 0.0f };
	m_Plane.m_vScale = { 1.0f,1.0f,1.0f };
	m_Box.m_vScale = { 1.0f,1.0f, 1.0f };


	for (int iObj = 0; iObj < 10; iObj++)
	{
		T::TMatrix matScale, matRotate, matTrans;
		
		T::D3DXMatrixScaling(&matScale, randstep(0.5f, 2.0f), randstep(0.5f, 2.0f), randstep(0.5f, 2.0f));
		T::D3DXMatrixRotationY(&matRotate, randstep(0.0f, TBASIS_PI * 2.0f));
		T::D3DXMatrixTranslation(&matTrans, -15.0f + iObj * 3.0f, 0, 0);
		/*matScale.Scale(randstep(0.5f, 2.0f), randstep(0.5f, 2.0f), randstep(0.5f, 2.0f));
		matRotate.rotateY(randstep(0.0f, TBASIS_PI* 2.0f));
		matTrans.Translation(-15.0f + iObj * 3.0f, 0, 0);*/
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


	T::TVector3 eye = { 0.0f, 0.0f, -25.0f };
	T::TVector3 target = { 0.0f, 0.0f, 0.0f };
	T::TVector3 up = { 0.0f, 1.0f, 0.0f };
	// 이항 '=': 오른쪽 피연산자로 'T_Math::FMatrix' 형식을 사용하는 연산자가 없거나 허용되는 변환이 없습니다.
	m_MainCamera.SetView(eye, target, up);
	
	Reset();
	
}
void  Sample::Reset()
{
	T::D3DXMatrixPerspectiveFovLH(&m_matProj, TBASIS_PI * 0.25f,
		(float)g_xClientSize / (float)g_yClientSize, 1.0f, 100.0f);
	/*m_matProj = FMatrix::CreateProjTransform(1.0f, 100.0f,
		TBASIS_PI * 0.25f, (float)g_xClientSize / (float)g_yClientSize);*/
}
void  Sample::PreRender()
{
}
void    Sample::Frame()
{	
	if (TInput::Get().KeyCheck('W'))
	{
		m_MainCamera.MoveLook(g_fSecondPerFrame* 50.0f);
	}
	if (TInput::Get().KeyCheck('S'))
	{
		m_MainCamera.MoveLook(-g_fSecondPerFrame * 50.0f);
	}
	if (TInput::Get().KeyCheck('A'))
	{
		m_MainCamera.MoveSide(-g_fSecondPerFrame * 50.0f);
	}
	if (TInput::Get().KeyCheck('D'))
	{
		m_MainCamera.MoveSide(g_fSecondPerFrame * 50.0f);
	}

	if (TInput::Get().KeyCheck('Q'))
	{
		m_MainCamera.MoveUp(g_fSecondPerFrame * 50.0f);
	}
	if (TInput::Get().KeyCheck('E'))
	{
		m_MainCamera.MoveUp(-g_fSecondPerFrame * 50.0f);
	}
	m_MainCamera.Update(
		TInput::Get().GetDeltaY(),
		TInput::Get().GetDeltaX(), 
		 0.0f, m_fDistance);
	m_Plane.Frame();
	m_Box.Frame();
}
void    Sample::Render()
{	
	//m_Plane.SetMatrix(nullptr, &m_MainCamera.m_matView, &m_matProj);
	//m_Plane.Render(TDevice::m_pContext);

	//m_Box.SetMatrix(nullptr, &m_MainCamera.m_matView, &m_matProj);
	//m_Box.Render(TDevice::m_pContext);

	for (int iObj = 0; iObj < m_ShapeList.size(); iObj++)
	{
		T::TMatrix matWorld = m_ShapeList[iObj]->m_matWorld;
		//인수 1을(를) 'T_Math::FMatrix *'에서 'T::TMatrix *'(으)로 변환할 수 없습니다.
		m_ShapeList[iObj]->m_pBaseShape->SetMatrix(&matWorld, 
			&m_MainCamera.m_matView, &m_matProj);
		m_ShapeList[iObj]->m_pBaseShape->Render(TDevice::m_pContext);
	}

	m_Line.SetMatrix(nullptr, &m_MainCamera.m_matView, &m_matProj);
	m_Line.Draw(FVector3(0.0f, 0.0f, 0.0f), 
				FVector3(10000.0f, 0.0f, 0.0f),
				FVector4(1.0f, 0.0f, 0.0f, 1.0f));
	m_Line.Draw(FVector3(0.0f, 0.0f, 0.0f),
		FVector3(0.0f, 10000.0f, 0.0f),
		FVector4(0.0f, 1.0f, 0.0f, 1.0f));
	m_Line.Draw(FVector3(0.0f, 0.0f, 0.0f),
		FVector3(0.0f, 0.0f, 10000.0f),
		FVector4(0.0f, 0.0f, 1.0f, 1.0f));
}
void    Sample::Release()
{
	m_Plane.Release();
	m_Box.Release();
	m_Line.Release();
}

T_GAME_START(800, 600);
