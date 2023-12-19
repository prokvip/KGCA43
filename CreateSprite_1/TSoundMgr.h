#pragma once
#include "TStd.h"
#include "fmod.hpp"
#include "fmod_errors.h"
#pragma comment(lib, "fmod_vc.lib")

class TSound
{
	FMOD::System* m_pSystem = nullptr;
	FMOD::Sound*  m_pSound = nullptr;
	FMOD::Channel*  m_pChannel = nullptr;
	float  m_fVolume = 0.5f;
public:
	TCHAR  m_szBuffer[256];
public:
	bool Load(FMOD::System* pSystem, std::string loadfilename);
	void Play(bool bLoop=true);
	void PlayEffect();
	void Stop();
	void VolumeUp(float fValue);
	void VolumeDown(float fValue);
	void Paused();
	bool Frame();
	bool Release();
};
class TSoundMgr
{
	FMOD::System* m_pSystem = nullptr;
public:
	static TSoundMgr& Get()
	{
		static TSoundMgr mgr;
		return mgr;
	}
	std::map<std::wstring, std::shared_ptr<TSound>> m_list;
public:
	TSound* Load(std::wstring loadfilename);
	bool   Init();
	bool   Frame();
	bool   Release();
private:
	TSoundMgr();
public:
	~TSoundMgr();
};

