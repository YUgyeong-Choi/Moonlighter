#include "pch.h"
#include "CHouse.h"
#include "CScrollManager.h"

CHouse::CHouse()
{
}

void CHouse::Initialize()
{
	m_eOBJID = OBJ_MAPOBJ;

	m_tInfo.fCX = 260.f;
	m_tInfo.fCY = 280.f;

	m_eRender = RENDER_BACKGROUND;
}

int CHouse::Update()
{
	__super::Update_Rect();
	return 0;
}

void CHouse::Late_Update()
{
}

void CHouse::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();
	if (g_bDevmode) {
		Hitbox(hDC, m_tRect, iScrollX, iScrollY);
	}
}

void CHouse::Release()
{
}
