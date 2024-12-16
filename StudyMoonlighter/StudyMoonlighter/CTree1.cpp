#include "pch.h"
#include "CTree1.h"
#include "CBitManager.h"
#include "CScrollManager.h"

CTree1::CTree1()
{
}

void CTree1::Initialize()
{
	m_eOBJID = OBJ_MAPOBJ;
	
	m_tInfo.fY -= 10.f;
	m_tInfo.fCX = 30.f;
	m_tInfo.fCY = 10.f;

	m_tRenderSizeX = 140.f;
	m_tRenderSizeY = 170.f;

	m_eRender = RENDER_GAMEOBJECT;
}

int CTree1::Update()
{
	__super::Update_Rect();
	return 0;
}

void CTree1::Late_Update()
{
}

void CTree1::Render(HDC hDC)
{
	Image image(L"../MoonlighterAssets/Map/Tree/Village_Tree_01.png");
	Graphics graphics(hDC);
	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

	graphics.DrawImage(&image, (int)m_tRenderRect.left + iScrollX, (int)m_tRenderRect.top + iScrollY, 0, 0, (int)m_tRenderSizeX, (int)m_tRenderSizeY, UnitPixel);

	if (g_bDevmode) {
		Hitbox(hDC, m_tRect, iScrollX, iScrollY);
		Renderbox(hDC, m_tRenderRect, iScrollX, iScrollY);
	}
}

void CTree1::Release()
{
}
