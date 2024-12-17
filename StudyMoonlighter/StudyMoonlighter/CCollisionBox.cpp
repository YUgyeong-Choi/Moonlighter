#include "pch.h"
#include "CCollisionBox.h"
#include "CScrollManager.h"
#include "CObjectManager.h"

CCollisionBox::CCollisionBox()
{
}

void CCollisionBox::Initialize()
{
	m_eOBJID = OBJ_MAPOBJ;
	m_mapType = COLLISION;

	m_eRender = RENDER_BACKGROUND;
}

int CCollisionBox::Update()
{
	if (CObjectManager::Get_Instance()->Get_IsMapMove()) {
		m_tInfo.fX = m_originX - CObjectManager::Get_Instance()->Get_MapMoveX();
	}
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
