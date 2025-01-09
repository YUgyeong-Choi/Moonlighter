#include "pch.h"
#include "CGolemBoss.h"
#include "CScrollManager.h"
#include "CBitManager.h"
#include "CSceneManager.h"
#include "CGolemBossScene.h"
#include "CSoundManager.h"
#include "CObjectManager.h"
#include "CAbstractFactory.h"
#include "CGolemBossRock.h"
#include "CGolemPunch.h"
#include "CCollisionManager.h"
#include "CBabySlime.h"
#include "CSlimeHermit.h"
#include "CCoin.h"

CGolemBoss::CGolemBoss():m_ePrePattern(NONE), m_eCurPattern(NONE), m_IsWake(false) , m_PatternIndex(0), m_preFrame(0), m_Shoot(false), m_fAngle(0), m_monster(MONSTER_END), m_hitBoxX(0), m_hitBoxY(0), tick(0), removeTick(0), m_MonsterHitox{0,0,0,0}, count(0), m_bDead(false)
{
}

void CGolemBoss::Initialize()
{
	ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/boss/boss1_wakeup.bmp", L"GolemBossWakeup");
	ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/boss/boss1_idle.bmp", L"GolemBossIdle");
	ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/boss/boss1_idleNoarm.bmp", L"GolemBossIdleNoram");
	ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/boss/boss1_spawn.bmp", L"GolemBossSpawn");
	ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/boss/boss1_wave.bmp", L"GolemBossWave");
	ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/boss/boss1_launch.bmp", L"GolemBossLaunch");
	ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/boss/boss1_recover.bmp", L"GolemBossRecover");
	ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/boss/boss1_stickypre.bmp", L"GolemBossStickyPre");
	ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/boss/boss1_stickyidle.bmp", L"GolemBossStickyIdle");
	ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/boss/boss1_stickyend.bmp", L"GolemBossStickyEnd");
	ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/boss/boss1_stickyPunch.bmp", L"GolemBossStickyPunch");
	ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/boss/boss1_death.bmp", L"GolemBossDeath");
	ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/boss/Coin.bmp", L"Coin");
	m_eOBJID = OBJ_BOSS;

	m_tInfo.fCX = 260.f;
	m_tInfo.fCY = 150;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 31;
	m_tFrame.dwSpeed = 100;
	m_tFrame.dwTime = GetTickCount64();

	m_tRenderSizeX = 1024.f;
	m_tRenderSizeY = 1024.f;
	m_eRender = RENDER_GAMEOBJECT;

	m_targetObj = CObjectManager::Get_Instance()->Get_Player();
	count = 10;

	m_iHp = 10;
	m_iMaxHp = m_iHp;
	m_iAttackDamage = 10;
}

int CGolemBoss::Update()
{
	if (m_iHp <= 0) {
		if (m_eCurPattern != DEATH) {
			CSoundManager::Get_Instance()->StopAll();
			CSoundManager::Get_Instance()->PlaySound(L"golem_dungeon_king_golem_death.wav", SOUND_EFFECT, 1, true);
			CSoundManager::Get_Instance()->PlayBGM(L"golem_dungeon_floor.wav", g_fBackgroundVolume, true);
			SpawnMoney(300,10);
			SpawnMoney(400,15);
		}

		m_eCurPattern = DEATH;
	}
	//보스 처음 화면 관련
	if (static_cast<CGolemBossScene*>(CSceneManager::GetInstance()->Get_Scene())->Get_bBossOffSet() && !m_IsWake) {
		m_eCurPattern = WAKEUP;
		if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd) {
			static_cast<CGolemBossScene*>(CSceneManager::GetInstance()->Get_Scene())->Set_bWakeup();
			m_IsWake = true;
			m_eCurPattern = IDLE;
		}
	}

	//보스 몬스터 타입 생성
	if (m_eCurPattern == SHOOTIDLE) {
		tick++;
		if (tick > 50) {
			if (m_monster == MONSTER_END) {
				m_hitBoxX = m_tInfo.fX - 150;
				m_hitBoxY = m_tInfo.fY  + 30;
				Shoot();
				tick = 0;

				//각도
				float playerX = m_targetObj->Get_Info().fX;
				float playerY = m_targetObj->Get_Info().fY;
				float deltaX = playerX - m_hitBoxX;
				float deltaY = playerY - m_hitBoxY;
				float length = sqrtf(deltaX * deltaX + deltaY * deltaY);
				float angleCos = deltaX / length;

				m_fAngle = acosf(angleCos) * (180.f / PI);
				count--;

				if (count == 0) {
					m_Shoot = true;
					m_monster = MONSTER_END;
					removeTick = 0;
					m_MonsterHitox = { 0,0,0,0 };
					tick = 0;
				}
			}
		}
	}

	if (m_monster != MONSTER_END) {
		m_hitBoxX = m_hitBoxX + (10 * cosf(m_fAngle * (PI / 180.f)));
		m_hitBoxY = m_hitBoxY + (10 * sinf(m_fAngle * (PI / 180.f)));
		removeTick++;
		m_MonsterHitox = { m_hitBoxX - 25, m_hitBoxY - 25, m_hitBoxX + 25, m_hitBoxY + 25 };
		if (removeTick > 100) {
			m_monster = MONSTER_END;
			removeTick = 0;
			m_MonsterHitox = { 0,0,0,0 };
		}
	}

	//보스 죽으면
	if (m_eCurPattern == DEATH && m_tFrame.iFrameStart == m_tFrame.iFrameEnd) {
		m_bDead = true;
	}
	switch (m_eCurPattern)
	{
	case CGolemBoss::WAKEUP:
		break;
	case CGolemBoss::IDLE:
		if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd) {
			m_eCurPattern = m_Pattern.at(m_PatternIndex);
			m_PatternIndex++;
			if (m_PatternIndex >= m_Pattern.size()) {
				m_PatternIndex = 3;
			}
			m_preFrame = 0;
		}
		break;
	case CGolemBoss::IDLENOARM:
		if (dynamic_cast<CGolemPunch*>(CObjectManager::Get_Instance()->Get_LastMonster())->Get_PunchTime() == 0) {
			if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd) {
				m_eCurPattern = RECOVERARM;
				m_preFrame = 0;
			}
		}
		break;
	case CGolemBoss::SHOOTPRE:
		if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd) {
			m_eCurPattern = SHOOTIDLE;
			m_Shoot = false;
			m_preFrame = 0;
		}
		break;
	case CGolemBoss::SHOOTIDLE:
		if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd && m_Shoot) {
			m_eCurPattern = SHOOTEND;
			m_preFrame = 0;
		}
		break;
	case CGolemBoss::SHOOTEND:
		if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd) {
			m_eCurPattern = IDLE;
			m_preFrame = 0;
		}
		break;
	case CGolemBoss::SPAWNCIRCLE:
		if (16 <= m_tFrame.iFrameStart && m_tFrame.iFrameStart <= 35) {
			m_HitBox = { (int)m_tInfo.fX - 50, (int)m_tInfo.fY + 100, (int)m_tInfo.fX + 100, (int)m_tInfo.fY + 200 };

			if (m_tFrame.iFrameStart == 18 && m_preFrame != 18) {
				m_preFrame = 18;
				SpawnRockCircle(300,20);
				CSoundManager::Get_Instance()->StopSound(MONSTER_EFFECT);
				CSoundManager::Get_Instance()->PlaySound(L"golem_dungeon_king_golem_handcrash.wav", MONSTER_EFFECT, g_fMonsterVolume + 0.2f, true);

			}

			if (m_tFrame.iFrameStart == 25 && m_preFrame != 25) {
				m_preFrame = 25;
				SpawnRockCircle(500, 30);
			}

			if (m_tFrame.iFrameStart == 32 && m_preFrame != 32) {
				m_preFrame = 30;
				SpawnRockCircle(700, 40);
			}
		}
		else {
			m_HitBox = { 0,0,0,0 };
		}

		if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd) {
			m_eCurPattern = IDLE;
			m_preFrame = 0;
		}
		break;
	case CGolemBoss::PUNCHARM:
		if (m_tFrame.iFrameStart == 22 && m_preFrame != 22) {
			m_preFrame = 22;
			CObjectManager::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CGolemPunch>::Create(0,0));
		}


		if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd) {
			m_eCurPattern = IDLENOARM;
			m_preFrame = 0;
		}
		break;
	case CGolemBoss::RECOVERARM:
		if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd) {
			m_eCurPattern = IDLE;
			m_preFrame = 0;
		}
		break;
	case CGolemBoss::SPAWNRANDOM:
		if (16 <= m_tFrame.iFrameStart && m_tFrame.iFrameStart <= 35) {
			m_HitBox = { (int)m_tInfo.fX - 50, (int)m_tInfo.fY + 100, (int)m_tInfo.fX + 100, (int)m_tInfo.fY + 200 };

			if (m_tFrame.iFrameStart == 18 && m_preFrame != 18) {
				CSoundManager::Get_Instance()->StopSound(MONSTER_EFFECT);
				CSoundManager::Get_Instance()->PlaySound(L"golem_dungeon_king_golem_handcrash.wav", MONSTER_EFFECT, g_fMonsterVolume + 0.2f, true);
				m_preFrame = 18;
				SpawnRockRandom(60);
			}
		}
		else {
			m_HitBox = { 0,0,0,0 };
		}

		if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd) {
			m_eCurPattern = IDLE;
			m_preFrame = 0;
		}
		break;
	case CGolemBoss::DEATH:
		break;
	case CGolemBoss::NONE:
		break;
	default:
		break;
	}

	Change_Frame();
	__super::Update_Rect();
    return 0;
}

void CGolemBoss::Late_Update()
{
	Hit();
	OnCollision();
	if (!m_bDead) {
		__super::Move_Frame();
	}
	
	if (!static_cast<CGolemBossScene*>(CSceneManager::GetInstance()->Get_Scene())->Get_bBossOffSet()) {
		m_tFrame.iFrameStart = 0;
	}
}

void CGolemBoss::Render(HDC hDC)
{
	HDC hMemDC = CBitManager::GetInstance()->FindImage(L"GolemBossWakeup");
	switch (m_eCurPattern)
	{
	case CGolemBoss::WAKEUP:
		hMemDC = CBitManager::GetInstance()->FindImage(L"GolemBossWakeup");
		break;
	case CGolemBoss::IDLE:
		hMemDC = CBitManager::GetInstance()->FindImage(L"GolemBossIdle");
		break;
	case CGolemBoss::IDLENOARM:
		hMemDC = CBitManager::GetInstance()->FindImage(L"GolemBossIdleNoram");
		break;
	case CGolemBoss::SHOOTPRE:
		hMemDC = CBitManager::GetInstance()->FindImage(L"GolemBossStickyPre");
		break;
	case CGolemBoss::SHOOTIDLE:
		hMemDC = CBitManager::GetInstance()->FindImage(L"GolemBossStickyIdle");
		break;
	case CGolemBoss::SHOOTEND:
		hMemDC = CBitManager::GetInstance()->FindImage(L"GolemBossStickyEnd");
		break;
	case CGolemBoss::SPAWNCIRCLE:
	case CGolemBoss::SPAWNRANDOM:
		hMemDC = CBitManager::GetInstance()->FindImage(L"GolemBossSpawn");
		break;
	case CGolemBoss::PUNCHARM:
		hMemDC = CBitManager::GetInstance()->FindImage(L"GolemBossLaunch");
		break;
	case CGolemBoss::RECOVERARM:
		hMemDC = CBitManager::GetInstance()->FindImage(L"GolemBossRecover");
		break;
	case CGolemBoss::DEATH:
		hMemDC = CBitManager::GetInstance()->FindImage(L"GolemBossDeath");
		break;
	case CGolemBoss::NONE:
		break;
	default:
		break;
	}

	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();
	
	GdiTransparentBlt(hDC, (int)m_tRenderRect.left + iScrollX, (int)m_tRenderRect.top + iScrollY, (int)m_tRenderSizeX, (int)m_tRenderSizeY, hMemDC, (int)m_tRenderSizeX * m_tFrame.iFrameStart, 0, (int)m_tRenderSizeX, (int)m_tRenderSizeY, RGB(255, 255, 255));

	if (m_eCurPattern == SHOOTIDLE) {
		HDC hMemDC = CBitManager::GetInstance()->FindImage(L"GolemBossStickyPunch");
		GdiTransparentBlt(hDC, (int)m_tInfo.fX - 220 + iScrollX, (int)m_tInfo.fY - 130 + iScrollY, 150, 150, hMemDC, 0, 0, 150, 150, RGB(255, 255, 255));
	}

	if (m_eCurPattern == SHOOTIDLE && m_monster != MONSTER_END) {
		Graphics graphics(hDC);
		Image* image(nullptr);
	
		switch (m_monster)
		{
		case BABYSLIME:
			image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/boss/babyslime.png");
			graphics.DrawImage(image, m_hitBoxX - 50 + iScrollX, m_hitBoxY - 50 + iScrollY, 0, 0, 100, 100, UnitPixel);
			break;
		case SLIME_HERMIT:
			image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/boss/slimehermit.png");
			graphics.DrawImage(image, m_hitBoxX - 50 + iScrollX, m_hitBoxY - 50 + iScrollY, 0, 0, 100, 100, UnitPixel);
			break;
		}
		delete image;
	}

	if (g_bDevmode) {
		Hitbox(hDC, m_tRect, iScrollX, iScrollY);
		Hitbox(hDC, m_HitBox, iScrollX, iScrollY);
		HitCircle(hDC, m_MonsterHitox, iScrollX, iScrollY);
		Renderbox(hDC, m_tRenderRect, iScrollX, iScrollY);
	}
}

void CGolemBoss::Release()
{
}

void CGolemBoss::OnCollision(CObject* _obj)
{
	if (_obj->Get_OBJID() == OBJ_PLAYER_ARROW) {
		if (m_bCanHit) {
			if (m_fAttacktedTime + 500 < GetTickCount64()) {
				m_iAttackedDamage = _obj->Get_AttackDamage();
				m_bCanHit = false;
				m_fAttacktedTime = GetTickCount64();
				CSoundManager::Get_Instance()->StopSound(MONSTER_EFFECT);
				CSoundManager::Get_Instance()->PlaySound(L"golem_dungeon_king_golem_hit.wav", MONSTER_EFFECT, g_fMonsterVolume + 0.2f, true);
			}
		}
	}
}

void CGolemBoss::Change_Frame()
{
	if (m_eCurPattern != m_ePrePattern) {
		switch (m_eCurPattern)
		{
		case CGolemBoss::WAKEUP:
			CSoundManager::Get_Instance()->StopSound(MONSTER_EFFECT);
			CSoundManager::Get_Instance()->PlaySound(L"golem_dungeon_king_golem_awake.wav", MONSTER_EFFECT, g_fMonsterVolume + 0.2f, true);
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 31;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount64();
			break;
		case CGolemBoss::IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 15;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount64();
			break;
		case CGolemBoss::IDLENOARM:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 15;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount64();
			break;
		case CGolemBoss::SHOOTPRE:
			CSoundManager::Get_Instance()->StopSound(MONSTER_EFFECT);
			CSoundManager::Get_Instance()->PlaySound(L"golem_dungeon_king_golem_slimearm_prepare.wav", MONSTER_EFFECT, g_fMonsterVolume + 0.2f, true);
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 15;
			m_tFrame.dwSpeed = 120;
			m_tFrame.dwTime = GetTickCount64();
			break;
		case CGolemBoss::SHOOTIDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 15;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount64();
			break;
		case CGolemBoss::SHOOTEND:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 22;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount64();
			break;
		case CGolemBoss::SPAWNCIRCLE:
		case CGolemBoss::SPAWNRANDOM:
			CSoundManager::Get_Instance()->StopSound(MONSTER_EFFECT);
			CSoundManager::Get_Instance()->PlaySound(L"golem_dungeon_king_golem_handcrash_prepare.wav", MONSTER_EFFECT, g_fMonsterVolume + 0.2f, true);
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 41;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount64();
			break;
		case CGolemBoss::PUNCHARM:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 23;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount64();
			break;
		case CGolemBoss::RECOVERARM:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 23;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount64();
			break;
		case CGolemBoss::DEATH:
			CSoundManager::Get_Instance()->StopSound(MONSTER_EFFECT);
			CSoundManager::Get_Instance()->PlaySound(L"golem_dungeon_king_golem_death.wav", MONSTER_EFFECT, g_fMonsterVolume + 0.2f, true);
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 46;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount64();
			break;
		case CGolemBoss::NONE:
			break;
		default:
			break;
		}

		m_ePrePattern = m_eCurPattern;
	}
}

void CGolemBoss::SpawnRockCircle(float radius, int numRocks)
{
	for (int i = 0; i < numRocks; ++i) {
		float angle = i *  PI / (numRocks-1); // 각도 계산
		float x = m_tInfo.fX + radius * cos(angle);
		float y = m_tInfo.fY + radius * sin(angle);
 		CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CGolemBossRock>::Create(x, y));
	}
}

void CGolemBoss::SpawnRockRandom(int numRocks)
{
	for (int i = 0; i < numRocks; ++i) {
		int x = rand() % 1400 + 500;
		int y = rand() % 630 + 600;
		CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CGolemBossRock>::Create(x, y));
	}
}

void CGolemBoss::SpawnMoney(float radius, int numRocks)
{
	for (int i = 0; i < numRocks; ++i) {
		float angle = i * PI / (numRocks - 1);
		float x = m_tInfo.fX + radius * cos(angle);
		float y = m_tInfo.fY + radius * sin(angle);
		CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CCoin>::Create(x, y));
	}
}

void CGolemBoss::Shoot()
{
	int num = rand() % 2;
	switch (num)
	{
	case 0:
		m_monster = BABYSLIME;
		break;
	case 1:
		m_monster = SLIME_HERMIT;
		break;
	default:
		break;
	}
	CSoundManager::Get_Instance()->StopSound(MONSTER_EFFECT);
	CSoundManager::Get_Instance()->PlaySound(L"golem_dungeon_king_golem_slimearm.wav", MONSTER_EFFECT, g_fMonsterVolume + 0.2f, true);
}

void CGolemBoss::OnCollision()
{
	//몬스터 소환
	CObject* _copyPlayer = CObjectManager::Get_Instance()->Get_Player();

	float fRadius = (_copyPlayer->Get_Info().fCX + 50) * 0.5f;

	float fWidth = abs(_copyPlayer->Get_Info().fX - m_hitBoxX);
	float fHeight = abs(_copyPlayer->Get_Info().fY - m_hitBoxY);

	float fDiagonal = powf((fWidth * fWidth + fHeight * fHeight), 0.5f);

	bool check = fRadius >= fDiagonal;

	if (check) {
		switch (m_monster)
		{
		case BABYSLIME:
			CObjectManager::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CBabySlime>::Create(m_hitBoxX-40, m_hitBoxY- 40));
			CObjectManager::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CBabySlime>::Create(m_hitBoxX- 40, m_hitBoxY+ 40));
			CObjectManager::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CBabySlime>::Create(m_hitBoxX+ 40, m_hitBoxY- 40));
			CObjectManager::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CBabySlime>::Create(m_hitBoxX+ 40, m_hitBoxY+ 40));
			break;
		case SLIME_HERMIT:
			CObjectManager::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CSlimeHermit>::Create(m_hitBoxX, m_hitBoxY));
			break;
		}
		m_monster = MONSTER_END;
		removeTick = 0;
		m_MonsterHitox = { 0,0,0,0 };
	}

	//검이랑 충돌
	if (CCollisionManager::CollisionRectWeapon(_copyPlayer, this)) {
		if (m_bCanHit) {
			if (m_fAttacktedTime + 500 < GetTickCount64()) {
				m_iAttackedDamage = _copyPlayer->Get_AttackDamage();
				m_bCanHit = false;
				m_fAttacktedTime = GetTickCount64();
				CSoundManager::Get_Instance()->StopSound(MONSTER_EFFECT);
				CSoundManager::Get_Instance()->PlaySound(L"golem_dungeon_king_golem_hit.wav", MONSTER_EFFECT, g_fMonsterVolume + 0.2f, true);
			}
		}
	}
	
}

