#include "pch.h"
#include "CTurretDownBullet.h"
#include "CScrollManager.h"
#include "CCollisionBox.h"

CTurretDownBullet::CTurretDownBullet()
{
}

void CTurretDownBullet::Initialize()
{
	m_eOBJID = OBJ_MONSTER_BULLET;

	m_eRender = RENDER_GAMEOBJECT;

	m_iHp = 1;
	m_iAttackDamage = 5;
}

int CTurretDownBullet::Update()
{
	if(m_iHp <= 0){
		return OBJ_DEAD;
	}

	switch (m_eDir)
	{
	case LEFT:
		m_tInfo.fCX = 10.f;
		m_tInfo.fCY = 10.f;
		m_tRenderSizeX = 44.f;
		m_tRenderSizeY = 36.f;
		m_tInfo.fX -= 8.f;
		break;
	case RIGHT:
		m_tInfo.fCX = 10.f;
		m_tInfo.fCY = 10.f;
		m_tRenderSizeX = 44.f;
		m_tRenderSizeY = 36.f;
		m_tInfo.fX += 8.f;
		break;
	case UP:
		m_tInfo.fCX = 10.f;
		m_tInfo.fCY = 10.f;
		m_tRenderSizeX = 36.f;
		m_tRenderSizeY = 44.f;
		m_tInfo.fY -= 8.f;
		break;
	case DOWN:
		m_tInfo.fCX = 10.f;
		m_tInfo.fCY = 10.f;
		m_tRenderSizeX = 36.f;
		m_tRenderSizeY = 44.f;
		m_tInfo.fY += 8.f;
		break;
	}
	__super::Update_Rect();
    return 0;
}

void CTurretDownBullet::Late_Update()
{
}

void CTurretDownBullet::Render(HDC hDC)
{
	Image* image(nullptr);
	switch (m_eDir)
	{
	case LEFT:
		image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/turret_broken_left_bullet.png");
		break;
	case RIGHT:
		image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/turret_broken_right_bullet.png");
		break;
	case UP:
		image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/turret_broken_up_bullet.png");
		break;
	case DOWN:
		image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/turret_broken_down_bullet.png");
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

	delete image;
}

void CTurretDownBullet::Release()
{
}

void CTurretDownBullet::OnCollision(CObject* _obj)
{
	switch (_obj->Get_OBJID())
	{
	case OBJ_PLAYER:
		m_iHp--;
		break;
	case OBJ_MONSTER:
		break;
	case OBJ_MONSTER_BULLET:
		break;
	case OBJ_MAPOBJ:
		if (dynamic_cast<CMapObj*>(_obj)->Get_MapObjType() == COLLISION) {
			m_iHp--;
		}
		break;
	case OBJ_FLOOR:
		break;
	case OBJ_PORTAL:
		break;
	case OBJ_END:
		break;
	default:
		break;
	}
}
