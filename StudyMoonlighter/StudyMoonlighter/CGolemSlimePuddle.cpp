#include "pch.h"
#include "CGolemSlimePuddle.h"
#include "CBitManager.h"
#include "CScrollManager.h"

CGolemSlimePuddle::CGolemSlimePuddle()
{
}

void CGolemSlimePuddle::Initialize()
{
	m_eOBJID = OBJ_SLIME;
	m_mapType = SLIME_PUDDLE;

	m_tInfo.fCX = 0.f;
	m_tInfo.fCY = 0.f;

	m_tRenderSizeX = 400.f;
	m_tRenderSizeY = 400.f;

	m_eRender = RENDER_BACKGROUND;
}

int CGolemSlimePuddle::Update()
{
	__super::Update_Rect();
	return 0;
}

void CGolemSlimePuddle::Late_Update()
{
}

void CGolemSlimePuddle::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();
	HDC hMemDC = CBitManager::GetInstance()->FindImage(L"DungeonBackgroundSlime");
	GdiTransparentBlt(hDC, (int)m_tInfo.fX - 200 + iScrollX, (int)m_tInfo.fY - 200 + iScrollY, m_tRenderSizeX, m_tRenderSizeY, hMemDC, 0, 0, (int)m_tRenderSizeX, (int)m_tRenderSizeY, RGB(255, 255, 255));

	if (g_bDevmode) {
		Hitbox(hDC, m_tRect, iScrollX, iScrollY);
		Renderbox(hDC, m_tRenderRect, iScrollX, iScrollY);
	}
}

void CGolemSlimePuddle::Release()
{
}
