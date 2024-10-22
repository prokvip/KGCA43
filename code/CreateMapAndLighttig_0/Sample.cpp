#include "Sample.h"
#include "TTexMgr.h"
#include "ScreenGrab.h"
bool	 Sample::CreateConstantBuffer(ID3D11Device* pd3dDevice)
{
	// 버퍼 할당 크기를 세팅한다.
	D3D11_BUFFER_DESC  bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(TLightInfo);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;


	// 할당된 버퍼에 
	// 시스템메모리가 ->GPU메모리로 체워지는 데이터
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_LightInfo;

	HRESULT hr = pd3dDevice->CreateBuffer(
		&bd,
		&sd,
		m_pConstantBufferLight.GetAddressOf());
	if (FAILED(hr)) return false;
	return true;
}
ID3D11Buffer* Sample::CreateConstantBuffer(
	ID3D11Device* pd3dDevice, 
	void* data, UINT iNumIndex,
	UINT iSize, bool bDynamic)
{
	HRESULT hr = S_OK;
	ID3D11Buffer* pBuffer = nullptr;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	if (bDynamic)
	{
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0;
	}
	bd.ByteWidth = iSize * iNumIndex;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	InitData.pSysMem = data;
	if (data != NULL)
	{
		if (FAILED(hr = pd3dDevice->CreateBuffer(&bd, &InitData, &pBuffer)))
		{			
			return nullptr;
		}
	}
	else
	{
		if (FAILED(hr = pd3dDevice->CreateBuffer(&bd, NULL, &pBuffer)))
		{			
			return nullptr;
		}
	}
	return pBuffer;
}
void   Sample::Init()
{
	m_LightInfo.m_vLightDir = T::TVector4(0, -1, 0, 1);
	CreateConstantBuffer(TDevice::m_pd3dDevice.Get());
	
	/*LoadTextureAndPixelWriteSave(L"../../data/map/heightmap/heightMap513.bmp", 
								 L"SaveLine");*/
	LoadHeightMapTexture(
		//L"../../data/map/heightmap/HEIGHT_CASTLE.bmp",
		L"../../data/map/heightmap/heightMap513.bmp",		
		//L"SaveLine.jpg",
		m_Map);

	TMapDesc desc = { m_Map.m_HeightMapDesc.Width,
					m_Map.m_HeightMapDesc.Height, 10.0f, 2.0f,
		//L"../../data/map/heightmap/castle.jpg",
		L"../../data/map/001.jpg",		
		L"Lightting.hlsl"};

	if (!m_Map.Load(desc))
	{
		return;
	}

	//m_LoadFiles.push_back(L"../../data/kgc/box.kgc");
	m_LoadFiles.push_back(L"../../data/kgc/SM_Rock.kgc");
	m_LoadFiles.push_back(L"../../data/kgc/SM_Barrel.kgc");
	m_LoadFiles.push_back(L"../../data/kgc/Man.kgc");
	for (int iObj = 0; iObj < m_LoadFiles.size(); iObj++)
	{
		auto model1 = std::make_shared<TFbxModel>();
		if (TKgcFileFormat::Import(m_LoadFiles[iObj], L"Lightting.hlsl", model1))
		{
			m_pFbxfileList.emplace_back(model1);
		}
	}
	for (int iObj = 0; iObj < 1000; iObj++)
	{
		TMapObject obj;
		obj.iObjectType = rand() % 3;
		obj.vPos = { randstep(-2000.0f, +2000.0f), 0.0f, randstep(-2000.0f, +2000.0f) };
		obj.vScale = { randstep(0.1f, 2.0f), randstep(0.1f, 2.0f), randstep(0.1f, 2.0f) };
		D3DXMatrixScaling(&obj.matWorld, obj.vScale.x, obj.vScale.y, obj.vScale.z);
		obj.matWorld._41 = obj.vPos.x;
		obj.matWorld._42 = m_Map.GetHeight(obj.vPos);
		obj.matWorld._43 = obj.vPos.z;
		m_pMapObjectList.emplace_back(obj);
	}


	T::TVector3 eye = { 0.0f, 500.0f, -500.0f };
	T::TVector3 target = { 0.0f, 0.0f, 0.0f };
	T::TVector3 up = { 0.0f, 1.0f, 0.0f };
	// 이항 '=': 오른쪽 피연산자로 'T_Math::FMatrix' 형식을 사용하는 연산자가 없거나 허용되는 변환이 없습니다.
	m_MainCamera.SetView(eye, target, up);


	
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
		m_pFbxfileList[iFbx]->Frame();
	}
}
void    Sample::Render()
{
	TDevice::m_pContext->VSSetConstantBuffers(1, 1, 
		m_pConstantBufferLight.GetAddressOf());

	m_Map.SetMatrix(nullptr, &m_MainCamera.m_matView, &m_matProj);
	m_Map.Render(TDevice::m_pContext);
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
		m_pFbxfileList[iObjType]->m_matParentWorld = m_pMapObjectList[iFbx].matWorld;

		m_pFbxfileList[iObjType]->SetMatrix(
			nullptr, &m_MainCamera.m_matView, &m_matProj);
		m_pFbxfileList[iObjType]->Render(TDevice::m_pContext);
	}
}
void    Sample::Release()
{
	for (int iFbx = 0; iFbx < m_pFbxfileList.size(); iFbx++)
	{
		m_pFbxfileList[iFbx]->Release();
	}
	m_fbxLoader.Release();

	m_Map.Release();
}
void Sample::SaveFile(T_STR name, ID3D11Texture2D* pRes)
{
	HRESULT hr = S_OK;
	ComPtr<ID3D11Texture2D> tex;
	if (pRes == nullptr)
	{
		hr = TDevice::m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&tex);
	}
	else
	{
		tex = pRes;
	}
	if (SUCCEEDED(hr))
	{
		hr = DirectX::SaveWICTextureToFile(TDevice::m_pContext, tex.Get(), GUID_ContainerFormatPng, (name + L".png").c_str());
		hr = DirectX::SaveWICTextureToFile(TDevice::m_pContext, tex.Get(), GUID_ContainerFormatJpeg, (name + L".jpg").c_str());
		hr = DirectX::SaveWICTextureToFile(TDevice::m_pContext, tex.Get(), GUID_ContainerFormatBmp, (name + L".bmp").c_str());
		hr = DirectX::SaveWICTextureToFile(TDevice::m_pContext, tex.Get(), GUID_ContainerFormatBmp, (name + L".bmp").c_str(), &GUID_WICPixelFormat16bppBGR565);
		hr = DirectX::SaveWICTextureToFile(TDevice::m_pContext, tex.Get(), GUID_ContainerFormatTiff, (name + L".tif").c_str());
		hr = DirectX::SaveWICTextureToFile(TDevice::m_pContext, tex.Get(), GUID_ContainerFormatTiff, (name + L".tif").c_str(), nullptr, [&](IPropertyBag2* props)
			{
				PROPBAG2 options[2] = { 0, 0 };
				options[0].pstrName = const_cast<WCHAR*>(OLESTR("CompressionQuality"));
				options[1].pstrName = const_cast<WCHAR*>(OLESTR("TiffCompressionMethod"));

				VARIANT varValues[2];
				varValues[0].vt = VT_R4;
				varValues[0].fltVal = 0.75f;

				varValues[1].vt = VT_UI1;
				varValues[1].bVal = WICTiffCompressionNone;

				(void)props->Write(2, options, varValues);
			});

		hr = DirectX::SaveDDSTextureToFile(TDevice::m_pContext, tex.Get(), (name + L".dds").c_str());
	}
	if (pRes == nullptr)
	{
		tex->Release();
	}
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
	SaveFile(texSave, pTexture2D.Get());
}
void   Sample::LoadHeightMapTexture(T_STR texName, TMap& map)
{
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

	pTexture2D->GetDesc(&map.m_HeightMapDesc);

	map.m_HeightList.resize(map.m_HeightMapDesc.Height * map.m_HeightMapDesc.Width);

	D3D11_MAPPED_SUBRESOURCE mapped;
	if (SUCCEEDED(TDevice::m_pContext->Map(pTexture2D.Get(),
		D3D11CalcSubresource(0, 0, 1),
		D3D11_MAP_READ, 0, &mapped)))
	{
		UCHAR* pTexels = (UCHAR*)mapped.pData;
		// RGBA
		for (int iRow = 0; iRow < map.m_HeightMapDesc.Height; iRow++)
		{
			// 한줄의 시작 인덱스
			UINT rowStart = iRow * mapped.RowPitch;
			for (int iCol = 0; iCol < map.m_HeightMapDesc.Width; iCol++)
			{
				// 한 픽셀 단위로 이동
				UINT colStart = iCol * 4;
				UINT r = pTexels[rowStart + colStart + 0];
				UINT g = pTexels[rowStart + colStart + 1];
				UINT b = pTexels[rowStart + colStart + 2];
				UINT a = pTexels[rowStart + colStart + 3];
				map.m_HeightList[iRow * map.m_HeightMapDesc.Width + iCol] = (float)r;
			}
		}
		TDevice::m_pContext->Unmap(pTexture2D.Get(), D3D11CalcSubresource(0, 0, 1));
	}
}
T_GAME_START(800, 600);
