#include "TSound.h"

RUNTIME_IMPLEMENT(TSound)

FMOD::System* TBaseSound::m_pFmodSystem;

void TBaseSound::Init()
{
	if (m_pFmodSystem == nullptr)
	{
		FMOD_RESULT hr = FMOD::System_Create(&m_pFmodSystem);
		if (hr != FMOD_OK) return;
		// 사운드 시스템 초기화
		//  MAXChannel=32 -> 동시에 가능한 일반사운드 갯수
		hr = m_pFmodSystem->init(32, FMOD_INIT_NORMAL, 0);
		if (hr != FMOD_OK) return;
	}
}
void  TBaseSound::Release()
{
	if (m_pFmodSystem)
	{
		m_pFmodSystem->close();
		m_pFmodSystem->release();
	}
	m_pFmodSystem = nullptr;
}


void   TSound::Init(std::wstring name)
{
	m_csName = name;
	
}
bool   TSound::Load( std::wstring filename)
{
	// 1개의 사운드 파일 단위로 1개의 채널이 만들어 진다.
	m_fVolume = 0.5f;  // 0 ~ 1	
	FMOD_RESULT hr = TBaseSound::m_pFmodSystem->createSound(to_wm(filename).c_str(),
		FMOD_DEFAULT, 0, &m_pSound);
	if (hr == FMOD_OK)
	{
		return true;
	}
	return false;
}
void  TSound::PlayEffect()
{
	// 채널 : 실행되는 사운드 제어를 담당한다.
	FMOD::Channel* pChannel = nullptr;
	FMOD_RESULT hr = TBaseSound::m_pFmodSystem->playSound(m_pSound,	nullptr, false, &pChannel);
	if (hr == FMOD_OK)
	{
	}
}
FMOD::Channel* TSound::Play(bool bLoop)
{
	// 채널 : 실행되는 사운드 제어를 담당한다.
	FMOD::Channel* pChannel = nullptr;
	FMOD_RESULT hr = TBaseSound::m_pFmodSystem->playSound(m_pSound,	nullptr, false, &pChannel);

	m_pChannel = pChannel;
	if (hr == FMOD_OK)
	{
		m_pChannel->setVolume(m_fVolume);
		m_pSound->getLength(&m_SizeMS, FMOD_TIMEUNIT_MS);
		_stprintf_s(m_msSound,
			L"전체시간[%02d:%02d:%02d]",
			m_SizeMS / 1000 / 60,
			m_SizeMS / 1000 % 60,
			m_SizeMS / 10 / 60);

		if (bLoop)
		{
			m_pChannel->setMode(FMOD_LOOP_NORMAL); // 무한반복
		}
		else
		{
			m_pChannel->setMode(FMOD_LOOP_OFF); 
		}
	}
	return pChannel;
}
void   TSound::VolumeUp(float fVolume)
{
	if (m_pChannel == nullptr) return;
	float fCurrentVolume;
	m_pChannel->getVolume(&fCurrentVolume);
	m_fVolume = fCurrentVolume + fVolume;
	m_fVolume = max(0.0f, m_fVolume);
	m_fVolume = min(1.0f, m_fVolume);
	m_pChannel->setVolume(m_fVolume);
}
void   TSound::VolumeDown(float fVolume)
{
	if (m_pChannel == nullptr) return;
	float fCurrentVolume;
	m_pChannel->getVolume(&fCurrentVolume);
	m_fVolume = fCurrentVolume - fVolume;
	m_fVolume = max(0.0f, m_fVolume);
	m_fVolume = min(1.0f, m_fVolume);
	m_pChannel->setVolume(m_fVolume);
};
// 일시정지
void   TSound::Paused() // 토클기능 on or off
{
	if (m_pChannel == nullptr) return;

	// 현재 일시정지 여부 파악
	bool bPlay = false;
	m_pChannel->isPlaying(&bPlay);
	if (bPlay == true)
	{
		bool paused;
		m_pChannel->getPaused(&paused);
		m_pChannel->setPaused(!paused);
	}
}
// 중단
void   TSound::Stop()
{
	if (m_pChannel!=nullptr)
	{
		m_pChannel->stop();
	}
}
void   TSound::Frame()
{
	if (m_pChannel == nullptr) return;
	unsigned int ms;
	m_pChannel->getPosition(&ms, FMOD_TIMEUNIT_MS);
	_stprintf_s(m_msSound,
		L"전체시간[%02d:%02d:%02d]\n",
		ms / 1000 / 60,
		ms / 1000 % 60,
		ms / 10 / 60);
	//OutputDebugString(m_msSound);
}
void   TSound::Render()
{
}
void   TSound::Release()
{
	if (m_pSound)
	{
		Stop();
		m_pSound->release();
	}
	m_pSound = nullptr;
	
}