#include "pch.h"
#include "CGolemBoss.h"
#include "CScrollManager.h"
#include "CBitManager.h"
#include "CSceneManager.h"
#include "CGolemBossScene.h"
#include "CSoundManager.h"

CGolemBoss::CGolemBoss():m_ePrePattern(NONE), m_eCurPattern(NONE), m_IsWake(false), m_fPatternCool(0)
{
}

void CGolemBoss::Initialize()
{
	ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/boss/boss1_wakeup.bmp", L"GolemBossWakeup");
	ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/boss/boss1_idle.bmp", L"GolemBossIdle");
	ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/boss/boss1_spawn.bmp", L"GolemBossSpawn");
	m_eOBJID = OBJ_MONSTER;

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

	if (m_fPatternCool + 5000 > GetTickCount64()) {
		m_eCurPattern = SPAWN;
		if (16 <= m_tFrame.iFrameStart && m_tFrame.iFrameStart <= 35) {
			m_HitBox = { (int)m_tInfo.fX-50, (int)m_tInfo.fY + 100, (int)m_tInfo.fX + 100, (int)m_tInfo.fY + 200 };
		}
		else {
			m_HitBox = { 0,0,0,0 };
		}
		m_fPatternCool = GetTickCount64();
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
	HDC hMemDC = CBitManager::GetInstance()->FindImage(L"GolemBossWakeup");;
	switch (m_eCurPattern)
	{
	case CGolemBoss::WAKEUP:
		hMemDC = CBitManager::GetInstance()->FindImage(L"GolemBossWakeup");
		break;
	case CGolemBoss::IDLE:
		hMemDC = CBitManager::GetInstance()->FindImage(L"GolemBossIdle");
		break;

	case CGolemBoss::IDLENOARM:
		hMemDC = CBitManager::GetInstance()->FindImage(L"GolemBossIdle");
		break;
	case CGolemBoss::SPAWN:
		hMemDC = CBitManager::GetInstance()->FindImage(L"GolemBossSpawn");
		break;
	case CGolemBoss::DEATH:
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
			break;
		case CGolemBoss::SPAWN:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 41;
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
