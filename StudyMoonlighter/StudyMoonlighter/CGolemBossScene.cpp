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

CGolemBossScene::CGolemBossScene():m_bWake(false), m_bBossOffSetTrue(false)
{
}

void CGolemBossScene::Initialize()
{
	CObjectManager::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CPlayer>::Create(WINCX / 2, WINCY / 2));
	ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/background_boss.bmp", L"GolemBossField");
	static_cast<CPlayer*>(CObjectManager::Get_Instance()->Get_Player())->Set_Pos(1160, 1182);
	m_fMapXSize = 2312.f;
	m_fMapYSize = 1566.f;
	CScrollManager::Get_Instance()->Set_Scroll(-600, -702);
	CScrollManager::Get_Instance()->Set_ScrollLock(m_fMapXSize, m_fMapYSize);

	Create_MapObj();
}

int CGolemBossScene::Update()
{
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
	GdiTransparentBlt(hDC, iScrollX, iScrollY, (int)m_fMapXSize, m_fMapYSize, hMemDC, 0, 0, (int)m_fMapXSize, m_fMapYSize, RGB(0, 0, 0));

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
	if (CKeyManager::Get_Instance()->Key_Down(VK_F1)) {
		g_bDevmode = !g_bDevmode;
	}
}

void CGolemBossScene::Create_MapObj()
{
	CObjectManager::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CGolemBoss>::Create(1176, 500));
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CGolemScroll>::Create(1176, 700));
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
