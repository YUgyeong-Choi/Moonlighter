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
	CObject::Update_Rect();
	return 0;
}

void CCollisionBox::Late_Update()
{
}

void CCollisionBox::Render(HDC hDC)
{
	if (g_bDevmode) {
		Hitbox(hDC, m_tRect, 0, 0);
	}
}

void CCollisionBox::Release()
{
}
