#include "pch.h"
#include "CBabySlime.h"
#include "CScrollManager.h"
#include "CSoundManager.h"
#include "CObjectManager.h"
#include "CCollisionManager.h"
#include "CAbstractFactory.h"
#include "CItem.h"

CBabySlime::CBabySlime()
{
}

void CBabySlime::Initialize()
{
	m_eOBJID = OBJ_MONSTER;
	m_MonsterType = BABYSLIME;

	m_tInfo.fCX = 15.f;
	m_tInfo.fCY = 15.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 4;
	m_tFrame.dwSpeed = 100;
	m_tFrame.dwTime = GetTickCount64();

	m_tRenderSizeX = 70.f;
	m_tRenderSizeY = 70.f;
	m_eRender = RENDER_GAMEOBJECT;

	m_targetObj = CObjectManager::Get_Instance()->Get_Player();

	m_iHp = 25;
	m_iAttackDamage = 12;
	m_iMaxHp = m_iHp;
	m_fSpeed = 1.f;
	InitHitFrame();
}

int CBabySlime::Update()
{
	if (m_bHit) {
		if (m_HitFrame.iFrameStart == m_HitFrame.iFrameEnd) {
			m_bHit = false;
		}
	}
	if (m_iHp <= 0) {
		CObjectManager::Get_Instance()->Add_Object(OBJ_ITEM, CAbstractFactory<CItem>::Create(m_tInfo.fX, m_tInfo.fY));
		static_cast<CItem*>(CObjectManager::Get_Instance()->Get_LastItem())->Set_ItemType(RICHJELLY);
		return OBJ_DEAD;
	}
	m_fTimeSinceLastStep += 0.1f;
	if (m_fTimeSinceLastStep >= 5) {
		CSoundManager::Get_Instance()->StopSound(MONSTER_EFFECT);
		CSoundManager::Get_Instance()->PlaySound(L"golem_dungeon_slime_attack.wav", MONSTER_EFFECT, g_fMonsterVolume, true);
		m_fTimeSinceLastStep = 0;
	}

	float   fWidth(0.f), fHeight(0.f), fDiagonal(0.f), fRadian(0.f), m_fAngle(0.f);

	fWidth = m_targetObj->Get_Info().fX - m_tInfo.fX;
	fHeight = m_targetObj->Get_Info().fY - m_tInfo.fY;

	fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

	fRadian = acosf(fWidth / fDiagonal);

	m_fAngle = fRadian * (180.f / PI);

	if (m_targetObj->Get_Info().fY > m_tInfo.fY)
		m_fAngle *= -1.f;

	// degree to radian
	m_tInfo.fX += m_fSpeed * cosf(m_fAngle * (PI / 180.f));
	m_tInfo.fY -= m_fSpeed * sinf(m_fAngle * (PI / 180.f));

	__super::Update_Rect();
	return 0;
}

void CBabySlime::Late_Update()
{
	OnCollision();
	Hit();
	if (m_bHit) {
		Move_Frame_Hit();
	}
	__super::Move_Frame();
}

void CBabySlime::Render(HDC hDC)
{
	Image* image(nullptr);
	image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/babyslime/babyslime_walk.png");
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

void CBabySlime::Release()
{
}

void CBabySlime::OnCollision()
{
	CObject* _copyPlayer = CObjectManager::Get_Instance()->Get_Player();
	
	if (CCollisionManager::CollisionRectWeapon(_copyPlayer, this)){
		if (m_bCanHit) {
			if (m_fAttacktedTime + 500 < GetTickCount64()) {
				m_iAttackedDamage = _copyPlayer->Get_AttackDamage();
				m_bCanHit = false;
				m_fAttacktedTime = GetTickCount64();
				CSoundManager::Get_Instance()->StopSound(MONSTER_EFFECT);
				CSoundManager::Get_Instance()->PlaySound(L"golem_dungeon_babyslime_hit.wav", MONSTER_EFFECT, g_fMonsterVolume + 0.5f, true);
				m_bHit = true;
			}
		}
	}
}

void CBabySlime::OnCollision(CObject* _obj)
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