#include "pch.h"
#include "CGolemBossScene.h"
#include "CObjectManager.h"
#include "CPlayer.h"
#include "CBitManager.h"
#include "CScrollManager.h"
#include "CUiManager.h"
#include "CKeyManager.h"
#include "CGolemBoss.h"
#include "CAbstractFactory.h"
#include "CGolemScroll.h"
#include "CSoundManager.h"

CGolemBossScene::CGolemBossScene():m_bWake(false), m_bBossOffSetTrue(false), m_bScrollOpen(false)
{
}

void CGolemBossScene::Initialize()
{
	CSoundManager::Get_Instance()->StopSound(SOUND_BGM);
	CSoundManager::Get_Instance()->PlaySound(L"golem_boss_track.wav", SOUND_BGM, g_fBackgroundVolume, true);

	CObjectManager::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CPlayer>::Create(WINCX / 2, WINCY / 2));
	static_cast<CPlayer*>(CObjectManager::Get_Instance()->Get_Player())->Set_Pos(1160, 1182);
	m_fMapXSize = 2312.f;
	m_fMapYSize = 1566.f;
	CScrollManager::Get_Instance()->Set_Scroll(-600, -702);
	CScrollManager::Get_Instance()->Set_ScrollLock(m_fMapXSize, m_fMapYSize);

	ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/boss/Rock/boss1_rock_shadow.bmp", L"GolemBossShadow");
	ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/boss/Rock/boss1_rock1.bmp", L"GolemBossRock1");
	ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/boss/Rock/boss1_rock2.bmp", L"GolemBossRock2");
	ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/boss/Rock/boss1_rock3.bmp", L"GolemBossRock3");
	ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/boss/Rock/boss1_rock4.bmp", L"GolemBossRock4");
	ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/boss/Rock/boss1_rock5.bmp", L"GolemBossRock5");
	ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/boss/Rock/boss1_rock6.bmp", L"GolemBossRock6");
	ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/boss/Rock/boss1_rock_death.bmp", L"GolemBossRockDeath");
	ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/boss/boss1_punch_down.bmp", L"GolemBossPunchDown");
	ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/boss/boss1_punch_up.bmp", L"GolemBossPunchUp");
	ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/background_boss.bmp", L"GolemBossField");


	Create_MapObj();
}

int CGolemBossScene::Update()
{
	if (m_bBossOffSetTrue) {
		if (!m_bScrollOpen) {
			CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CGolemScroll>::Create(1176, 700));
			m_bScrollOpen = true;
		}
		
	}
	Key_Input();
	CObjectManager::Get_Instance()->Update();
	return 0;
}

void CGolemBossScene::LateUpdate()
{
	if (m_bWake) {
		Offset();
	}
	else {
		BossOffset();
	}
	
	CObjectManager::Get_Instance()->Late_Update();
	CScrollManager::Get_Instance()->Scroll_Lock();
}

void CGolemBossScene::Render(HDC hDC)
{
	HDC hMemDC = CBitManager::GetInstance()->FindImage(L"GolemBossField");
	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();
	GdiTransparentBlt(hDC, iScrollX, iScrollY, (int)m_fMapXSize, (int)m_fMapYSize, hMemDC, 0, 0, (int)m_fMapXSize, (int)m_fMapYSize, RGB(0, 0, 0));

	Image* image(nullptr);
	image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/boss/boss1_crack.png");
	Graphics graphics(hDC);
	graphics.DrawImage(image, 670 + iScrollX, (int)-10 + iScrollY, (int)0, 0, 1024, 1024, UnitPixel);

	delete image;
	CObjectManager::Get_Instance()->Render(hDC);
	CUiManager::GetInstance()->Render(hDC);
}

void CGolemBossScene::Release()
{
	CObjectManager::Get_Instance()->Delete_ID(OBJ_PORTAL);
	CObjectManager::Get_Instance()->RenderListClear();
}

void CGolemBossScene::Key_Input()
{
	if (CKeyManager::Get_Instance()->Key_Down(KEY_MODE,VK_F1)) {
		g_bDevmode = !g_bDevmode;
	}
}

void CGolemBossScene::Create_MapObj()
{
	CObjectManager::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CGolemBoss>::Create(1176, 500));
}

void CGolemBossScene::Offset()
{
	CObject* _copyPlayer = CObjectManager::Get_Instance()->Get_Player();
	int		iOffSetminX = 462;
	int		iOffSetmaxX = 562;

	int iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	if (iOffSetminX > _copyPlayer->Get_Info().fX + iScrollX)
		CScrollManager::Get_Instance()->Set_ScrollX(_copyPlayer->Get_Speed());

	if (iOffSetmaxX < _copyPlayer->Get_Info().fX + iScrollX)
		CScrollManager::Get_Instance()->Set_ScrollX(-_copyPlayer->Get_Speed());

	int		iOffSetminY = 310; 
	int		iOffSetmaxY = 410;

	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

	if (iOffSetminY > _copyPlayer->Get_Info().fY + iScrollY)
		CScrollManager::Get_Instance()->Set_ScrollY(_copyPlayer->Get_Speed());

	if (iOffSetmaxY < _copyPlayer->Get_Info().fY + iScrollY)
		CScrollManager::Get_Instance()->Set_ScrollY(-_copyPlayer->Get_Speed());
}

void CGolemBossScene::BossOffset()
{
	m_bBossOffSetTrue = true;

	CObject* _copyPlayer = CObjectManager::Get_Instance()->Get_Player();
	int		iOffSetminX = 412;
	int		iOffSetmaxX = 612;

	int iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	if (iOffSetminX > 1176 + iScrollX) {
		m_bBossOffSetTrue = false;
		CScrollManager::Get_Instance()->Set_ScrollX(_copyPlayer->Get_Speed());
	}
		
	if (iOffSetmaxX < 1176 + iScrollX) {
		m_bBossOffSetTrue = false;
		CScrollManager::Get_Instance()->Set_ScrollX(-_copyPlayer->Get_Speed());
	}
		

	int		iOffSetminY = 260; 
	int		iOffSetmaxY = 460;

	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

	if (iOffSetminY > 500 + iScrollY) {
		m_bBossOffSetTrue = false;
		CScrollManager::Get_Instance()->Set_ScrollY(_copyPlayer->Get_Speed());
	}
		

	if (iOffSetmaxY < 500 + iScrollY) {
		m_bBossOffSetTrue = false;
		CScrollManager::Get_Instance()->Set_ScrollY(-_copyPlayer->Get_Speed());
	}
		
}
