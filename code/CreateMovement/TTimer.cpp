#include "TTimer.h"
double   g_fSecondPerFrame = 0.0f;
void    TTimer::DebugTimer()
{
#ifdef _DEBUG
	TCHAR msgKey[MAX_PATH] = { 0, };
	_stprintf_s(msgKey,
		L"FPS=%ld, GameTimer=%10.4f SPF=%d\n",
		m_dwFPS, m_fGameTimeTick,
		m_dwSecondPerFrame);
	OutputDebugString(msgKey);
#endif
}
void    TTimer::Init()
{
	m_dwPreTimeTick = timeGetTime();
	m_dwCurrentTimeTick = 0.0f;
	m_fGameTimeTick = 0.0f;
	m_dwFPS = 0;
}
void    TTimer::Frame()
{
	// 이전시간
	// 현재시간
	m_dwCurrentTimeTick = timeGetTime();
	// 경과시간(초단위)= 현재시간 - 이전시간
	m_dwSecondPerFrame = m_dwCurrentTimeTick - m_dwPreTimeTick;
	m_fSecondPerFrame = (float)m_dwSecondPerFrame / 1000.0f;
	m_fGameTimeTick += m_fSecondPerFrame;

	g_fSecondPerFrame = m_fSecondPerFrame;

	fSecondTime += m_fSecondPerFrame;
	if (fSecondTime >= 1.0f) // 1.01f
	{
		m_dwFPS = iFPS;
		fSecondTime = fSecondTime - 1.0f;
		iFPS = 0;
	}
	iFPS++;

	m_dwPreTimeTick = m_dwCurrentTimeTick;


	TCHAR msgKey[MAX_PATH] = { 0, };
	_stprintf_s(msgKey,
		L"FPS=%ld, GameTimer=%10.4f SPF=%d\n",
		m_dwFPS, m_fGameTimeTick,
		m_dwSecondPerFrame);
	m_csBuffer = msgKey;
}