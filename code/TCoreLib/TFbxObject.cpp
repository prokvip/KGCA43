#include "TFbxObject.h"
bool     TFbxModel::RebuildAnimation(TFbxModel* pModel)
{
	if (pModel != nullptr)
	{
		// mesh
		auto pMeshObj = pModel->m_pTNodeList;
		auto pAnimObj = m_pTNodeList;
		using boneFrameMatrix = std::vector<T::TMatrix>;
		std::vector<boneFrameMatrix> m_pNewBoneAnimMatrix;
		std::vector<std::vector<TTrack>>    m_pNewAnimTrack;

		m_pNewBoneAnimMatrix.resize(pMeshObj.size());
		m_pNewAnimTrack.resize(pMeshObj.size());

		UINT iNumNodes = pModel->m_FileHeader.iNumNodeCounter;
		UINT iLastFrame = m_FileHeader.iLastFrame;

		boneFrameMatrix matNewDummyMatrix(iLastFrame);
		std::vector<TTrack> matNewDummyTrack(iLastFrame);
		for (int ibone = 0; ibone < pMeshObj.size(); ibone++)
		{
			m_pNewBoneAnimMatrix[ibone].resize(iLastFrame);
			m_pNewAnimTrack[ibone].resize(iLastFrame);

			int anim = 0;
			std::wstring name1 = pMeshObj[ibone].szName;
			for (anim = 0; anim < pAnimObj.size(); anim++)
			{
				std::wstring name2 = pAnimObj[anim].szName;
				if (name1 == name2)
				{
					m_pNewBoneAnimMatrix[ibone] = m_pBoneAnimMatrix[anim];
					m_pNewAnimTrack[ibone] = m_matAnimTrack[anim];
					break;
				}
			}
			if (anim >= pAnimObj.size())
			{
				m_pNewBoneAnimMatrix[ibone] = matNewDummyMatrix;
				m_pNewAnimTrack[ibone] = matNewDummyTrack;
			}
		}
		for (int anim = 0; anim < pAnimObj.size(); anim++)
		{
			m_pBoneAnimMatrix[anim].clear();
			m_matAnimTrack[anim].clear();
		}
		m_pBoneAnimMatrix.clear();
		m_matAnimTrack.clear();

		/*m_pBoneAnimMatrix.resize(iNumNodes);
		m_matAnimTrack.resize(iNumNodes);
		for (int node = 0; node < iNumNodes; node++)
		{
			m_pBoneAnimMatrix[node].resize(iLastFrame);
			m_matAnimTrack[node].resize(iLastFrame);
			for (int iFrame = 0; iFrame < iLastFrame; iFrame++)
			{
				m_pBoneAnimMatrix[node][iFrame] = m_pNewBoneAnimMatrix[node][iFrame];
				m_matAnimTrack[node][iFrame] = m_matAnimTrack[node][iFrame];
			}
		}*/
		m_pBoneAnimMatrix = m_pNewBoneAnimMatrix;
		m_matAnimTrack = m_pNewAnimTrack;
	}
	return true;
}
bool	 TFbxModel::CreateBoneSRV(ID3D11Device* pd3dDevice)
{
	D3D11_BUFFER_DESC vbdesc =
	{
		MAX_BONE_MATRICES * sizeof(TMatrix),
		D3D11_USAGE_DYNAMIC,
		D3D11_BIND_SHADER_RESOURCE,
		D3D11_CPU_ACCESS_WRITE,
		0
	};
	pd3dDevice->CreateBuffer(&vbdesc, NULL, m_pBoneBuffer.GetAddressOf());

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	// Again, we need a resource view to use it in the shader
	ZeroMemory(&SRVDesc, sizeof(SRVDesc));
	SRVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	SRVDesc.Buffer.ElementOffset = 0;
	SRVDesc.Buffer.ElementWidth = MAX_BONE_MATRICES * 4;
	pd3dDevice->CreateShaderResourceView(
		m_pBoneBuffer.Get(), &SRVDesc, m_pBoneBufferRV.GetAddressOf());

	return true;
}
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
			TLoadData ld;
			ld.m_csLoadFileName			= name;
			ld.m_csLoadShaderFileName   = name;
			m_pSubMeshTexture[iMesh] = I_Tex.Load(ld).get();
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
		ID3D11ShaderResourceView* srvArray[] = { pModel->m_pBoneBufferRV.Get() };
		pContext->VSSetShaderResources(1, 1, srvArray);

		if (pModel->m_pSubMeshVertexBuffer.size())
		{
			for (int iSubMesh = 0; iSubMesh < pModel->m_pSubMeshVertexBuffer.size(); iSubMesh++)
			{
				UINT StartSlot = 0;
				UINT NumBuffers = 1;
				UINT pStrides[3] = { sizeof(PNCT_Vertex), sizeof(IW_Vertex), sizeof(TInstance) }; // 1개의 정점 크기
				UINT pOffsets[3] = { 0, 0, 0 }; // 버퍼에 시작 인덱스
				if (pModel->m_pInstanceBuffer != nullptr)
				{					
					for (int iInstance = 0; iInstance < pModel->m_InstanceData.size();
						iInstance++)
					{
						pModel->m_InstanceData[iInstance].matInstance._42 = 
							iInstance * 50;
					}
					pContext->UpdateSubresource(
						pModel->m_pInstanceBuffer.Get(), 0, NULL,
						&pModel->m_InstanceData.at(0), 0,0 );

					ID3D11Buffer* buffer[3] = { pModel->m_pSubMeshVertexBuffer[iSubMesh].Get(),
												pModel->m_pSubMeshIWVertexBuffer[iSubMesh].Get(),
												pModel->m_pInstanceBuffer.Get() };
					NumBuffers = 3;
					pContext->IASetVertexBuffers(0, NumBuffers, buffer, pStrides, pOffsets);
				}
				else
				{
					ID3D11Buffer* buffer[2] = { pModel->m_pSubMeshVertexBuffer[iSubMesh].Get(),
												pModel->m_pSubMeshIWVertexBuffer[iSubMesh].Get()};
					NumBuffers = 2;
					pContext->IASetVertexBuffers(0, NumBuffers, buffer, pStrides, pOffsets);
				}
				// 
				//pContext->IASetVertexBuffers(StartSlot, NumBuffers,
				//	pModel->m_pSubMeshVertexBuffer[iSubMesh].GetAddressOf(), &pStrides, &pOffsets);
				
				pContext->PSSetShaderResources(0, 1, pModel->m_pSubMeshSRV[iSubMesh].GetAddressOf());
				if (pModel->m_pSubMeshIndexBuffer.size())
				{
					pContext->IASetIndexBuffer(pModel->m_pSubMeshIndexBuffer[iSubMesh].Get(), DXGI_FORMAT_R32_UINT, 0);

					if (pModel->m_pSubMeshIndexBuffer[iSubMesh] != nullptr)
					{
						if (pModel->m_pInstanceBuffer == nullptr)
						{
							pContext->DrawIndexed(pModel->m_vSubMeshIndexList[iSubMesh].size(), 0, 0);
						}
						else
						{
							pContext->DrawIndexedInstanced(
								pModel->m_vSubMeshIndexList[iSubMesh].size(),
								pModel->m_InstanceData.size(), 0, 0, 0);
						}
					}
				}
				else
				{
					if (pModel->m_pInstanceBuffer == nullptr)
					{
						pContext->Draw(pModel->m_vSubMeshVertexList[iSubMesh].size(), 0);
					}
					else
					{
						pContext->DrawInstanced(
							pModel->m_vSubMeshVertexList[iSubMesh].size(),
							pModel->m_InstanceData.size(), 0, 0);
					}
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
void     TFbxModel::RenderInstance(ID3D11Buffer* pInstanceBuffer, int iNumInstance)
{
	auto pContext = TDevice::m_pContext;
	for (int iChild = 0; iChild < m_ChildModel.size(); iChild++)
	{
		auto pModel = m_ChildModel[iChild];

		//pModel->m_matWorld = pModel->m_pAnimationMatrix[m_fFrameAnimation];
		//pModel->m_matWorld = pModel->m_pAnimationMatrix[m_fFrameAnimation];		
		D3DXMatrixIdentity(&pModel->m_matWorld);
		pModel->m_matParentWorld = m_matParentWorld;
		pModel->SetMatrix(nullptr, &m_matView, &m_matProj);

		pModel->PreRender(TDevice::m_pContext);

		
		// skinning
		pContext->VSSetConstantBuffers(2, 1, pModel->m_pBoneCB.GetAddressOf());
		ID3D11ShaderResourceView* srvArray[] = { pModel->m_pBoneBufferRV.Get() };
		pContext->VSSetShaderResources(1, 1, srvArray);

		if (pModel->m_pSubMeshVertexBuffer.size())
		{
			for (int iSubMesh = 0; iSubMesh < pModel->m_pSubMeshVertexBuffer.size(); iSubMesh++)
			{
				UINT StartSlot = 0;
				UINT NumBuffers = 1;
				UINT pStrides[3] = { sizeof(PNCT_Vertex), sizeof(IW_Vertex), sizeof(TInstance) }; // 1개의 정점 크기
				UINT pOffsets[3] = { 0, 0, 0 }; // 버퍼에 시작 인덱스
				if (pInstanceBuffer != nullptr)
				{					
					ID3D11Buffer* buffer[3] = { pModel->m_pSubMeshVertexBuffer[iSubMesh].Get(),
												pModel->m_pSubMeshIWVertexBuffer[iSubMesh].Get(),
												pInstanceBuffer };
					NumBuffers = 3;
					pContext->IASetVertexBuffers(0, NumBuffers, buffer, pStrides, pOffsets);
				}
				else
				{
					ID3D11Buffer* buffer[2] = { pModel->m_pSubMeshVertexBuffer[iSubMesh].Get(),
												pModel->m_pSubMeshIWVertexBuffer[iSubMesh].Get() };
					NumBuffers = 2;
					pContext->IASetVertexBuffers(0, NumBuffers, buffer, pStrides, pOffsets);
				}
				// 
				//pContext->IASetVertexBuffers(StartSlot, NumBuffers,
				//	pModel->m_pSubMeshVertexBuffer[iSubMesh].GetAddressOf(), &pStrides, &pOffsets);

				pContext->PSSetShaderResources(0, 1, pModel->m_pSubMeshSRV[iSubMesh].GetAddressOf());
				if (pModel->m_pSubMeshIndexBuffer.size())
				{
					pContext->IASetIndexBuffer(pModel->m_pSubMeshIndexBuffer[iSubMesh].Get(), DXGI_FORMAT_R32_UINT, 0);

					if (pModel->m_pSubMeshIndexBuffer[iSubMesh] != nullptr)
					{
						if (pInstanceBuffer == nullptr)
						{
							pContext->DrawIndexed(pModel->m_vSubMeshIndexList[iSubMesh].size(), 0, 0);
						}
						else
						{
							pContext->DrawIndexedInstanced(
								pModel->m_vSubMeshIndexList[iSubMesh].size(),
								pModel->m_InstanceData.size(), 0, 0, 0);
						}
					}
				}
				else
				{
					if (pInstanceBuffer == nullptr)
					{
						pContext->Draw(pModel->m_vSubMeshVertexList[iSubMesh].size(), 0);
					}
					else
					{
						pContext->DrawInstanced(
							pModel->m_vSubMeshVertexList[iSubMesh].size(),
							iNumInstance, 0, 0);
					}
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

		{"TEXCOORD",1,	DXGI_FORMAT_R32G32B32A32_FLOAT,		1,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{"TEXCOORD",2,	DXGI_FORMAT_R32G32B32A32_FLOAT,		1,16,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{"TEXCOORD",3,	DXGI_FORMAT_R32G32B32A32_FLOAT,		1,32,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{"TEXCOORD",4,	DXGI_FORMAT_R32G32B32A32_FLOAT,		1,48,D3D11_INPUT_PER_VERTEX_DATA,0 },

		{"TRANSFORM",0,	DXGI_FORMAT_R32G32B32A32_FLOAT,		2,0,D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{"TRANSFORM",1,	DXGI_FORMAT_R32G32B32A32_FLOAT,		2,16,D3D11_INPUT_PER_INSTANCE_DATA,1 },
		{"TRANSFORM",2,	DXGI_FORMAT_R32G32B32A32_FLOAT,		2,32,D3D11_INPUT_PER_INSTANCE_DATA,1 },
		{"TRANSFORM",3,	DXGI_FORMAT_R32G32B32A32_FLOAT,		2,48,D3D11_INPUT_PER_INSTANCE_DATA,1 },

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
	CreateBoneSRV(pd3dDevice);
	m_pBoneCB.Attach(DX::CreateVertexBuffer(TDevice::m_pd3dDevice.Get(),&m_matBoneList,	1, sizeof(TBoneMatrix)));
	//// 버퍼 할당 크기를 세팅한다.
	//D3D11_BUFFER_DESC  bd;
	//ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	//bd.ByteWidth = sizeof(TBoneMatrix);
	//bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;


	//// 할당된 버퍼에 
	//// 시스템메모리가 ->GPU메모리로 체워지는 데이터
	//D3D11_SUBRESOURCE_DATA sd;
	//ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	//sd.pSysMem = &m_matBoneList;

	//HRESULT hr = pd3dDevice->CreateBuffer(
	//	&bd,
	//	&sd,
	//	m_pBoneCB.GetAddressOf());
	//if (FAILED(hr)) return false;


	// Instance buffer
	//m_InstanceData.resize(5);
	//for (int i = 0; i < m_InstanceData.size(); i++)
	//{
	//	T::TMatrix mat;
	//	mat._41 = i * 100.0f;
	//	m_InstanceData[i].matInstance = mat;
	//}	
	//m_pInstanceBuffer.Attach(
	//	DX::CreateVertexBuffer(TDevice::m_pd3dDevice.Get(),
	//		&m_InstanceData.at(0),
	//		m_InstanceData.size(), sizeof(TInstance)));
	return true;
}
void	 TFbxModel::Frame() {
	
} 
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
			T::TVector3 v, s;
			T::TQuaternion q;
			int iBegin = fAnimFrame; // 3.5
			int iEnd = iBegin + 1;   // 4

			float fLerpTime = fAnimFrame - pAnim->m_matAnimTrack[iBone][iBegin].fTime;
			D3DXVec3Lerp(&v, &pAnim->m_matAnimTrack[iBone][iBegin].v, &pAnim->m_matAnimTrack[iBone][iEnd].v, fLerpTime);
			D3DXVec3Lerp(&s, &pAnim->m_matAnimTrack[iBone][iBegin].s, &pAnim->m_matAnimTrack[iBone][iEnd].s, fLerpTime);
			TQuaternion q1 = pAnim->m_matAnimTrack[iBone][iBegin].q;
			TQuaternion q2 = pAnim->m_matAnimTrack[iBone][iEnd].q;
			D3DXQuaternionSlerp(&q, &q1, &q2, fLerpTime);

			T::TMatrix matScale, matTrans, matRotate;
			D3DXMatrixScaling(&matScale, s.x, s.y, s.z);
			D3DXMatrixTranslation(&matTrans, v.x, v.y, v.z);
			D3DXMatrixRotationQuaternion(&matRotate, &q);

			T::TMatrix matFinalAnim = pModel->m_matBindPose[iBone] * (matScale * matRotate * matTrans);
			D3DXMatrixTranspose(&pModel->m_matBoneList.matBone[iBone], &matFinalAnim);

			//matAnim = pModel->m_matBindPose[iBone] * pAnim->m_pBoneAnimMatrix[iBone][fAnimFrame];
			//D3DXMatrixTranspose(&pModel->m_matBoneList.matBone[iBone], &matAnim);
		}
		TDevice::m_pContext->UpdateSubresource(pModel->m_pBoneCB.Get(), 0, NULL, &pModel->m_matBoneList, 0, 0);
	}
	SetBoneMatrices(TDevice::m_pContext, fAnimFrame, pAnim);
}
void TFbxModel::SetBoneMatrices(ID3D11DeviceContext* pContext, float& fAnimFrame, TFbxModel* pAnim)
{
	T::TMatrix* pMatrices;
	T::TMatrix  matAnim;
	HRESULT hr = S_OK;
	for (int iChild = 0; iChild < m_ChildModel.size(); iChild++)
	{
		auto pModel = m_ChildModel[iChild];
		D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
		if (SUCCEEDED(pContext->Map(
			(ID3D11Resource*)pModel->m_pBoneBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD,
			0, &MappedFaceDest)))
		{
			pMatrices = (TMatrix*)MappedFaceDest.pData;
			for (int iBone = 0; iBone < pModel->m_matBindPose.size(); iBone++)
			{
				matAnim = pModel->m_matBindPose[iBone] * pAnim->m_pBoneAnimMatrix[iBone][fAnimFrame];			
				pMatrices[iBone] = matAnim;
			}
			pContext->Unmap(pModel->m_pBoneBuffer.Get(), 0);
		}
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
bool	TKgcObject::Load(TLoadData ld)
{
	m_pdxObj = std::make_shared<TFbxModel>();
	if (TKgcFileFormat::Import(ld.m_csLoadFileName, ld.m_csLoadShaderFileName, m_pdxObj))
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