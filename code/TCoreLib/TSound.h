#pragma once
#include "TBaseManager.h"

// https://www.fmod.com  fmod���̺귯��
// �ν��� : C:\Program Files (x86)\FMOD SoundSystem\FMOD Studio API Windows\api\core
// inc, lib �����ؼ� �ַ��� ��� fmod ���� ���� �� �����Ѵ�.
// ������Ʈ �Ӽ�����
//    c++ -> ../../fmod/inc
//    ��Ŀ  -> ../../fmod/lib/x64

//    ../../fmod/lib/x64  �������� fmod.dll -> output ������ �����Ѵ�.

#include "fmod.hpp"
#include "fmod_errors.h"
#pragma comment(lib, "fmod_vc.lib")
//1 > LINK : fatal error LNK1104 : 'fmod_vc.lib' ������ �� �� �����ϴ�.

class TSound : public TResource
{
private:
	FMOD::System*  m_pFmodSystem = nullptr;
public:	
	void           SetFmod(FMOD::System* pSystem)
	{
		m_pFmodSystem = pSystem;
	}
	FMOD::Sound*   m_pSound = nullptr;
	FMOD::Channel* m_pChannel = nullptr;
	float		   m_fVolume = 1.0f;
	unsigned int	m_SizeMS;
	wchar_t			m_msSound[MAX_PATH] = { 0, };
	wchar_t			m_msPlay[MAX_PATH] = { 0, };
public:	
	bool   Load(TLoadData ld);
	FMOD::Channel*  Play(bool bLoop=false);
	void   PlayEffect();
	void   Stop();
	void   Paused();
	void   VolumeUp(float fVolume);
	void   VolumeDown(float fVolume);
public:
	void   Init();
	void   Frame();
	void   Render();
	void   Release();
};

class TSoundMgr : public TBaseManager<TSound, TSoundMgr>
{
	friend class TSingleton<TSoundMgr>;
public:
	FMOD::System* m_pFmodSystem = nullptr;
	void   Frame() { 
		m_pFmodSystem->update(); 
		//if (I_Input.KeyCheck(VK_PRIOR) == KEY_PUSH)
		//{
		//	if (m_pEffectSound)
		//	{
		//		m_pEffectSound->PlayEffect();
		//	}
		//}

		//if (I_Input.KeyCheck(VK_HOME) == KEY_PUSH)
		//{
		//	if (m_pBGSound)
		//	{
		//		m_pBGSound->Paused();
		//	}
		//}
		//if (I_Input.KeyCheck(VK_INSERT) == KEY_HOLD)
		//{
		//	if (m_pBGSound)
		//	{
		//		m_pBGSound->VolumeUp(g_fSecondPerFrame * 0.3f);
		//	}
		//	//m_Sound.VolumeUp(g_fSecondPerFrame*0.3f);
		//}
		//if (I_Input.KeyCheck(VK_DELETE) == KEY_HOLD)
		//{
		//	if (m_pBGSound)
		//	{
		//		m_pBGSound->VolumeDown(g_fSecondPerFrame * 0.3f);
		//	}
		//	//m_Sound.VolumeDown(g_fSecondPerFrame * 0.3f);
		//}
	};
	void   Render() {};
	void   Release() {	
		TBaseManager<TSound, TSoundMgr>::Release();
		if (m_pFmodSystem)
		{
			m_pFmodSystem->close();
			m_pFmodSystem->release();
		}
		m_pFmodSystem = nullptr;
	};
private:
	TSoundMgr() {
		FMOD_RESULT hr = FMOD::System_Create(&m_pFmodSystem);
		if (hr != FMOD_OK) return;
		// ���� �ý��� �ʱ�ȭ
		//  MAXChannel=32 -> ���ÿ� ������ �Ϲݻ��� ����
		hr = m_pFmodSystem->init(32, FMOD_INIT_NORMAL, 0);
		if (hr != FMOD_OK) return;
	}
	~TSoundMgr()
	{				
	}

	std::shared_ptr<TSound>  CreateObject(std::wstring path, std::wstring name)
	{
		std::shared_ptr<TSound>  pData = std::make_shared<TSound>();
		pData->SetFmod(m_pFmodSystem);
		pData->m_csName = name;
		TLoadData ld;
		ld.m_csLoadFileName = path;
		ld.m_csLoadShaderFileName = path;
		if (pData->Load(ld) == false)
		{
			return nullptr;
		}
		m_list.insert(std::make_pair(pData->m_csName, pData));
		return pData;
	}
};
#define I_Sound TSoundMgr::Get()

