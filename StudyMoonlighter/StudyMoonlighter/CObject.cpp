#include "pch.h"
#include "CObject.h"

CObject::CObject() :m_pImgKey(nullptr), m_fSpeed(0)
{
	ZeroMemory(&m_tFrame, sizeof(FRAME));
	ZeroMemory(&m_tRect, sizeof(RECT));
	ZeroMemory(&m_tInfo, sizeof(INFO));
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

void CObject::Update_Rect()
{
	m_tRect.left = LONG(m_tInfo.fX - (m_tInfo.fCX * 0.5f));
	m_tRect.top = LONG(m_tInfo.fY - (m_tInfo.fCY * 0.5f));
	m_tRect.right = LONG(m_tInfo.fX + (m_tInfo.fCX * 0.5f));
	m_tRect.bottom = LONG(m_tInfo.fY + (m_tInfo.fCY * 0.5f));
}

void CObject::Move_Frame()
{
	if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount64())
	{
		++m_tFrame.iFrameStart;

		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			m_tFrame.iFrameStart = 0;

		m_tFrame.dwTime = GetTickCount64();
	}
}
