#include "pch.h"
#include "TCharacterObj.h"
void       TMapObject::SetAnimFrame(UINT s, UINT e)
{
	if (m_fStartFrame != s && m_fLastFrame != e)
	{
		m_fStartFrame = s;
		m_fLastFrame = e;
		m_fFrameAnimation = s;
	}
}

void       TMapObject::Render(TMap& map, TCamera& cam)
{
	T::TMatrix matScale, matBaseRotate, matRotate, matTrans;
	//D3DXMatrixScaling(&matScale,vScale.x,	vScale.y,	vScale.z);		
	//D3DXMatrixRotationY(&matBaseRotate, DegreeToRadian(180.0f));
	//D3DXMatrixRotationY(&matRotate, g_fGameTime * 0.5f);
	//D3DXVec3TransformCoord(&vPos, &vPos, &matRotate);

	vLook.x = -matWorld._31;
	vLook.y = -matWorld._32;
	vLook.z = -matWorld._33;
	D3DXVec3Normalize(&vLook, &vLook);

	// += vLook * g_fSecondPerFrame * 100.0f;
	vPos.y = map.GetHeight(vPos);
	D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, vPos.z);
	matWorld = matScale * matBaseRotate * matRotate * matTrans;

	float FrameSpeed = 30.0f;
	m_fFrameAnimation += g_fSecondPerFrame * FrameSpeed * 1.0f;
	if (m_fFrameAnimation >= m_fLastFrame-1)
	{
		m_fFrameAnimation = m_fStartFrame;
	}

	if (m_pMesh)
	{
		auto pMesh = m_pMesh->Get();
		pMesh->SetAnimFrame(m_fStartFrame, m_fLastFrame);
		TFbxModel* pAnim = m_pAnimMatrix->Get().get();
		if (pAnim != nullptr)
		{
			pMesh->AnimFrame(m_fFrameAnimation, pAnim);
		}
		pMesh->m_matParentWorld = matWorld;
		pMesh->SetMatrix(nullptr, &cam.m_matView, &cam.m_matProj);
		pMesh->Render(TDevice::m_pContext);
	}
}