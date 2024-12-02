#include "pch.h"
#include "Sample.h"
#include "TTexMgr.h"
template< typename T >
struct array_deleter
{
	void operator ()(T const* p)
	{
		delete[] p;
	}
};
bool Sample::CreateBufferSRV(ComPtr<ID3D11Buffer>& pBuffer,
	ComPtr<ID3D11ShaderResourceView>& srv)
{
	D3D11_BUFFER_DESC descBuf;
	ZeroMemory(&descBuf, sizeof(descBuf));
	pBuffer->GetDesc(&descBuf);

	D3D11_SHADER_RESOURCE_VIEW_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	desc.BufferEx.FirstElement = 0;

	//if (descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS)
	//{
	//	// This is a Raw Buffer

	//	desc.Format = DXGI_FORMAT_R32_TYPELESS;
	//	desc.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;
	//	desc.BufferEx.NumElements = descBuf.ByteWidth / 4;
	//}
	//else
	if (descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)
	{
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.BufferEx.NumElements =
			descBuf.ByteWidth / descBuf.StructureByteStride;
	}
	else
	{
		return E_INVALIDARG;
	}

	return TDevice::m_pd3dDevice->CreateShaderResourceView(
		pBuffer.Get(),
		&desc,
		srv.GetAddressOf());
}
bool  Sample::CreateStructureBuffer(
	UINT uElementSize,
	UINT uCount,
	VOID* pInitData,
	ComPtr<ID3D11Buffer>& sBuffer)
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BindFlags =
		D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	desc.ByteWidth = uElementSize * uCount;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = uElementSize;

	if (pInitData)
	{
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = pInitData;
		return TDevice::m_pd3dDevice->CreateBuffer(&desc,
			&InitData, sBuffer.GetAddressOf());
	}
	return TDevice::m_pd3dDevice->CreateBuffer(&desc, NULL,
		sBuffer.GetAddressOf());
}
HRESULT  Sample::CreateAlphaTex(UINT iWidth, UINT iHeight)
{
	HRESULT hr = S_OK;
	std::shared_ptr<int> buf(new int[iWidth * iHeight], array_deleter<int>());
	for (int i = 0; i < iHeight; i++)
	{
		for (int j = 0; j < iWidth; j++)
		{
			if ((i & 32) == (j & 32))
				buf.get()[i * iWidth + j] = 0x00000000;//0xffffffff ABGR
			else
				buf.get()[i * iWidth + j] = 0x00000000; //0xffffff00; ABGR
		}
	}

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = iWidth;
	textureDesc.Height = iHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// 텍스처 초기화 방법들
	//D3D11_USAGE_DEFAULT(	// 1) pInitialData	// 2)UpdateSubresource)
	//D3D11_USAGE_DYNAMIC Map()~Unmap()
	//D3D11_USAGE_STAGING(D3D11_MAP_WRITE) 1)Map()~Unmap() 2)CopyResource 
	D3D11_SUBRESOURCE_DATA pInitialData;
	ZeroMemory(&pInitialData, sizeof(pInitialData));
	pInitialData.pSysMem = (void*)buf.get();
	pInitialData.SysMemPitch = iWidth * 4;	// 1줄 바이트 크기 지정
	pInitialData.SysMemSlicePitch = iWidth * iHeight * 4; // 2D텍스처는 필요없다.

	hr = TDevice::m_pd3dDevice->CreateTexture2D(&textureDesc, &pInitialData,
					m_pAlphaTexture.GetAddressOf());
	if (FAILED(hr))
	{
		return hr;
	}
	//D3D11_BIND_UNORDERED_ACCESS 필요
	D3D11_UNORDERED_ACCESS_VIEW_DESC viewDescUAV;
	ZeroMemory(&viewDescUAV, sizeof(viewDescUAV));
	viewDescUAV.Format = textureDesc.Format;
	viewDescUAV.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	viewDescUAV.Texture2D.MipSlice = 0;
	hr = TDevice::m_pd3dDevice->CreateUnorderedAccessView(m_pAlphaTexture.Get(), 
		&viewDescUAV, m_pAlphaTexUAV.GetAddressOf());
	if (FAILED(hr))
	{
		return hr;
	}
	//D3D11_BIND_SHADER_RESOURCE 필요
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	hr = TDevice::m_pd3dDevice->CreateShaderResourceView(m_pAlphaTexture.Get(), &srvDesc, 
		m_pAlphaTexSRV.GetAddressOf());
	if (FAILED(hr))
	{
		return hr;
	}
	return hr;
}
void   Sample::Splatting()
{
	TLoadData ld;
	ld.m_csLoadFileName = L"../../data/map/tiles/Tile28.jpg";
	m_pTexSplatting[0] = I_Tex.Load(ld).get();
	ld.m_csLoadFileName = L"../../data/map/tiles/020.bmp";
	m_pTexSplatting[1] = I_Tex.Load(ld).get();
	ld.m_csLoadFileName = L"../../data/map/tiles/Snow.jpg";
	m_pTexSplatting[2] = I_Tex.Load(ld).get();
	ld.m_csLoadFileName = L"../../data/map/tiles/Tile42.jpg";
	m_pTexSplatting[3] = I_Tex.Load(ld).get();

	m_pCS.Attach(
		DX::LoadComputeShaderFile(
			TDevice::m_pd3dDevice.Get(),
			L"CS.txt", nullptr, "CSMain"));
	float fX = 100.0f;
	float fY = 0.0f;
	m_SelectPick.vPickPos = { -fX,fY, 0.0f };
	m_SelectPick.fRaidus = 256.0f;
	m_SelectPick.iIndex = 1; // A=0,B=1,G=2,R=3
	m_SelectPick.fTexWidth = 1024.0f;
	m_SelectPick.fTexHeight = 1024.0f;
	
	CreateStructureBuffer(sizeof(PickData), 1, &m_SelectPick, m_pBuf0);	
	CreateBufferSRV(m_pBuf0, m_pBuf0SRV);
	CreateAlphaTex(m_iTextureWidth, m_iTextureHeight);

	ID3D11ShaderResourceView* SRVArray[] = {m_pBuf0SRV.Get()};
	TDevice::m_pContext->CSSetShader(m_pCS.Get(), NULL, 0);
	TDevice::m_pContext->CSSetShaderResources(0, 1, SRVArray);
	TDevice::m_pContext->CSSetUnorderedAccessViews(0, 1, m_pAlphaTexUAV.GetAddressOf(), NULL);

	// groupid= 1024*1*1
	// groupthreadid = [0~1023][1][1]
	// numthreads(1,1,1) -> thread = 0~1023;
	// SV_DispatchThreadID = 0~1024 -> CSMain()
	// x(32) = 1024 / 32
	// y(32) = 1024 / 32
	TDevice::m_pContext->Dispatch(  m_iTextureWidth/32.0f,
									m_iTextureHeight/32.0f, 1);

	TDevice::m_pContext->CSSetShader(NULL, NULL, 0);
	ID3D11UnorderedAccessView* ppUAViewNULL[1] = { NULL };
	TDevice::m_pContext->CSSetUnorderedAccessViews(0, 1, ppUAViewNULL, NULL);

	ID3D11ShaderResourceView* ppSRVNULL[2] = { NULL, NULL };
	TDevice::m_pContext->CSSetShaderResources(0, 2, ppSRVNULL);

	ID3D11Buffer* ppCBNULL[1] = { NULL };
	TDevice::m_pContext->CSSetConstantBuffers(0, 1, ppCBNULL);
}
void   Sample::SetObject(T::TVector3 vPos)
{
	TMapObject obj;
	int iNumObject = I_Object.m_list.size();
	obj.iObjectType = rand() % m_pFbxfileList.size();
	obj.m_pMesh = std::dynamic_pointer_cast<TKgcObject>(m_pFbxfileList[obj.iObjectType]->Get()).get();
	obj.m_pAnimMatrix = obj.m_pMesh;

	std::wstring expFilename = L"../../data/kgc/MM_Idle.kgc";
	//std::wstring expFilename = L"../../data/kgc/MM_Walk_Fwd.kgc";	
	TLoadData ld;
	ld.m_csLoadFileName = expFilename;
	ld.m_csLoadShaderFileName = L"CharacterLightting.hlsl";
	auto bLoadObject = I_Object.GetPtr(ld.m_csLoadFileName);
	if (bLoadObject == nullptr)
	{
		auto tObjectWalking = I_Object.Load(ld);
		if (tObjectWalking != nullptr)
		{
			obj.m_pAnimMatrix = std::dynamic_pointer_cast<TKgcObject>(tObjectWalking->Get()).get();
			obj.m_pAnimMatrix->Get()->RebuildAnimation(obj.m_pMesh->Get().get());
		}
	}
	else
	{
		obj.m_pAnimMatrix = std::dynamic_pointer_cast<TKgcObject>(bLoadObject->Get()).get();
	}
	obj.vPos = vPos;
	obj.vScale = { 1.0f, 1.0f, 1.0f };
	D3DXMatrixScaling(&obj.matWorld, obj.vScale.x, obj.vScale.y, obj.vScale.z);
	obj.matWorld._41 = obj.vPos.x;
	obj.matWorld._42 = obj.vPos.y;
	obj.matWorld._43 = obj.vPos.z;
	obj.m_FileHeader = obj.m_pAnimMatrix->Get()->m_FileHeader;	
	obj.SetAnimFrame(obj.m_FileHeader.iStartFrame,obj.m_FileHeader.iLastFrame);

	obj.Init();
	m_pMapObjectList.emplace_back(obj);
	
}
void   Sample::CreateTopViewRT()
{
	m_TopViewRT.Create(TDevice::m_pd3dDevice.Get(), 1024, 1024);
	T::TVector3 eye = { 0.0f, 10000.0f, -1.0f };
	T::TVector3 target = { 0.0f, 0.0f, 0.0f };
	T::TVector3 up = { 0.0f, 1.0f, 0.0f };
	// 이항 '=': 오른쪽 피연산자로 'T_Math::FMatrix' 형식을 사용하는 연산자가 없거나 허용되는 변환이 없습니다.
	m_TopViewCamera.SetView(eye, target, up);
	m_TopViewCamera.SetProj(XM_PI * 0.25f,
		(float)300.0f / (float)300.0f, 1.0f, 10000.0f);
}
void   Sample::Init()
{
	Splatting();
	CreateTopViewRT();

	m_LightInfo.m_vLightDir = T::TVector4(0, -1, 0, 1);
	m_pConstantBufferLight.Attach(
		TDxObject::CreateConstantBuffer(TDevice::m_pd3dDevice.Get(),
		&m_LightInfo, 1, sizeof(TLightInfo)));
	
	m_Map.LoadHeightMapTexture(
		//L"../../data/map/heightmap/HEIGHT_CASTLE.bmp",
		L"../../data/map/heightmap/heightMap513.bmp");

	TMapDesc desc = { m_Map.m_HeightMapDesc.Width,
					m_Map.m_HeightMapDesc.Height, 1.0f, 0.1f,
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
	//m_LoadFiles.push_back(L"../../data/kgc/Swat.kgc");
	//m_LoadFiles.push_back(L"../../data/kgc/Man.kgc");
	//m_LoadFiles.push_back(L"../../data/kgc/SK_Mannequin.kgc");	
	m_LoadFiles.push_back(L"../../data/kgc/SKM_Manny_Simple.kgc");
	//m_LoadFiles.push_back(L"../../data/kgc/Turret_Deploy1.kgc"); 	
	TLoadData ld;	
	//ld.m_csLoadShaderFileName = L"CharacterLightting.hlsl";
	ld.m_csLoadShaderFileName = L"CharacterInstance.hlsl";
	for (int iObj = 0; iObj < m_LoadFiles.size(); iObj++)
	{
		ld.m_csLoadFileName = m_LoadFiles[iObj];
		auto tObject = I_Object.Load(ld);
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

	m_Quadtree.Set(&m_Map, &m_MainCamera);
	m_Quadtree.Init();
	m_Quadtree.BuildTree(m_Quadtree.m_pRootNode);

	T::TVector3 eye = { 0.0f, 500.0f, -500.0f };
	T::TVector3 target = { 0.0f, 0.0f, 0.0f };
	T::TVector3 up = { 0.0f, 1.0f, 0.0f };
	// 이항 '=': 오른쪽 피연산자로 'T_Math::FMatrix' 형식을 사용하는 연산자가 없거나 허용되는 변환이 없습니다.
	m_MainCamera.SetView(eye, target, up);


	m_MiniMapObj.Create(L"../../data/22879344.jpg", L"../../data/shader/ScreenQuad.txt");
}
void	Sample::PreFrame()
{
	m_Select.SetMatrix(nullptr, 
		&m_MainCamera.m_matView,
		&m_MainCamera.m_matProj);
	m_Select.Frame();

	std::vector<T::TVector3> vIntersectionList;
	if (I_Input.Get().KeyCheck(VK_MBUTTON) == KEY_UP)
	{
		for (auto node : m_Quadtree.m_DrawNodes)
		{
			BOOL bRet = m_Select.ChkBoxToRay(node->m_Box);
			if (bRet == TRUE)
			{
				//node->m_Box : m_Select.m_Ray
				//node->m_Sphere : m_Select.m_Ray

				for (int iFace = 0; iFace < node->m_IndexList.size()/3; iFace++)
				{
					UINT i0 = node->m_IndexList[iFace * 3 + 0];
					UINT i1 = node->m_IndexList[iFace * 3 + 1];
					UINT i2 = node->m_IndexList[iFace * 3 + 2];
					T::TVector3 v0 = m_Map.m_vVertexList[i0].p;
					T::TVector3 v1 = m_Map.m_vVertexList[i1].p;
					T::TVector3 v2 = m_Map.m_vVertexList[i2].p;
					// 교점
					T::TVector3 e = m_Select.m_Ray.vOrigin + m_Select.m_Ray.vDirection * 1000.0f;
					T::TVector3 s = m_Select.m_Ray.vOrigin;
					T::TVector3 e0 = v1 - v0;
					T::TVector3 e1 = v2 - v0;
					T::TVector3 n;
					D3DXVec3Cross(&n, &e0, &e1);
					D3DXVec3Normalize(&n, &n);

					float t, u, v;
					if (m_Select.IntersectTriangle(
						m_Select.m_Ray.vOrigin,
						m_Select.m_Ray.vDirection,
						v0, v1, v2, &t, &u, &v))
					{
						m_Select.m_vIntersection =
							m_Select.m_Ray.vOrigin + m_Select.m_Ray.vDirection * t;
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
	TDevice::m_pContext->UpdateSubresource(	m_pConstantBufferLight.Get(), 0, NULL, &m_LightInfo, 0, 0);
	m_Quadtree.Frame();

	for (int iFbx = 0; iFbx < m_pMapObjectList.size(); iFbx++)
	{
		for (int iObj = 0; iObj < 10; iObj++)
		{
			m_pMapObjectList[iFbx].InstanceUpdate(iObj,
				T::TVector3(iObj*100.0f, iObj* 50.0f, iObj*100.0f));
		}
		m_pMapObjectList[iFbx].Frame();
	}

	//if (m_pMapObjectList.size() > 0)
	//{
	//	if (I_Input.Get().KeyCheck(VK_UP) == KEY_HOLD)
	//	{
	//		//for (int iFbx = 0; iFbx < m_pMapObjectList.size(); iFbx++)
	//		{
	//			m_pMapObjectList[1].SetAnimFrame(61, 91);
	//		}
	//	}
	//	else
	//	{
	//		//for (int iFbx = 0; iFbx < m_pMapObjectList.size(); iFbx++)
	//		{
	//			m_pMapObjectList[1].SetAnimFrame(0, 60);
	//		}
	//	}
	//}
}
void    Sample::PostFrame()
{	
	m_TopViewRT.Begin();
	MapRender(&m_TopViewCamera);
	m_MainCamera.FrustumRender( m_TopViewCamera.m_matView,
							    m_TopViewCamera.m_matProj);
	m_TopViewRT.End();	

	m_MiniMapObj.m_pSRV_RT = m_TopViewRT.m_pSRV_RT;
}
void    Sample::MapRender(TCamera* pCamera)
{
	TDevice::m_pContext->VSSetConstantBuffers(1, 1, m_pConstantBufferLight.GetAddressOf());
	m_Map.SetMatrix(nullptr, &pCamera->m_matView,
							 &pCamera->m_matProj);
	m_Map.PreRender(TDevice::m_pContext);
	
	ID3D11ShaderResourceView* aRViews[4] = { m_pTexSplatting[0]->m_pSRV.Get(),
											 m_pTexSplatting[1]->m_pSRV.Get(),
											 m_pTexSplatting[2]->m_pSRV.Get(),
											 m_pTexSplatting[3]->m_pSRV.Get() };
	TDevice::m_pContext->PSSetShaderResources(2, 4, aRViews);
	TDevice::m_pContext->PSSetShaderResources(1, 1, m_pAlphaTexSRV.GetAddressOf());
	
	TDevice::m_pContext->IASetIndexBuffer(
		m_Quadtree.m_pRootNode->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	//m_Quadtree.PostRender(TDevice::m_pContext, m_Quadtree.m_pRootNode);
	//m_Map.PostRender(TDevice::m_pContext);
	// 
	m_Quadtree.Render();

	m_Quadtree.m_Line.SetMatrix(nullptr, &m_MainCamera.m_matView, &m_MainCamera.m_matProj);
	//m_Quadtree.DrawBB(m_Quadtree.m_pRootNode);
}
void    Sample::Render()
{
	MapRender(&m_MainCamera);

	static bool m_bMainCamera = false;
	if (TInput::Get().KeyCheck(VK_F3) == KEY_PUSH)
	{
		m_bMainCamera = !m_bMainCamera;
	}

	for (int iFbx = 0; iFbx < m_pMapObjectList.size(); iFbx++)
	{
		m_pMapObjectList[iFbx].Render(m_Map, m_MainCamera);
	}

	MiniMapRander(nullptr);	
}
void    Sample::MiniMapRander(ID3D11ShaderResourceView* pSRV)
{
	m_MiniMapObj.SetMatrix(nullptr, nullptr, nullptr);
	m_MiniMapObj.Render(TDevice::m_pContext);
}
void    Sample::Release()
{
	m_MiniMapObj.Release();
	m_TopViewRT.Release();

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

