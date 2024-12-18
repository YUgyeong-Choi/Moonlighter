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
	case HOLE_1:
		m_tInfo.fCX = 45.f;
		m_tInfo.fCY = 536.f;
		m_tRenderSizeX = 84.f;
		m_tRenderSizeY = 536.f;
		break;
	case HOLE_2:
		m_tInfo.fCX = 90.f;
		m_tInfo.fCY = 536.f;
		m_tRenderSizeX = 168.f;
		m_tRenderSizeY = 536.f;
		break;
	case HOLE_3:
		m_tInfo.fCX = 90.f;
		m_tInfo.fCY = 90.f;
		m_tRenderSizeX = 150.f;
		m_tRenderSizeY = 150.f;
	default:
		break;
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
	case HOLE_1:
		image = Image::FromFile(L"../MoonlighterAssets/Map/Tutorial/golem_roll_hole.png");
		break;
	case HOLE_2:
		image = Image::FromFile(L"../MoonlighterAssets/Map/Tutorial/golem_roll_hole2.png");
		break;
	case HOLE_3:
		image = Image::FromFile(L"../MoonlighterAssets/Map/Tutorial/golem_roll_hole3.png");
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
