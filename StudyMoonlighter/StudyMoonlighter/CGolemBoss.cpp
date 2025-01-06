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

CGolemBoss::CGolemBoss():m_ePrePattern(NONE), m_eCurPattern(NONE), m_IsWake(false), m_fPatternCool(0), m_PatternIndex(0), m_preFrame(0)
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
}

int CGolemBoss::Update()
{
	if (static_cast<CGolemBossScene*>(CSceneManager::GetInstance()->Get_Scene())->Get_bBossOffSet() && !m_IsWake) {
		m_eCurPattern = WAKEUP;
		if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd) {
			static_cast<CGolemBossScene*>(CSceneManager::GetInstance()->Get_Scene())->Set_bWakeup();
			m_IsWake = true;
			m_eCurPattern = IDLE;
			m_fPatternCool = GetTickCount64();
		}
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
				m_PatternIndex = 0;
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
			m_preFrame = 0;
		}
		break;
	case CGolemBoss::SHOOTIDLE:
		if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd) {
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

			if (m_tFrame.iFrameStart == 20 && m_preFrame != 20) {
				m_preFrame = 20;
				SpawnRockCircle(300,20);
			}

			if (m_tFrame.iFrameStart == 25 && m_preFrame != 25) {
				m_preFrame = 25;
				SpawnRockCircle(500, 30);
			}

			if (m_tFrame.iFrameStart == 30 && m_preFrame != 30) {
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

			if (m_tFrame.iFrameStart == 20 && m_preFrame != 20) {
				m_preFrame = 20;
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
	__super::Move_Frame();
	if (!static_cast<CGolemBossScene*>(CSceneManager::GetInstance()->Get_Scene())->Get_bBossOffSet()) {
		m_tFrame.iFrameStart = 0;
	}
}

void CGolemBoss::Render(HDC hDC)
{
	HDC hMemDC = CBitManager::GetInstance()->FindImage(L"GolemBossIdle");
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
		break;
	case CGolemBoss::NONE:
		break;
	default:
		break;
	}
	
	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();
	GdiTransparentBlt(hDC, (int)m_tRenderRect.left + iScrollX, (int)m_tRenderRect.top + iScrollY, (int)m_tRenderSizeX, (int)m_tRenderSizeY, hMemDC, (int)m_tRenderSizeX * m_tFrame.iFrameStart, 0, (int)m_tRenderSizeX, (int)m_tRenderSizeY, RGB(255, 255, 255));


	if (g_bDevmode) {
		Hitbox(hDC, m_tRect, iScrollX, iScrollY);
		Hitbox(hDC, m_HitBox, iScrollX, iScrollY);
		Renderbox(hDC, m_tRenderRect, iScrollX, iScrollY);
	}

}

void CGolemBoss::Release()
{
}

void CGolemBoss::OnCollision(CObject* _obj)
{
}

void CGolemBoss::Change_Frame()
{
	if (m_eCurPattern != m_ePrePattern) {
		switch (m_eCurPattern)
		{
		case CGolemBoss::WAKEUP:
			CSoundManager::Get_Instance()->StopSound(MONSTER_EFFECT);
			CSoundManager::Get_Instance()->PlaySound(L"golem_dungeon_king_golem_awake.wav", MONSTER_EFFECT, 0.1f, true);
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
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 15;
			m_tFrame.dwSpeed = 80;
			m_tFrame.dwTime = GetTickCount64();
			break;
		case CGolemBoss::SHOOTIDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 15;
			m_tFrame.dwSpeed = 80;
			m_tFrame.dwTime = GetTickCount64();
			break;
		case CGolemBoss::SHOOTEND:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 22;
			m_tFrame.dwSpeed = 80;
			m_tFrame.dwTime = GetTickCount64();
			break;
		case CGolemBoss::SPAWNCIRCLE:
		case CGolemBoss::SPAWNRANDOM:
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

