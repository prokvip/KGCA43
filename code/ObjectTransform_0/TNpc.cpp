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

	// È­¸éÁÂÇ¥°è
	/*for (auto& v : m_vListScreen)
	{
		v.p += m_vOffset;
	}*/
	m_matWorld._31 = m_vOffset.X;
	m_matWorld._32 = m_vOffset.Y;
	Trans(m_matWorld);

	UpdateVertexBuffer();

	m_vOffset = { 0.0f,0.0f };
}
TNpc::TNpc()
{
	m_fSpeed = randstep(100.0f, 300.0f);
	m_vDirection.X = randstep(-1.0f, 1.0f);
	m_vDirection.Y = randstep(-1.0f, 1.0f);
}