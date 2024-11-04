#include "pch.h"
#include "Sample.h"
#include "TTexMgr.h"
void   Sample::SetObject(T::TVector3 vPos)
{
	TMapObject obj;
	int iNumObject = I_Object.m_list.size();
	obj.iObjectType = rand() % m_pFbxfileList.size();
	obj.vPos = vPos;/*{ randstep(-m_Map.m_fMapDistance.x / 2.0f, +m_Map.m_fMapDistance.x / 2.0f), 0.0f,
		randstep(-m_Map.m_fMapDistance.y / 2.0f, +m_Map.m_fMapDistance.y / 2.0f) };*/
	obj.vScale = { randstep(0.1f, 2.0f), randstep(0.1f, 2.0f), randstep(0.1f, 2.0f) };
	D3DXMatrixScaling(&obj.matWorld, obj.vScale.x, obj.vScale.y, obj.vScale.z);
	obj.matWorld._41 = obj.vPos.x;
	obj.matWorld._42 = obj.vPos.y;
	obj.matWorld._43 = obj.vPos.z;
	m_pMapObjectList.emplace_back(obj);
	
}
void   Sample::Init()
{
	m_LightInfo.m_vLightDir = T::TVector4(0, -1, 0, 1);
	m_pConstantBufferLight.Attach(
		TDxObject::CreateConstantBuffer(TDevice::m_pd3dDevice.Get(),
		&m_LightInfo, 1, sizeof(TLightInfo)));
	
	m_Map.LoadHeightMapTexture(
		//L"../../data/map/heightmap/HEIGHT_CASTLE.bmp",
		L"../../data/map/heightmap/heightMap513.bmp");

	TMapDesc desc = { m_Map.m_HeightMapDesc.Width,
					m_Map.m_HeightMapDesc.Height, 10.0f, 2.0f,
		//TMapDesc desc = { 5,
		//			5, 10.0f, 2.0f,
		//L"../../data/map/heightmap/castle.jpg",
		L"../../data/map/001.jpg",		
		L"Lightting.hlsl"};

	if (!m_Map.Load(desc))
	{
		return;
	}

	//m_LoadFiles.push_back(L"../../data/kgc/box.fbx");
	m_LoadFiles.push_back(L"../../data/kgc/SM_Rock.kgc");
	m_LoadFiles.push_back(L"../../data/kgc/SM_Barrel.kgc");
	m_LoadFiles.push_back(L"../../data/kgc/Man.kgc");
	for (int iObj = 0; iObj < m_LoadFiles.size(); iObj++)
	{
		auto tObject = I_Object.Load(m_LoadFiles[iObj]);
		m_pFbxfileList.emplace_back(tObject);
	}

	/*for (int iObj = 0; iObj < 10; iObj++)
	{
		TMapObject obj;
		int iNumObject = I_Object.m_list.size();
		obj.iObjectType = rand() % m_pFbxfileList.size();
		obj.vPos = { randstep(-m_Map.m_fMapDistance.x/2.0f, +m_Map.m_fMapDistance.x/2.0f), 0.0f,
			randstep(-m_Map.m_fMapDistance.y/2.0f, +m_Map.m_fMapDistance.y/2.0f) };
		obj.vScale = { randstep(0.1f, 2.0f), randstep(0.1f, 2.0f), randstep(0.1f, 2.0f) };
		D3DXMatrixScaling(&obj.matWorld, obj.vScale.x, obj.vScale.y, obj.vScale.z);
		obj.matWorld._41 = obj.vPos.x;
		obj.matWorld._42 = m_Map.GetHeight(obj.vPos);
		obj.matWorld._43 = obj.vPos.z;
		m_pMapObjectList.emplace_back(obj);
	}*/

	m_Quadtree.m_pMap = &m_Map;
	m_Quadtree.Init();
	m_Quadtree.BuildTree(m_Quadtree.m_pRootNode);

	T::TVector3 eye = { 0.0f, 500.0f, -500.0f };
	T::TVector3 target = { 0.0f, 0.0f, 0.0f };
	T::TVector3 up = { 0.0f, 1.0f, 0.0f };
	// 이항 '=': 오른쪽 피연산자로 'T_Math::FMatrix' 형식을 사용하는 연산자가 없거나 허용되는 변환이 없습니다.
	m_MainCamera.SetView(eye, target, up);


	
}
void	Sample::PreFrame()
{
	m_Select.SetMatrix(nullptr, 
		&m_MainCamera.m_matView,
		&m_MainCamera.m_matProj);
	m_Select.Frame();

	std::vector<T::TVector3> vIntersectionList;
	if (I_Input.Get().KeyCheck(VK_MBUTTON) == KEY_PUSH)
	{
		for (int iFace = 0; iFace < m_Map.m_iNumFace; iFace++)
		{
			UINT i0 = m_Map.m_vIndexList[iFace*3+0];
			UINT i1 = m_Map.m_vIndexList[iFace * 3 + 1];
			UINT i2 = m_Map.m_vIndexList[iFace * 3 + 2];
			T::TVector3 v0 = m_Map.m_vVertexList[i0].p;
			T::TVector3 v1 = m_Map.m_vVertexList[i1].p;
			T::TVector3 v2 = m_Map.m_vVertexList[i2].p;
			// 교점
			T::TVector3 e = m_Select.m_Ray.vOrigin + m_Select.m_Ray.vDirecton * 1000.0f;
			T::TVector3 s = m_Select.m_Ray.vOrigin;
			T::TVector3 e0 = v1 - v0;
			T::TVector3 e1 = v2 - v0;
			T::TVector3 n;
			D3DXVec3Cross(&n, &e0, &e1);
			D3DXVec3Normalize(&n, &n);

			float t, u, v;
			if (m_Select.IntersectTriangle(
								m_Select.m_Ray.vOrigin,
								m_Select.m_Ray.vDirecton,
								v0, v1, v2, &t,&u,&v))
			{
				m_Select.m_vIntersection =
					m_Select.m_Ray.vOrigin + m_Select.m_Ray.vDirecton * t;
				vIntersectionList.push_back(m_Select.m_vIntersection);
				SetObject(m_Select.m_vIntersection);
			}
			////if (m_Select.GetIntersection(v0, v1, v2, n, s, e))
			////{				// 점포함 테스트
			////	if (m_Select.PointInPolygon(m_Select.m_vIntersection, n,
			////		v0, v1, v2))
			////	{
			////		vIntersectionList.push_back(m_Select.m_vIntersection);
			////	}
			////}
		}
	}
}
void    Sample::Frame()
{
	T::TVector3 vDir;
	T::TVector3 vPos = { 0, 500, 0 };
	T::TMatrix matRotation;
	D3DXMatrixRotationY(&matRotation, 0);
	D3DXVec3TransformCoord(&vPos, &vPos, &matRotation);
	D3DXVec3Normalize(&vDir,&vPos);
	m_LightInfo.m_vLightDir.x = -vDir.x;
	m_LightInfo.m_vLightDir.y = -vDir.y;
	m_LightInfo.m_vLightDir.z = -vDir.z;
	m_LightInfo.m_vLightDir.w = (cos(g_fGameTime)*0.5f+0.5f) * 3000.0f;

	m_LightInfo.m_vLightPos.x = vPos.x;
	m_LightInfo.m_vLightPos.y = vPos.y;
	m_LightInfo.m_vLightPos.z = vPos.z;
	m_LightInfo.m_vLightPos.w = 1.0f;
	TDevice::m_pContext->UpdateSubresource(
		m_pConstantBufferLight.Get(), 0, NULL, &m_LightInfo, 0, 0);


	for (int iFbx = 0; iFbx < m_pFbxfileList.size(); iFbx++)
	{
		m_pFbxfileList[iFbx]->Get()->Frame();
	}
}
void    Sample::Render()
{
	TDevice::m_pContext->VSSetConstantBuffers(1, 1, m_pConstantBufferLight.GetAddressOf());

	m_Map.SetMatrix(nullptr, &m_MainCamera.m_matView, &m_MainCamera.m_matProj);
	m_Map.PreRender(TDevice::m_pContext);
	
	TDevice::m_pContext->IASetIndexBuffer(
		m_Quadtree.m_pRootNode->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	//m_Quadtree.PostRender(TDevice::m_pContext, m_Quadtree.m_pRootNode);
	//m_Map.PostRender(TDevice::m_pContext);
	m_Quadtree.Render();

	m_Quadtree.m_Line.SetMatrix(nullptr, &m_MainCamera.m_matView, &m_MainCamera.m_matProj);
	m_Quadtree.DrawBB(m_Quadtree.m_pRootNode);

	static bool m_bMainCamera = false;
	if (TInput::Get().KeyCheck(VK_F3) == KEY_PUSH)
	{
		m_bMainCamera = !m_bMainCamera;
	}

	//m_pMapObjectList.emplace_back(obj);
	for (int iFbx = 0; iFbx < m_pMapObjectList.size(); iFbx++)
	{
		T::TMatrix matScale, matRotate, matTrans;
		D3DXMatrixScaling(&matScale,
			m_pMapObjectList[iFbx].vScale.x,
			m_pMapObjectList[iFbx].vScale.y,
			m_pMapObjectList[iFbx].vScale.z);
		D3DXMatrixTranslation(&matTrans,
			m_pMapObjectList[iFbx].vPos.x,
			m_pMapObjectList[iFbx].vPos.y,
			m_pMapObjectList[iFbx].vPos.z);
		
		D3DXMatrixRotationY(&matRotate,0/*g_fGameTime*0.1f*/);

		T::TVector3 vPos;
		D3DXVec3TransformCoord(&vPos, &m_pMapObjectList[iFbx].vPos, &matRotate);
		vPos.y = m_Map.GetHeight(vPos);
		D3DXMatrixTranslation(&matTrans,vPos.x, vPos.y, vPos.z);
		
		
		if (iFbx == 0 && m_bMainCamera)
		{
			T::TVector3 eye =
				vPos +
				T::TVector3(0, 0, -1) * 50.0f +
				T::TVector3(0, 1, 0) * 50.0f;
			T::TVector3 target = vPos;
			T::TVector3 up = { 0.0f, 1.0f, 0.0 };
			// 이항 '=': 오른쪽 피연산자로 'T_Math::FMatrix' 형식을 사용하는 연산자가 없거나 허용되는 변환이 없습니다.
			m_MainCamera.SetView(eye, target, up);
		}
		m_pMapObjectList[iFbx].matWorld = matScale * matTrans;	

		int iObjType = m_pMapObjectList[iFbx].iObjectType;
		
		auto model = std::dynamic_pointer_cast<TKgcObject>(m_pFbxfileList[iObjType]->Get());
		if (model)
		{
			model->Get()->m_matParentWorld = m_pMapObjectList[iFbx].matWorld;
			model->Get()->SetMatrix(nullptr, &m_MainCamera.m_matView, &m_MainCamera.m_matProj);
			model->Get()->Render(TDevice::m_pContext);
		}
	}
}
void    Sample::Release()
{
	I_Object.Release();
	m_Map.Release();
	m_Quadtree.Release();
}
void   Sample::LoadTextureAndPixelWriteSave(T_STR texName, T_STR texSave)
{
	D3D11_TEXTURE2D_DESC desc;
	ComPtr<ID3D11Resource> pResource2D = NULL;

	size_t maxsize = 0;
	HRESULT hr = DirectX::CreateWICTextureFromFileEx(
		TDevice::m_pd3dDevice.Get(),
		texName.c_str(),
		maxsize,
		D3D11_USAGE_STAGING,
		0,
		D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ,
		0,
		DirectX::DX11::WIC_LOADER_DEFAULT,
		pResource2D.GetAddressOf(),
		nullptr);

	ComPtr<ID3D11Texture2D> pTexture2D = NULL;
	if (FAILED(pResource2D->QueryInterface(__uuidof(ID3D11Texture2D),
		(void**)pTexture2D.GetAddressOf())))
	{

	}
	pTexture2D->GetDesc(&desc);

	D3D11_MAPPED_SUBRESOURCE mapped;
	if (SUCCEEDED(TDevice::m_pContext->Map(pTexture2D.Get(),
		D3D11CalcSubresource(0, 0, 1),
		D3D11_MAP_READ_WRITE, 0, &mapped)))
	{
		// 한줄의 시작 인덱스
		BYTE* pTexels = (BYTE*)mapped.pData;
		// RGBA
		for (int iRow = 0; iRow < desc.Height; iRow++)
		{
			// 한줄의 시작 인덱스
			BYTE* pByteAddress = pTexels;
			UINT rowStart = iRow * mapped.RowPitch;
			for (int iCol = 0; iCol < desc.Width; iCol++)
			{
				//if (iCol % 50 == 0 || iRow % 40 == 0)
				//{
				//	*pByteAddress++ = 0; // r
				//	*pByteAddress++ = 0; // g
				//	*pByteAddress++ = 0; // b
				//	*pByteAddress++ = 255; // a
				//}
				//else
				//{
				//	*pByteAddress++ = 255.0f - (float)iCol / desc.Width * 255.0f; // r
				//	*pByteAddress++ = 255.0f - (float)iCol / desc.Width * 255.0f; // g
				//	*pByteAddress++ = 255.0f - (float)iCol / desc.Width * 255.0f; // b
				//	*pByteAddress++ = 255; // a
				//}				
				* pByteAddress++ = 255.0f - (float)iCol / desc.Width * 255.0f; // r
				*pByteAddress++ = 255.0f - (float)iCol / desc.Width * 255.0f; // g
				*pByteAddress++ = 255.0f - (float)iCol / desc.Width * 255.0f; // b
				*pByteAddress++ = 255; // a
			}
			pTexels += mapped.RowPitch;
		}
		TDevice::m_pContext->Unmap(pTexture2D.Get(), D3D11CalcSubresource(0, 0, 1));
	}
	TTexMgr::SaveFile(texSave, pTexture2D.Get());
}

