#include "TTimer.h"
bool  TTimer::Init()
{
	m_dwTickStart = timeGetTime();
	return true;
}
bool  TTimer::Frame()
{
	m_dwTickEnd = timeGetTime();
	DWORD dwElapseTick = m_dwTickEnd - m_dwTickStart;
	
	m_fSecondPerFrame = dwElapseTick / 1000.0f;
	m_fGameTimer += m_fSecondPerFrame;
	
	m_iFPS++;
	m_dwTime += dwElapseTick;
	if (m_dwTime >= 1000)
	{
		m_msg = std::to_wstring(m_iFPS);
		m_msg += L" FPS\n";
		m_dwTime -= 1000;
		m_iFPS = 0;
	}

	m_dwTickStart = m_dwTickEnd;
	return true;
}
bool  TTimer::Render()
{
	std::wstring outmsg = L"게임경과시간:";
	outmsg += std::to_wstring(m_fGameTimer);
	outmsg += L" ";
	outmsg += L"1프레임 경과시간:";
	outmsg += std::to_wstring(m_fSecondPerFrame);
	outmsg += L" ";
	outmsg += L"초당 프레임:";
	outmsg += m_msg;
	outmsg += L"\n";

	OutputDebugString(outmsg.c_str());
	return true;
}
bool  TTimer::Release()
{
	return true;
}
