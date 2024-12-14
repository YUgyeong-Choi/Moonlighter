#include "pch.h"
#include "CCollisionBox.h"
#include "CScrollManager.h"

void CCollisionBox::Initialize()
{
	m_eOBJID = OBJ_MAPOBJ;

	m_eRender = RENDER_BACKGROUND;
}

int CCollisionBox::Update()
{
	CObject::Update_Rect();
	return 0;
}

void CCollisionBox::Late_Update()
{
}

void CCollisionBox::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();
	if (g_bDevmode) {
		Hitbox(hDC, m_tRect, iScrollX, iScrollY);
	}
}

void CCollisionBox::Release()
{
}
