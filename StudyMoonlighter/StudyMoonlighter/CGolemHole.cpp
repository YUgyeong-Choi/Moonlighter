#include "pch.h"
#include "CGolemHole.h"
#include "CObjectManager.h"
#include "CScrollManager.h"

CGolemHole::CGolemHole():m_type(HOLE_END)
{
}

void CGolemHole::Initialize()
{
	m_eOBJID = OBJ_FLOOR;
	m_mapType = GOLEM_HOLE;
	m_eRender = RENDER_BACKGROUND;
}

int CGolemHole::Update()
{
	switch (m_type)
	{
	case 0:
		m_tInfo.fCX = 45.f;
		m_tInfo.fCY = 536.f;
		m_tRenderSizeX = 84.f;
		m_tRenderSizeY = 536.f;
		break;
	case 1:
		m_tInfo.fCX = 90.f;
		m_tInfo.fCY = 536.f;
		m_tRenderSizeX = 168.f;
		m_tRenderSizeY = 536.f;
		break;
	default:
		break;
	}
	if (CObjectManager::Get_Instance()->Get_IsMapMove()) {
		m_tInfo.fX = m_originX - CObjectManager::Get_Instance()->Get_MapMoveX();
	}
	__super::Update_Rect();
	return 0;
}

void CGolemHole::Late_Update()
{
}

void CGolemHole::Render(HDC hDC)
{
	Image* image(nullptr);

	switch (m_type)
	{
	case 0:
		image = Image::FromFile(L"../MoonlighterAssets/Map/Tutorial/golem_roll_hole.png");
		break;
	case 1:
		image = Image::FromFile(L"../MoonlighterAssets/Map/Tutorial/golem_roll_hole2.png");
		break;
	default:
		break;
	}
		
		
	Graphics graphics(hDC);

	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

	graphics.DrawImage(image, (int)m_tRenderRect.left + iScrollX, (int)m_tRenderRect.top + iScrollY, 0, 0, (int)m_tRenderSizeX, (int)m_tRenderSizeY, UnitPixel);

	if (g_bDevmode) {
		Hitbox(hDC, m_tRect, iScrollX, iScrollY);
		Renderbox(hDC, m_tRenderRect, iScrollX, iScrollY);
	}
}

void CGolemHole::Release()
{
}
