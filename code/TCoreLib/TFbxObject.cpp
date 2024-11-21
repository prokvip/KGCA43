#include "TFbxObject.h"
void	 TFbxModel::LoadTexture(std::wstring szPath) 
{
	if (m_vSubMeshVertexList.size() == 0)
	{
		TDxObject3D::LoadTexture(szPath);
		return;
	}
	else
	{
		m_pSubMeshSRV.resize(m_vSubMeshVertexList.size());
		m_pSubMeshTexture.resize(m_vSubMeshVertexList.size());
		for (int iMesh = 0; iMesh < m_pSubMeshSRV.size(); iMesh++)
		{
			std::wstring name = L"../../data/";
			std::wstring texPath = m_szTexFileList[iMesh];
			wchar_t  szDrive[MAX_PATH] = { 0, };
			wchar_t  szDir[MAX_PATH] = { 0, };
			wchar_t  szFileName[MAX_PATH] = { 0, };
			wchar_t  szFileExt[MAX_PATH] = { 0, };
			_tsplitpath_s(texPath.c_str(), szDrive, szDir, szFileName, szFileExt);
			name += szFileName;
			std::wstring ext = szFileExt;

			if (ext == L".tga")
			{
				name += L".dds";
			}
			else
			{
				name += szFileExt;
			}

			m_pSubMeshTexture[iMesh] = I_Tex.Load(name).get();
			if (m_pSubMeshTexture[iMesh] != nullptr)
			{
				m_pSubMeshSRV[iMesh] = m_pSubMeshTexture[iMesh]->m_pSRV;
			}
		}
	}
}
bool     TFbxModel::CreateIWVertexBuffer(ID3D11Device* pd3dDevice)
{
	if (m_vSubMeshIWVertexList.size() == 0)
	{
		if (m_vIWVertexList.size() == 0) return true;
		// 버퍼 할당 크기를 세팅한다.
		D3D11_BUFFER_DESC  bd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
		bd.ByteWidth = sizeof(IW_Vertex) * m_vIWVertexList.size();
		// 연결에 용도가 뭐냐? 
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;


		// 할당된 버퍼에 
		// 시스템메모리가 ->GPU메모리로 체워지는 데이터
		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
		sd.pSysMem = &m_vIWVertexList.at(0);

		HRESULT hr = pd3dDevice->CreateBuffer(
			&bd,
			&sd,
			m_pIWVertexBuffer.GetAddressOf());
		if (FAILED(hr)) return false;
	}
	else
	{
		m_pSubMeshIWVertexBuffer.resize(m_vSubMeshVertexList.size());
		for (int iMesh = 0; iMesh < m_vSubMeshIWVertexList.size(); iMesh++)
		{
			if (m_vSubMeshIWVertexList[iMesh].size() <= 0) continue;

			D3D11_BUFFER_DESC  bd;
			ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
			bd.ByteWidth = sizeof(IW_Vertex) * m_vSubMeshIWVertexList[iMesh].size();
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
			sd.pSysMem = &m_vSubMeshIWVertexList[iMesh].at(0);

			HRESULT hr = pd3dDevice->CreateBuffer(
				&bd,
				&sd,
				m_pSubMeshIWVertexBuffer[iMesh].GetAddressOf());
			if (FAILED(hr)) return false;
		}
	}
	return true;
}
bool     TFbxModel::CreateVertexBuffer(ID3D11Device* pd3dDevice)
{
	if (m_vSubMeshVertexList.size() == 0)
	{
		TDxObject3D::CreateVertexBuffer(pd3dDevice);
		return true;
	}
	else
	{
		m_pSubMeshVertexBuffer.resize(m_vSubMeshVertexList.size());
		for (int iMesh = 0; iMesh < m_vSubMeshVertexList.size(); iMesh++)
		{			
			if (m_vSubMeshVertexList[iMesh].size() <= 0) continue;

			D3D11_BUFFER_DESC  bd;
			ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
			bd.ByteWidth = sizeof(PNCT_Vertex) * m_vSubMeshVertexList[iMesh].size();			
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
			sd.pSysMem = &m_vSubMeshVertexList[iMesh].at(0);

			HRESULT hr = pd3dDevice->CreateBuffer(
				&bd,
				&sd,
				m_pSubMeshVertexBuffer[iMesh].GetAddressOf());
			if (FAILED(hr)) return false;
		}
	}
	return true;
}
bool     TFbxModel::CreateIndexBuffer(ID3D11Device* pd3dDevice)
{
	if (m_vSubMeshIndexList.size() == 0)
	{
		TDxObject3D::CreateIndexBuffer(pd3dDevice);
		return true;
	}
	else
	{
		m_pSubMeshIndexBuffer.resize(m_vSubMeshIndexList.size());
		for (int iMesh = 0; iMesh < m_vSubMeshIndexList.size(); iMesh++)
		{
			if (m_vSubMeshIndexList[iMesh].size() <= 0) continue;
			D3D11_BUFFER_DESC  bd;
			ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
			bd.ByteWidth = sizeof(DWORD) * m_vSubMeshIndexList[iMesh].size();			
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
			sd.pSysMem = &m_vSubMeshIndexList[iMesh].at(0);

			HRESULT hr = pd3dDevice->CreateBuffer(
				&bd,
				&sd,
				m_pSubMeshIndexBuffer[iMesh].GetAddressOf());
			if (FAILED(hr)) return false;
		}
	}
	return true;
}
void     TFbxModel::SetAnimFrame(float s, float e)
{
	m_fStartFrame = s;
	m_fEndFrame = e;
}
void     TFbxModel::Render(ID3D11DeviceContext* pContext)
{	
	for (int iChild = 0; iChild < m_ChildModel.size(); iChild++)
	{
		auto pModel = m_ChildModel[iChild];
		//pModel->m_matWorld = pModel->m_pAnimationMatrix[m_fFrameAnimation];
		//pModel->m_matWorld = pModel->m_pAnimationMatrix[m_fFrameAnimation];		
		D3DXMatrixIdentity(&pModel->m_matWorld);
		pModel->m_matParentWorld = m_matParentWorld;
		pModel->SetMatrix(nullptr, &m_matView, &m_matProj);
		
		pModel->PreRender(pContext);

		// skinning
		pContext->VSSetConstantBuffers(2, 1, pModel->m_pBoneCB.GetAddressOf());

		if (pModel->m_pSubMeshVertexBuffer.size())
		{
			for (int iSubMesh = 0; iSubMesh < pModel->m_pSubMeshVertexBuffer.size(); iSubMesh++)
			{
				UINT StartSlot = 0;
				UINT NumBuffers = 1;
				UINT pStrides[2] = { sizeof(PNCT_Vertex), sizeof(IW_Vertex) }; // 1개의 정점 크기
				UINT pOffsets[2] = { 0, 0 }; // 버퍼에 시작 인덱스
				ID3D11Buffer* buffer[2] = { pModel->m_pSubMeshVertexBuffer[iSubMesh].Get(),
											pModel->m_pSubMeshIWVertexBuffer[iSubMesh].Get()  };
				NumBuffers = 2;
				pContext->IASetVertexBuffers(0, 2,buffer, pStrides, pOffsets);
				// 
				//pContext->IASetVertexBuffers(StartSlot, NumBuffers,
				//	pModel->m_pSubMeshVertexBuffer[iSubMesh].GetAddressOf(), &pStrides, &pOffsets);
				
				pContext->PSSetShaderResources(0, 1, pModel->m_pSubMeshSRV[iSubMesh].GetAddressOf());
				if (m_pSubMeshIndexBuffer.size())
				{
					pContext->IASetIndexBuffer(pModel->m_pSubMeshIndexBuffer[iSubMesh].Get(), DXGI_FORMAT_R32_UINT, 0);

					if (pModel->m_pSubMeshIndexBuffer[iSubMesh] != nullptr)
					{
						pContext->DrawIndexed(pModel->m_vSubMeshIndexList[iSubMesh].size(), 0, 0);
					}
				}
				else
				{
					pContext->Draw(pModel->m_vSubMeshVertexList[iSubMesh].size(), 0);
				}
			}
		}
		else
		{
			UINT StartSlot = 0;
			UINT NumBuffers = 1;
			UINT pStrides[2] = { sizeof(PNCT_Vertex), sizeof(IW_Vertex) }; // 1개의 정점 크기
			UINT pOffsets[2] = { 0, 0 }; // 버퍼에 시작 인덱스
			ID3D11Buffer* buffer[2] = { pModel->m_pVertexBuffer,
										pModel->m_pIWVertexBuffer.Get() };
			NumBuffers = 2;
			pContext->IASetVertexBuffers(0, 2, buffer, pStrides, pOffsets);
			pModel->PostRender(pContext);
		}
	}
}
void     TFbxModel::Release() 
{
	for (int iChild = 0; iChild < m_ChildModel.size(); iChild++)
	{
		m_ChildModel[iChild]->Release();
	}
	TDxObject3D::Release();

	m_szTexFileList.clear();
	m_pSubMeshSRV.clear();
	m_pSubMeshTexture.clear();
	m_vSubMeshVertexList.clear();
	m_vSubMeshVertexList.clear();
	m_pSubMeshVertexBuffer.clear();
}
void     TFbxModel::SetVertexData() 
{
}
bool     TFbxModel::CreateInputLayout(ID3D11Device* pd3dDevice)
{
	//if(m_vIWVertexList.size() > 0)
	/*const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POS",0,	DXGI_FORMAT_R32G32B32_FLOAT,		0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{"NOR",0,	DXGI_FORMAT_R32G32B32_FLOAT,		0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{"COL",0,DXGI_FORMAT_R32G32B32A32_FLOAT,			0,24,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{"TEX",0,DXGI_FORMAT_R32G32_FLOAT,				0,40,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};*/

	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POS",0,	DXGI_FORMAT_R32G32B32_FLOAT,		0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{"NOR",0,	DXGI_FORMAT_R32G32B32_FLOAT,		0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{"COL",0,DXGI_FORMAT_R32G32B32A32_FLOAT,		0,24,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{"TEX",0,DXGI_FORMAT_R32G32_FLOAT,				0,40,D3D11_INPUT_PER_VERTEX_DATA,0 },

		{"INDEX",0,	DXGI_FORMAT_R32G32B32_FLOAT,		1,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{"WEIGHT",0,DXGI_FORMAT_R32G32B32A32_FLOAT,		1,16,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	UINT NumElements = sizeof(layout) / sizeof(layout[0]);
	HRESULT hr = pd3dDevice->CreateInputLayout(
		layout,
		NumElements,
		m_pShader->VS_Bytecode->GetBufferPointer(),
		m_pShader->VS_Bytecode->GetBufferSize(),
		&m_pVertexLayout);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}
bool	 TFbxModel::CreateConstantBuffer(ID3D11Device* pd3dDevice)
{
	TDxObject3D::CreateConstantBuffer(pd3dDevice);

	// 버퍼 할당 크기를 세팅한다.
	D3D11_BUFFER_DESC  bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(TBoneMatrix);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;


	// 할당된 버퍼에 
	// 시스템메모리가 ->GPU메모리로 체워지는 데이터
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_matBoneList;

	HRESULT hr = pd3dDevice->CreateBuffer(
		&bd,
		&sd,
		m_pBoneCB.GetAddressOf());
	if (FAILED(hr)) return false;
	return true;
}
void	 TFbxModel::Frame() {} 
void     TFbxModel::AnimFrame(float& fAnimFrame)
{
	T::TMatrix matAnim;
	for (int iChild = 0; iChild < m_ChildModel.size(); iChild++)
	{
		auto pModel = m_ChildModel[iChild];
		for (int iBone = 0; iBone < pModel->m_matBindPose.size(); iBone++)
		{
			matAnim = pModel->m_matBindPose[iBone] * 
				m_pBoneAnimMatrix[iBone][fAnimFrame];
			D3DXMatrixTranspose(
				&pModel->m_matBoneList.matBone[iBone],&matAnim);
		}
		TDevice::m_pContext->UpdateSubresource(pModel->m_pBoneCB.Get(),	0, NULL, &pModel->m_matBoneList, 0, 0);
	}
}
void     TFbxModel::AnimFrame(float& fAnimFrame, TFbxModel* pAnim)
{
	T::TMatrix matAnim;
	for (int iChild = 0; iChild < m_ChildModel.size(); iChild++)
	{
		auto pModel = m_ChildModel[iChild];
		for (int iBone = 0; iBone < pModel->m_matBindPose.size(); iBone++)
		{			
			//T::TMatrix matbone = GetBoneMatrix(pAnim, m_pTNodeList[iBone].szName, fAnimFrame);
			//matAnim = pModel->m_matBindPose[iBone] * matbone;// pAnim->m_pBoneAnimMatrix[iBone][fAnimFrame];
			matAnim = pModel->m_matBindPose[iBone] * pAnim->m_pBoneAnimMatrix[iBone][fAnimFrame];
			D3DXMatrixTranspose(&pModel->m_matBoneList.matBone[iBone], &matAnim);
		}
		TDevice::m_pContext->UpdateSubresource(pModel->m_pBoneCB.Get(), 0, NULL, &pModel->m_matBoneList, 0, 0);
	}
}
T::TMatrix TFbxModel::GetBoneMatrix(TFbxModel* pAnim, std::wstring name, int iFrame)
{
	T::TMatrix mat;
	for (int ibone = 0; ibone < pAnim->m_pTNodeList.size(); ibone++)
	{
		if (pAnim->m_pTNodeList[ibone].szName == name)
		{
			return pAnim->m_pBoneAnimMatrix[ibone][iFrame];
		}
	}
	return mat;
}
bool	TKgcObject::Load(T_STR filename)
{
	m_pdxObj = std::make_shared<TFbxModel>();
	if (TKgcFileFormat::Import(filename, L"../../data/shader/CharacterLightting.hlsl", m_pdxObj))
	{
		return true;
	}
	return true;
}
void	TKgcObject::Release() 
{
	m_pdxObj->Release();
	return;
}