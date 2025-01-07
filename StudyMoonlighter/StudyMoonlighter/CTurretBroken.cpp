#include "pch.h"
#include "CTurretBroken.h"
#include "CScrollManager.h"
#include "CObjectManager.h"
#include "CAbstractFactory.h"
#include "CTurretDownBullet.h"
#include "CCollisionManager.h"
#include "CSoundManager.h"
#include "CItem.h"

CTurretBroken::CTurretBroken()
{
}

void CTurretBroken::Initialize()
{
	m_eOBJID = OBJ_MONSTER;
	m_MonsterType = TURRETBROKEN;

	m_tInfo.fCX = 45.f;
	m_tInfo.fCY = 45.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 10;
	m_tFrame.dwSpeed = 20;
	m_tFrame.dwTime = GetTickCount64();

	m_tRenderSizeX = 72.f;
	m_tRenderSizeY = 72.f;
	m_eRender = RENDER_GAMEOBJECT;

	m_iHp = 50;
	m_iAttackDamage = 12;
	m_iMaxHp = m_iHp;
	InitHitFrame();
}

int CTurretBroken::Update()
{
	if (m_iHp <= 0) {
		CObjectManager::Get_Instance()->Add_Object(OBJ_ITEM, CAbstractFactory<CItem>::Create(m_tInfo.fX, m_tInfo.fY));
		static_cast<CItem*>(CObjectManager::Get_Instance()->Get_LastItem())->Set_ItemType(IRONBAR);
		return OBJ_DEAD;
	}
	if (m_tFrame.iFrameStart == 7) {
		Shoot();
	}

	__super::Update_Rect();
	return 0;
}

void CTurretBroken::Late_Update()
{
	OnCollision();
	Hit();
	if (m_bHit) {
		Move_Frame_Hit();
	}
	__super::Move_Frame();
}

void CTurretBroken::Render(HDC hDC)
{
	Image* image(nullptr); 
	switch (m_eDir)
	{
	case LEFT:
		image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/turret_broken/turret_broken_left.png");
		break;
	case RIGHT:
		image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/turret_broken/turret_broken_right.png");
		break;
	case UP:
		image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/turret_broken/turret_broken_up.png");
		break;
	case DOWN:
		image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/turret_broken/turret_broken_down.png");
		break;
	}
	Graphics graphics(hDC);

	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

	graphics.DrawImage(image, (int)m_tRenderRect.left + iScrollX, (int)m_tRenderRect.top + iScrollY, (int)m_tRenderSizeX * m_tFrame.iFrameStart, 0, (int)m_tRenderSizeX, (int)m_tRenderSizeY, UnitPixel);

	if (!m_bCanHit) {
		RenderHpUi(hDC);
		HitEffect(hDC);
	}


	if (g_bDevmode) {
		Hitbox(hDC, m_tRect, iScrollX, iScrollY);
		Renderbox(hDC, m_tRenderRect, iScrollX, iScrollY);
	}

	delete image;
}

void CTurretBroken::Release()
{
}

void CTurretBroken::OnCollision()
{
	CObject* _copyPlayer = CObjectManager::Get_Instance()->Get_Player();
	if (CCollisionManager::CollisionRectWeapon(_copyPlayer, this)) {
		if (m_bCanHit) {
			if (m_fAttacktedTime + 500 < GetTickCount64()) {
				m_iAttackedDamage = _copyPlayer->Get_AttackDamage();
				m_bCanHit = false;
				m_fAttacktedTime = GetTickCount64();
				CSoundManager::Get_Instance()->StopSound(MONSTER_EFFECT);
				CSoundManager::Get_Instance()->PlaySound(L"golem_dungeon_turret_shot_impact.wav", MONSTER_EFFECT, g_fMonsterVolume + 0.5f, true);
				m_bHit = true;
			}
		}
	}
}

void CTurretBroken::OnCollision(CObject* _obj)
{
	if (_obj->Get_OBJID() == OBJ_PLAYER_ARROW) {
		if (m_bCanHit) {
			if (m_fAttacktedTime + 500 < GetTickCount64()) {
				m_iAttackedDamage = _obj->Get_AttackDamage();
				m_bCanHit = false;
				m_fAttacktedTime = GetTickCount64();
				CSoundManager::Get_Instance()->StopSound(MONSTER_EFFECT);
				CSoundManager::Get_Instance()->PlaySound(L"golem_dungeon_turret_shot_impact.wav", MONSTER_EFFECT, g_fMonsterVolume + 0.5f, true);
			}
		}
	}
}

void CTurretBroken::Shoot()
{
	CSoundManager::Get_Instance()->StopSound(MONSTER_EFFECT);
	CSoundManager::Get_Instance()->PlaySound(L"golem_dungeon_turret_shot.wav", MONSTER_EFFECT, g_fMonsterVolume, true);
	CObjectManager::Get_Instance()->Add_Object(OBJ_MONSTER_BULLET, CAbstractFactory<CTurretDownBullet>::Create(m_tInfo.fX, m_tInfo.fY-10, m_eDir));
}
