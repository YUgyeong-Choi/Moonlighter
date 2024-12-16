#include "pch.h"
#include "CScrollWasd.h"
#include "CBitManager.h"
#include "CScrollManager.h"

CScrollWasd::CScrollWasd():m_bIsOpen(false)
{
}

void CScrollWasd::Initialize()
{
	m_eOBJID = OBJ_MAPOBJ;
	m_mapType = SCROLLWASD;

	m_tInfo.fCX = 502.f;
	m_tInfo.fCY = 80.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 5;
	m_tFrame.dwSpeed = 200;
	m_tFrame.dwTime = GetTickCount64();

	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/Map/Tutorial/tutorial_wasd.bmp", L"Tutorial_wasd");
	m_tRenderSizeX = 502.f;
	m_tRenderSizeY = 80.f;

	m_eRender = RENDER_BACKGROUND;
}

int CScrollWasd::Update()
{
	__super::Update_Rect();
	return 0;
}

void CScrollWasd::Late_Update()
{
	if (!m_bIsOpen) {
		if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount64())
		{
			++m_tFrame.iFrameStart;

			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd) {
				m_tFrame.iFrameStart = m_tFrame.iFrameEnd;
				m_bIsOpen = true;
			}

			m_tFrame.dwTime = GetTickCount64();
		}
	}
}

void CScrollWasd::Render(HDC hDC)
{
	HDC hMemDC = CBitManager::GetInstance()->FindImage(L"Tutorial_wasd");
	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();
	GdiTransparentBlt(hDC, m_tRenderRect.left + iScrollX, m_tRenderRect.top + iScrollY, m_tRenderSizeX, m_tRenderSizeY, hMemDC, m_tFrame.iFrameStart*m_tRenderSizeX, 0, m_tRenderSizeX, m_tRenderSizeY, RGB(255, 255, 255));
	if (g_bDevmode) {
		Hitbox(hDC, m_tRect, iScrollX, iScrollY);
		Renderbox(hDC, m_tRenderRect, iScrollX, iScrollY);
	}
}

void CScrollWasd::Release()
{
}
