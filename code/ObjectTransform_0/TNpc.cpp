#include "TNpc.h"
void    TNpc::Frame() 
{
	if (m_vPos.X > g_xClientSize + 300)
	{
		m_vDirection.X = -1.0f;
	}
	if (m_vPos.X < 0 - 300)
	{
		m_vDirection.X = 1.0f;
	}
	if (m_vPos.Y > g_yClientSize + 300)
	{
		m_vDirection.Y = -1.0f;
	}
	if (m_vPos.Y < 0 - 300)
	{
		m_vDirection.Y = 1.0f;
	}

	Move(m_vDirection);
	TActor::Frame();
}
TNpc::TNpc()
{
	m_fSpeed = randstep(100.0f, 300.0f);
	m_vDirection.X = randstep(-1.0f, 1.0f);
	m_vDirection.Y = randstep(-1.0f, 1.0f);
}