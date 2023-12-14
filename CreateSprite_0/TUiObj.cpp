#include "TUiObj.h"

//bool TUiNumber::LoadTexture(std::wstring texFileName)
//{   
//    for (int i = 0; i < 10; i++)
//    {
//        std::wstring filename = L"../../data/";
//        filename += std::to_wstring(i);
//        filename += L".png";
//        m_texArray[i] = std::make_unique<DirectX::ScratchImage>();
//        DirectX::TexMetadata metadata;
//        HRESULT hr = DirectX::GetMetadataFromWICFile(filename.c_str(), DirectX::WIC_FLAGS_NONE, metadata);
//        if (SUCCEEDED(hr))
//        {
//            hr = DirectX::LoadFromWICFile(filename.c_str(), DirectX::WIC_FLAGS_NONE, &metadata, *m_texArray[i]);
//            if (SUCCEEDED(hr))
//            {
//                hr = DirectX::CreateShaderResourceView(m_pd3dDevice,
//                    m_texArray[i]->GetImages(), //_In_reads_(nimages) const Image * srcImages, 
//                    m_texArray[i]->GetImageCount(), // _In_ size_t nimages, 
//                    metadata,// _In_ const TexMetadata & metadata,
//                    &m_pTextureSRVArray[i]);//_Outptr_ ID3D11ShaderResourceView * *ppSRV
//                if (SUCCEEDED(hr))
//                {
//                    continue;
//                }
//            }
//        }
//        //DDS 다이렉트SDK 전용 압축포맷
//        hr = DirectX::GetMetadataFromDDSFile(filename.c_str(), DirectX::DDS_FLAGS_NONE, metadata);
//        if (SUCCEEDED(hr))
//        {
//            hr = DirectX::LoadFromDDSFile(filename.c_str(), DirectX::DDS_FLAGS_NONE, &metadata, *m_texArray[i]);
//            if (SUCCEEDED(hr))
//            {
//                hr = DirectX::CreateShaderResourceView(m_pd3dDevice,
//                    m_texArray[i]->GetImages(), //_In_reads_(nimages) const Image * srcImages, 
//                    m_texArray[i]->GetImageCount(), // _In_ size_t nimages, 
//                    metadata,// _In_ const TexMetadata & metadata,
//                    &m_pTextureSRVArray[i]);//_Outptr_ ID3D11ShaderResourceView * *ppSRV
//                if (SUCCEEDED(hr))
//                {
//                    continue;
//                }
//            }
//        }
//        hr = DirectX::GetMetadataFromTGAFile(filename.c_str(), DirectX::TGA_FLAGS_NONE, metadata);
//        if (SUCCEEDED(hr))
//        {
//            hr = DirectX::LoadFromTGAFile(filename.c_str(), DirectX::TGA_FLAGS_NONE, &metadata, *m_texArray[i]);
//            if (SUCCEEDED(hr))
//            {
//                hr = DirectX::CreateShaderResourceView(m_pd3dDevice,
//                    m_texArray[i]->GetImages(), //_In_reads_(nimages) const Image * srcImages, 
//                    m_texArray[i]->GetImageCount(), // _In_ size_t nimages, 
//                    metadata,// _In_ const TexMetadata & metadata,
//                    &m_pTextureSRVArray[i]);//_Outptr_ ID3D11ShaderResourceView * *ppSRV
//                if (SUCCEEDED(hr))
//                {
//                    continue;
//                }
//            }
//        }
//
//        hr = DirectX::GetMetadataFromHDRFile(filename.c_str(), metadata);
//        if (SUCCEEDED(hr))
//        {
//            hr = DirectX::LoadFromHDRFile(filename.c_str(), &metadata, *m_texArray[i]);
//            if (SUCCEEDED(hr))
//            {
//                hr = DirectX::CreateShaderResourceView(m_pd3dDevice,
//                    m_texArray[i]->GetImages(), //_In_reads_(nimages) const Image * srcImages, 
//                    m_texArray[i]->GetImageCount(), // _In_ size_t nimages, 
//                    metadata,// _In_ const TexMetadata & metadata,
//                    &m_pTextureSRVArray[i]);//_Outptr_ ID3D11ShaderResourceView * *ppSRV
//                if (SUCCEEDED(hr))
//                {
//                    continue;
//                }
//            }
//        }
//    }
//    return true;
//}