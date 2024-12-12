#include "pch.h"
#include "CObject.h"

CObject::CObject()
{
}

void CObject::Initialize()
{
}

int CObject::Update()
{
	return 0;
}

void CObject::Late_Update()
{
}

void CObject::Render(HDC hDC)
{
}

void CObject::Release()
{
}

void CObject::Move_Frame()
{
	if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;

		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			m_tFrame.iFrameStart = 0;

		m_tFrame.dwTime = GetTickCount();
	}
}
