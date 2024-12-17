#include "pch.h"
#include "CScrollWasd.h"
#include "CBitManager.h"
#include "CScrollManager.h"
#include "CObjectManager.h"

CScrollWasd::CScrollWasd():m_bIsOpen(false)
{
}

void CScrollWasd::Initialize()
{
	m_eOBJID = OBJ_MAPOBJ;
	m_mapType = SCROLLWASD;

	m_tInfo.fCX = 502.f;
	m_tInfo.fCY = 76.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 5;
	m_tFrame.dwSpeed = 200;
	m_tFrame.dwTime = GetTickCount64();

	m_tRenderSizeX = 502.f;
	m_tRenderSizeY = 76.f;

	m_eRender = RENDER_BACKGROUND;
}

int CScrollWasd::Update()
{
	if (CObjectManager::Get_Instance()->Get_IsMapMove()) {
		m_tInfo.fX = m_originX - CObjectManager::Get_Instance()->Get_MapMoveX();
	}
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
	Image image(L"../MoonlighterAssets/Map/Tutorial/Scroll_WASD.png");
	Graphics graphics(hDC);

	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

	graphics.DrawImage(&image, (int)m_tRenderRect.left + iScrollX, (int)m_tRenderRect.top + iScrollY, (int)m_tRenderSizeX * m_tFrame.iFrameStart, 0, (int)m_tRenderSizeX, (int)m_tRenderSizeY, UnitPixel);

	if (g_bDevmode) {
		Hitbox(hDC, m_tRect, iScrollX, iScrollY);
		Renderbox(hDC, m_tRenderRect, iScrollX, iScrollY);
	}
}

void CScrollWasd::Release()
{
}
