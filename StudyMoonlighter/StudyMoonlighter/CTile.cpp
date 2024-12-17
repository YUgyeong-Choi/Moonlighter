#include "pch.h"
#include "CTile.h"
#include "CScrollManager.h"
#include "CBitManager.h"

CTile::CTile():m_bIsWalk(true)
{
}

void CTile::Initialize()
{
	m_tInfo.fCX = TILECX;
	m_tInfo.fCY = TILECY;
	m_tRenderSizeX = 48.f;
	m_tRenderSizeY = 48.f;
}

int CTile::Update()
{
	__super::Update_Rect();
	return 0;
}

void CTile::Late_Update()
{
}

void CTile::Render(HDC hDC)
{
	Image  image(L"../MoonlighterAssets/Map/Dungeon1/Golem_Hole.png");
	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();
	
	if (!m_bIsWalk) {
		Graphics graphics(hDC);
		int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
		int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

		graphics.DrawImage(&image, (int)m_tRenderRect.left + iScrollX, (int)m_tRenderRect.top + iScrollY, 0, 0, (int)m_tRenderSizeX, (int)m_tRenderSizeY, UnitPixel);
	}
	Renderbox(hDC, m_tRenderRect, iScrollX, iScrollY);
	//Hitbox(hDC, m_tRect, iScrollX, iScrollY);
}

void CTile::Release()
{
}
