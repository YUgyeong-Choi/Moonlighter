#include "pch.h"
#include "CPlayerArrow.h"
#include "CScrollManager.h"

CPlayerArrow::CPlayerArrow()
{
}

void CPlayerArrow::Initialize()
{
	m_eOBJID = OBJ_PLAYER_ARROW;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 5;
	m_tFrame.dwSpeed = 100;
	m_tFrame.dwTime = GetTickCount64();

	m_eRender = RENDER_GAMEOBJECT;

	m_iHp = 1;
	m_iAttackDamage = 30;
	m_tRenderSizeX = 60;
	m_tRenderSizeY = 60;

}

int CPlayerArrow::Update()
{
	if (m_iHp <= 0) {
		return OBJ_DEAD;
	}

	switch (m_eDir)
	{
	case LEFT:
		m_tInfo.fCX = 10.f;
		m_tInfo.fCY = 10.f;
		m_tInfo.fX -= 8.f;
		break;
	case RIGHT:
		m_tInfo.fCX = 10.f;
		m_tInfo.fCY = 10.f;
		m_tInfo.fX += 8.f;
		break;
	case UP:
		m_tInfo.fCX = 10.f;
		m_tInfo.fCY = 10.f;
		m_tInfo.fY -= 8.f;
		break;
	case DOWN:
		m_tInfo.fCX = 10.f;
		m_tInfo.fCY = 10.f;
		m_tInfo.fY += 8.f;
		break;
	}
	__super::Update_Rect();
	return 0;
}

void CPlayerArrow::Late_Update()
{
	__super::Move_Frame();
}

void CPlayerArrow::Render(HDC hDC)
{
	Image* image(nullptr);
	switch (m_eDir)
	{
	case LEFT:
		image = Image::FromFile(L"../MoonlighterAssets/Weapon/Bow/arrow_left.png");
		break;
	case RIGHT:
		image = Image::FromFile(L"../MoonlighterAssets/Weapon/Bow/arrow_right.png");
		break;
	case UP:
		image = Image::FromFile(L"../MoonlighterAssets/Weapon/Bow/arrow_up.png");
		break;
	case DOWN:
		image = Image::FromFile(L"../MoonlighterAssets/Weapon/Bow/arrow_down.png");
		break;
	}

	Graphics graphics(hDC);

	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

	graphics.DrawImage(image, (int)m_tRenderRect.left + iScrollX, (int)m_tRenderRect.top + iScrollY, (int)m_tRenderSizeX * m_tFrame.iFrameStart, 0, (int)m_tRenderSizeX, (int)m_tRenderSizeY, UnitPixel);

	if (g_bDevmode) {
		Hitbox(hDC, m_tRect, iScrollX, iScrollY);
		Renderbox(hDC, m_tRenderRect, iScrollX, iScrollY);
	}

	delete image;
}

void CPlayerArrow::Release()
{
}

void CPlayerArrow::OnCollision(CObject* _obj)
{
	switch (_obj->Get_OBJID())
	{
	case OBJ_PLAYER:
		break;
	case OBJ_MONSTER:
		m_iHp--;
		break;
	case OBJ_MONSTER_BULLET:
		break;
	case OBJ_MAPOBJ:
		m_iHp--;
		break;
	case OBJ_FLOOR:
		break;
	case OBJ_PORTAL:
		break;
	case OBJ_BOSS:
		m_iHp--;
		break;
	case OBJ_END:
		break;
	default:
		break;
	}
}
