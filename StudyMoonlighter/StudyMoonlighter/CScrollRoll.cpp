#include "pch.h"
#include "CScrollRoll.h"
#include "CObjectManager.h"
#include "CScrollManager.h"

CScrollRoll::CScrollRoll()
{
}

void CScrollRoll::Initialize()
{
	m_eOBJID = OBJ_MAPOBJ;
	m_mapType = SCROLLROLL;

	m_tInfo.fCX = 334.f;
	m_tInfo.fCY = 66.f;

	m_tRenderSizeX = 334.f;
	m_tRenderSizeY = 66.f;

	m_eRender = RENDER_BACKGROUND;
}

int CScrollRoll::Update()
{
	if (CObjectManager::Get_Instance()->Get_IsMapMove()) {
		m_tInfo.fX = m_originX - CObjectManager::Get_Instance()->Get_MapMoveX();
	}
	__super::Update_Rect();
	return 0;
}

void CScrollRoll::Late_Update()
{
}

void CScrollRoll::Render(HDC hDC)
{
	Image image(L"../MoonlighterAssets/Map/Tutorial/png/Tutorial_Scroll_RollHole.png");
	Graphics graphics(hDC);

	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

	graphics.DrawImage(&image, (int)m_tRenderRect.left + iScrollX, (int)m_tRenderRect.top + iScrollY, 0, 0, (int)m_tRenderSizeX, (int)m_tRenderSizeY, UnitPixel);

	if (g_bDevmode) {
		Hitbox(hDC, m_tRect, iScrollX, iScrollY);
		Renderbox(hDC, m_tRenderRect, iScrollX, iScrollY);
	}
}

void CScrollRoll::Release()
{
}