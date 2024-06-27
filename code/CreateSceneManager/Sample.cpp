#include "Sample.h"

void   Sample::Init()
{
	m_Game.Init();	
}

void    Sample::Frame()
{	
	m_Game.Frame();
}
void    Sample::Render() 
{ 		
	m_Game.Render(TDevice::m_pContext);
}
void    Sample::Release() 
{			
	m_Game.Release();
}

T_GAME_START(800, 600);