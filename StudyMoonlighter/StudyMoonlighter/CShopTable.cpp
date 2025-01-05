#include "pch.h"
#include "CShopTable.h"
#include "CScrollManager.h"
#include "CObjectManager.h"

CShopTable::CShopTable()
{
}

void CShopTable::Initialize()
{
	m_eOBJID = OBJ_MAPOBJ;
	m_eRender = RENDER_GAMEOBJECT;
	m_tInfo.fCX = 180;
	m_tInfo.fCY = 60;
	m_tRenderSizeX = 200;
	m_tRenderSizeY = 74;
	m_HitBox = { 610, 600, 670, 630 };
}

int CShopTable::Update()
{
	__super::Update_Rect();
	return 0;
}

void CShopTable::Late_Update()
{
}

void CShopTable::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();
	Graphics graphics(hDC);
	Image* image = Image::FromFile(L"../MoonlighterAssets/Map/Shop/shop_main_table.png");
	graphics.DrawImage(image, (int)m_tRenderRect.left + iScrollX, (int)m_tRenderRect.top + iScrollY, 0, 0, (int)m_tRenderSizeX, (int)m_tRenderSizeY, UnitPixel);

	image = Image::FromFile(L"../MoonlighterAssets/Map/Shop/cash_register01.png");
	graphics.DrawImage(image, (int)m_tRenderRect.left + iScrollX +15, (int)m_tRenderRect.top + iScrollY, 0, 0, 52, 46, UnitPixel);
	delete image;

	if (g_bDevmode) {
		Hitbox(hDC, m_tRect, iScrollX, iScrollY);
		Renderbox(hDC, m_tRenderRect, iScrollX, iScrollY);
		DetectRect(hDC, m_HitBox, iScrollX, iScrollY);
	}
}

void CShopTable::Release()
{
}

void CShopTable::OnCollision(CObject* _obj)
{
	RECT rc;
	list<CObject*> _npclist = CObjectManager::Get_Instance()->Get_NpcList();
	for (auto& _npc : _npclist) {
		if (IntersectRect(&rc, &m_HitBox, _npc->Get_Rect()))
		{
			_npc->OnCollision(this);
		}
	}
}
