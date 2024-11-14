#include "TFbxObject.h"
//struct tText
//{
//	int kk[100];
//};
//tText data[200];
//fwrite(data, sizeof(tText)*20, 1, fp);
//fread(data, sizeof(tText) * 20, 1, fp);

bool  TKgcFileFormat::Export(TKgcFileFormat* tFile, std::wstring szFileName)
{
	FILE* fp = nullptr;
	errno_t err = _wfopen_s(&fp, szFileName.c_str(), L"wb");
	if (err != 0) return false;

	TKgcFileHeader fileHeader = tFile->m_Header;
	fileHeader.iLength = tFile->m_szFileName.size(); // ���ϸ� ũ��
	fileHeader.iChildNodeCounter = tFile->m_ChildList.size();
	fileHeader.iVersion = 100;
	fileHeader.matWorld = tFile->m_matWorld;
	fileHeader.iNumTrack = tFile->m_pAnimationMatrix.size();
	fileHeader.iStartFrame = fileHeader.iStartFrame;
	fileHeader.iLastFrame = fileHeader.iLastFrame;
	fileHeader.iFrameSpeed = fileHeader.iFrameSpeed;

	fwrite(&fileHeader, sizeof(TKgcFileHeader), 1, fp);
	fwrite(tFile->m_szFileName.c_str(), sizeof(wchar_t), fileHeader.iLength, fp);

	for (auto mesh : tFile->m_ChildList)
	{
		TKgcFileHeader header;
		header.iLength = mesh->m_szTexFileList.size(); // �ؽ�ó ����
		header.iChildNodeCounter = mesh->m_ChildList.size();
		header.matWorld = mesh->m_matWorld;
		header.isSubMesh = (mesh->m_vSubMeshVertexList.size() > 0) ? 1 : 0;
		header.iSubVertexBufferCounter = mesh->m_vSubMeshVertexList.size();
		header.iSubIndexBufferCounter = mesh->m_vSubMeshIndexList.size();
		header.iSubIWVertexBufferCounter = mesh->m_vSubMeshIWVertexList.size();
		header.iNumTrack = mesh->m_pAnimationMatrix.size();
		header.iStartFrame = fileHeader.iStartFrame;
		header.iLastFrame  = fileHeader.iLastFrame;
		header.iFrameSpeed = fileHeader.iFrameSpeed;
		fwrite(&header, sizeof(TKgcFileHeader), 1, fp);

		// animation
		//fwrite(&mesh->m_pAnimationMatrix, sizeof(T::TMatrix), header.iNumTrack, fp);
		for (int iFrame = 0; iFrame < header.iNumTrack; iFrame++)
		{
			fwrite(&mesh->m_pAnimationMatrix[iFrame], sizeof(T::TMatrix), 1, fp);
		}

		for (auto tex : mesh->m_szTexFileList)
		{
			TTexFileHeader texHead;
			texHead.iLength = tex.size();
			fwrite(&texHead, sizeof(TTexFileHeader), 1, fp);			
			fwrite(tex.c_str(), sizeof(wchar_t), texHead.iLength, fp);
		}
		if (header.isSubMesh > 0)
		{
			for (auto sub : mesh->m_vSubMeshVertexList)
			{
				int iSize = sub.size();
				fwrite(&iSize, sizeof(int), 1, fp);
				if (iSize <= 0) continue;
				fwrite(&sub.at(0), sizeof(PNCT_Vertex), sub.size(), fp);
			}
			for (auto sub : mesh->m_vSubMeshIndexList)
			{
				int iSize = sub.size();
				fwrite(&iSize, sizeof(int), 1, fp);
				if (iSize <= 0) continue;
				fwrite(&sub.at(0), sizeof(DWORD), sub.size(), fp);
			}
			for (auto sub : mesh->m_vSubMeshIWVertexList)
			{
				int iSize = sub.size();
				fwrite(&iSize, sizeof(int), 1, fp);
				if (iSize <= 0) continue;
				fwrite(&sub.at(0), sizeof(IW_Vertex), sub.size(), fp);
			}
		}
		else
		{
			int iLen = mesh->m_vVertexList.size();
			fwrite(&iLen, sizeof(int), 1, fp);
			if (iLen != 0)
			{
				fwrite(&mesh->m_vVertexList.at(0), sizeof(PNCT_Vertex), mesh->m_vVertexList.size(), fp);
			}

			iLen = mesh->m_vIndexList.size();
			fwrite(&iLen, sizeof(int), 1, fp);
			if (iLen != 0)
			{
				fwrite(&mesh->m_vIndexList.at(0), sizeof(DWORD), mesh->m_vIndexList.size(), fp);
			}
			// index + weight
			iLen = mesh->m_vIWVertexList.size();
			fwrite(&iLen, sizeof(int), 1, fp);
			if (iLen != 0)
			{
				fwrite(&mesh->m_vIWVertexList.at(0), sizeof(IW_Vertex), 
											mesh->m_vIWVertexList.size(), fp);
			}
		}
	}
	err = fclose(fp);
	if (err != 0) return false;
	_fcloseall();
	return true;
}
bool  TKgcFileFormat::Import(std::wstring szFileName, std::wstring szShaderFile,
	std::shared_ptr<TFbxModel>& tFbxModel )
{
	FILE* fp = nullptr;
	errno_t err = _wfopen_s(&fp, szFileName.c_str(), L"rb");
	if (err != 0) return false;

	int iLen;
	TKgcFileHeader fileHeader;
	fread(&fileHeader, sizeof(TKgcFileHeader), 1, fp);
	WCHAR szLoadfilename[256] = { 0, };
	fread(szLoadfilename, sizeof(wchar_t), fileHeader.iLength, fp);
	tFbxModel->m_FileHeader = fileHeader;

	for (int iMesh =0; iMesh < fileHeader.iChildNodeCounter; iMesh++)
	{
		std::shared_ptr<TFbxModel>  fbxModel = std::make_shared<TFbxModel>();

		TKgcFileHeader header;
		fread(&header, sizeof(TKgcFileHeader), 1, fp);
		fbxModel->m_FileHeader = header;

		// animation
		fbxModel->m_pAnimationMatrix.resize(header.iNumTrack);
		for (int iFrame = 0; iFrame < header.iNumTrack; iFrame++)
		{
			fread(&fbxModel->m_pAnimationMatrix[iFrame], sizeof(T::TMatrix), 1, fp);

		}

		//fbxModel->m_pAnimationMatrix.resize(header.iNumTrack);
		//fread(&fbxModel->m_pAnimationMatrix, sizeof(T::TMatrix), header.iNumTrack, fp);

		fbxModel->m_matWorld = header.matWorld;
		fbxModel->m_matInitWorld = header.matWorld;

		fbxModel->m_szTexFileList.resize(header.iLength);
		for (int iTex = 0; iTex < header.iLength; iTex++)
		{
			TTexFileHeader texHead;			
			fread(&texHead, sizeof(TTexFileHeader), 1, fp);
			wchar_t  szTexName[256] = { 0, };
			fread(szTexName, sizeof(wchar_t), texHead.iLength, fp);
			fbxModel->m_szTexFileList[iTex] = szTexName;
		}
		if (header.isSubMesh > 0)
		{			
			for (int iVertex = 0; iVertex < header.iSubVertexBufferCounter; iVertex++)
			{			
				int iSize = 0;
				fread(&iSize, sizeof(int), 1, fp);	
				if (iSize <= 0) continue;
				vList  subVertexList(iSize);				
				fread(&subVertexList.at(0), sizeof(PNCT_Vertex), iSize, fp);
				fbxModel->m_vSubMeshVertexList.emplace_back(subVertexList);
			}
			
			
			for (int iIndex=0; iIndex < header.iSubIndexBufferCounter; iIndex++)
			{
				int iSize = 0;
				fread(&iSize, sizeof(int), 1, fp);		
				if (iSize <= 0) continue;
				iList  subIndexList(iSize);
				fread(&subIndexList.at(0), sizeof(DWORD), iSize, fp);
				fbxModel->m_vSubMeshIndexList.emplace_back(subIndexList);
			}
			// skinning
			for (int iIndex = 0; iIndex < header.iSubIWVertexBufferCounter; iIndex++)
			{
				int iSize = 0;
				fread(&iSize, sizeof(int), 1, fp);
				if (iSize <= 0) continue;
				iwList  subIW(iSize);
				fread(&subIW.at(0), sizeof(IW_Vertex), iSize, fp);
				fbxModel->m_vSubMeshIWVertexList.emplace_back(subIW);
			}
		}
		else
		{
			int iLen = 0;
			fread(&iLen, sizeof(int), 1, fp);
			if (iLen != 0)
			{
				fbxModel->m_vVertexList.resize(iLen);
				fread(&fbxModel->m_vVertexList.at(0), sizeof(PNCT_Vertex), 
					iLen, fp);
			}

			fread(&iLen, sizeof(int), 1, fp);
			if (iLen != 0)
			{
				fbxModel->m_vIndexList.resize(iLen);
				fread(&fbxModel->m_vIndexList.at(0), sizeof(DWORD),
					iLen, fp);
			}

			fread(&iLen, sizeof(int), 1, fp);
			if (iLen != 0)
			{
				fbxModel->m_vIWVertexList.resize(iLen);
				fread(&fbxModel->m_vIWVertexList.at(0), sizeof(IW_Vertex),	iLen, fp);
			}
		}

		tFbxModel->m_ChildModel.emplace_back(fbxModel);
	}
	err = fclose(fp);
	if (err != 0) return false;
	_fcloseall();


	for (int iObj = 0; iObj < tFbxModel->m_ChildModel.size(); iObj++)
	{
		auto pFbxMesh = tFbxModel->m_ChildModel[iObj];
		std::wstring name = L"../../data/";
		if (pFbxMesh->m_szTexFileList.size() > 0)
		{
			std::wstring texPath = pFbxMesh->m_szTexFileList[0];
			wchar_t  szDrive[MAX_PATH] = { 0, };
			wchar_t  szDir[MAX_PATH] = { 0, };
			wchar_t  szFileName[MAX_PATH] = { 0, };
			wchar_t  szFileExt[MAX_PATH] = { 0, };
			_tsplitpath_s(texPath.c_str(), szDrive, szDir, szFileName, szFileExt);
			name += szFileName;
			std::wstring ext = szFileExt;

			if (ext == L".tga" || ext == L".TGA")
			{
				name += L".dds";
			}
			else
			{
				name += szFileExt;
			}
		}
		pFbxMesh->Create(name, szShaderFile);
	}

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
	if (m_vIWVertexList.size() == 0) return true;
	// ���� �Ҵ� ũ�⸦ �����Ѵ�.
	D3D11_BUFFER_DESC  bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(IW_Vertex) * m_vIWVertexList.size();
	// ���ῡ �뵵�� ����? 
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;


	// �Ҵ�� ���ۿ� 
	// �ý��۸޸𸮰� ->GPU�޸𸮷� ü������ ������
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_vIWVertexList.at(0);

	HRESULT hr = pd3dDevice->CreateBuffer(
		&bd,
		&sd,
		m_pIWVertexBuffer.GetAddressOf());
	if (FAILED(hr)) return false;
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
void     TFbxModel::Render(ID3D11DeviceContext* pContext)
{
	static float fDirection = 1.0f;	
	float fStartFrame = m_FileHeader.iStartFrame;
	float fEndFrame = m_FileHeader.iLastFrame;
	float FrameSpeed = m_FileHeader.iFrameSpeed;
	m_fFrameAnimation += fDirection * g_fSecondPerFrame * FrameSpeed * 1.f;
	if (m_fFrameAnimation > fEndFrame)
	{
		m_fFrameAnimation = fEndFrame - 1;
		fDirection *= -1.0f;
	}
	if (m_fFrameAnimation < fStartFrame)
	{
		m_fFrameAnimation = fStartFrame;
		fDirection *= -1.0f;
	}

	for (int iChild = 0; iChild < m_ChildModel.size(); iChild++)
	{
		auto pModel = m_ChildModel[iChild];
		pModel->m_matWorld = pModel->m_pAnimationMatrix[m_fFrameAnimation];
		pModel->m_matParentWorld = m_matParentWorld;
		pModel->SetMatrix(&pModel->m_matWorld, &m_matView, &m_matProj);
		
		pModel->PreRender(pContext);
		if (pModel->m_pSubMeshVertexBuffer.size())
		{
			for (int iSubMesh = 0; iSubMesh < pModel->m_pSubMeshVertexBuffer.size(); iSubMesh++)
			{
				UINT StartSlot = 0;
				UINT NumBuffers = 1;
				UINT pStrides[2] = { sizeof(PNCT_Vertex), sizeof(IW_Vertex) }; // 1���� ���� ũ��
				UINT pOffsets[2] = { 0, 0 }; // ���ۿ� ���� �ε���
				ID3D11Buffer* buffer[2] = { pModel->m_pSubMeshVertexBuffer[iSubMesh].Get(),
											pModel->m_pSubMeshIWVertexBuffer[iSubMesh].Get()  };
				NumBuffers = 2;
				pContext->IASetVertexBuffers(0, 2,buffer, pStrides, pOffsets);
				// 
				//pContext->IASetVertexBuffers(StartSlot, NumBuffers,
				//	pModel->m_pSubMeshVertexBuffer[iSubMesh].GetAddressOf(), &pStrides, &pOffsets);
				pContext->IASetIndexBuffer(pModel->m_pSubMeshIndexBuffer[iSubMesh].Get(), DXGI_FORMAT_R32_UINT, 0);
				pContext->PSSetShaderResources(0, 1, pModel->m_pSubMeshSRV[iSubMesh].GetAddressOf());
				if (pModel->m_pSubMeshIndexBuffer[iSubMesh] != nullptr)
				{
					pContext->DrawIndexed(pModel->m_vSubMeshIndexList[iSubMesh].size(), 0, 0);
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
			UINT pStrides[2] = { sizeof(PNCT_Vertex), sizeof(IW_Vertex) }; // 1���� ���� ũ��
			UINT pOffsets[2] = { 0, 0 }; // ���ۿ� ���� �ε���
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
//void   TFbxModel::GenBufferVI(PNCT_Vertex& v)
//{
//	std::vector<PNCT_Vertex>    vertexArray;
//	std::vector<DWORD>			indexArray;
//	if (pGeomModel)
//	{
//		// ������͸��� ��� ����
//		for (int iVertex = 0; iVertex < pGeomModel->m_vVertexList.size(); iVertex++)
//		{
//			PNCT_Vertex v1 = pGeomModel->m_vVertexList[iVertex];
//			// vertexArray�� ��� ������ ��
//			int iVertexIndex = -1;
//			for (int iArray = 0; iArray < vertexArray.size(); iArray++)
//			{
//				PNCT_Vertex v2 = vertexArray[iArray];
//				if (v1 == v2)
//				{
//					iVertexIndex = iArray;
//					break;
//				}
//			}
//			if (iVertexIndex < 0)
//			{
//				vertexArray.emplace_back(v1);
//				iVertexIndex = vertexArray.size() - 1;
//			}
//			indexArray.emplace_back(iVertexIndex);
//		}
//
//		pGeomModel->m_vVertexList.clear();
//		pGeomModel->m_vIndexList = indexArray;
//		pGeomModel->m_vVertexList = vertexArray;
//	}
//}