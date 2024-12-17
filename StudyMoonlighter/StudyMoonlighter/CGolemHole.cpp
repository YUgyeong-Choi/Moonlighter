#include "pch.h"
#include "CGolemHole.h"
#include "CObjectManager.h"
#include "CScrollManager.h"

CGolemHole::CGolemHole() :m_IsTop(false), m_FreeLeft(false), m_FreeRight(false), m_type(LEFTRIGHT_TOP)
{
}

void CGolemHole::Initialize()
{
	m_eOBJID = OBJ_MAPOBJ;
	//m_mapType = GOLEM_HOLE;

	m_tInfo.fCX = 48.f;
	m_tInfo.fCY = 48.f;

	m_tRenderSizeX = 48.f;
	m_tRenderSizeY = 48.f;

	m_eRender = RENDER_BACKGROUND;
}

int CGolemHole::Update()
{
	if (CObjectManager::Get_Instance()->Get_IsMapMove()) {
		m_tInfo.fX = m_originX - CObjectManager::Get_Instance()->Get_MapMoveX();
	}
	__super::Update_Rect();
	return 0;
}

void CGolemHole::Late_Update()
{
	switch (m_type)
	{
	case HOLE_LEFT:
		m_FreeLeft = true;
		break;
	case HOLE_RIGHT:
		m_FreeRight = true;
		break;
	case LEFTRIGHT:
		m_FreeLeft = true;
		m_FreeRight = true;
		break;
	case LEFT_TOP:
		m_FreeLeft = true;
		m_IsTop = true;
		break;
	case RIGHT_TOP:
		m_FreeRight = true;
		m_IsTop = true;
		break;
	case LEFTRIGHT_TOP:
		m_FreeLeft = true;
		m_FreeRight = true;
		m_IsTop = true;
		break;
	default:
		break;
	}
}

void CGolemHole::Render(HDC hDC)
{
	Image* image(nullptr);
	if (m_IsTop) {
		image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/Golem_HoleSlope.png");
	}
	else {
		image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/Golem_Hole.png");
	}
	
	Graphics graphics(hDC);

	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

	graphics.DrawImage(image, (int)m_tRenderRect.left + iScrollX, (int)m_tRenderRect.top + iScrollY, 0, 0, (int)m_tRenderSizeX, (int)m_tRenderSizeY, UnitPixel);

	if (m_FreeLeft) {
		image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/Golem_HoleBorder_Left.png");
		graphics.DrawImage(image, (int)m_tRenderRect.left + iScrollX - 9, (int)m_tRenderRect.top + iScrollY, 0, 0, (int)18, (int)52, UnitPixel);
	}

	if (m_FreeRight) {
		image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/Golem_HoleBorder_Right.png");
		graphics.DrawImage(image, (int)m_tRenderRect.left + iScrollX + 39, (int)m_tRenderRect.top + iScrollY-3, 0, 0, (int)18, (int)52, UnitPixel);
	}
	

	if (g_bDevmode) {
		Hitbox(hDC, m_tRect, iScrollX, iScrollY);
		Renderbox(hDC, m_tRenderRect, iScrollX, iScrollY);
	}
}

void CGolemHole::Release()
{
}
