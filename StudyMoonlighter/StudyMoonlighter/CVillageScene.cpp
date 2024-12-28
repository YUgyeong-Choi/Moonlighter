#include "pch.h"
#include "CVillageScene.h"
#include "CBitManager.h"
#include "CAbstractFactory.h"
#include "CObjectManager.h"
#include "CPlayer.h"
#include "CScrollManager.h"
#include "CKeyManager.h"
#include "CTree1.h"
#include "CCollisionBox.h"
#include "CPortal.h"
#include "CSoundManager.h"
#include "CUiManager.h"
CVillageScene::CVillageScene()
{
}

void CVillageScene::Initialize()
{
	CSoundManager::Get_Instance()->StopAll();
	CSoundManager::Get_Instance()->PlayBGM(L"rynoka_day_normal.wav", g_fBackgroundVolume,true);
	ADD_BMP(L"../MoonlighterAssets/Map/MainVillage.bmp", L"VillageBackground");
	CObjectManager::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CPlayer>::Create(2040, 646));
	CScrollManager::Get_Instance()->Set_Scroll(-1470, -279);
	m_fMapXSize = 2602.f;
	m_fMapYSize = 2134.f;
	CScrollManager::Get_Instance()->Set_ScrollLock(m_fMapXSize, m_fMapYSize);

	Create_MapObj();
}

int CVillageScene::Update()
{
	Key_Input();
	CObjectManager::Get_Instance()->Update();
	return 0;
}

void CVillageScene::LateUpdate()
{
	Offset();
	CObjectManager::Get_Instance()->Late_Update();
	CScrollManager::Get_Instance()->Scroll_Lock();
}

void CVillageScene::Render(HDC hDC)
{
	HDC hMemDC = CBitManager::GetInstance()->FindImage(L"VillageBackground");
	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();
	GdiTransparentBlt(hDC, iScrollX, iScrollY, m_fMapXSize, m_fMapYSize, hMemDC, 0, 0, m_fMapXSize, m_fMapYSize, RGB(0, 0, 0));
	CObjectManager::Get_Instance()->Render(hDC);
	CUiManager::GetInstance()->Render(hDC);
}

void CVillageScene::Release()
{
	CObjectManager::Get_Instance()->Delete_ID(OBJ_MAPOBJ);
	CObjectManager::Get_Instance()->Delete_ID(OBJ_PORTAL);
	CObjectManager::Get_Instance()->RenderListClear();
}

void CVillageScene::Key_Input()
{
	if (CKeyManager::Get_Instance()->Key_Down(VK_F1)) {
		g_bDevmode = !g_bDevmode;
	}
}

void CVillageScene::Create_MapObj()
{
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CTree1>::Create(490, 530));
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CCollisionBox>::Create(450, 820, 260, 280));
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CCollisionBox>::Create(910, 350, 260, 280));
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CCollisionBox>::Create(1225, 410,260,280));
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CCollisionBox>::Create(1620,350,400,280));
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CCollisionBox>::Create(2185,845,260,280));
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CCollisionBox>::Create(2255,1280,480, 300));
	CObjectManager::Get_Instance()->Add_Object(OBJ_PORTAL, CAbstractFactory<CPortal>::Create(670, 0, 140, 100));
	static_cast<CPortal*>(CObjectManager::Get_Instance()->Get_LastPortal())->Set_PortalType(FIELD);
	CObjectManager::Get_Instance()->Add_Object(OBJ_PORTAL, CAbstractFactory<CPortal>::Create(2600,680,100,120));
	static_cast<CPortal*>(CObjectManager::Get_Instance()->Get_LastPortal())->Set_PortalType(FIELD);
}

void CVillageScene::Offset()
{
	CObject* _copyPlayer = CObjectManager::Get_Instance()->Get_Player();
	int		iOffSetminX = 412;
	int		iOffSetmaxX = 612;

	int iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	if (iOffSetminX > _copyPlayer->Get_Info().fX + iScrollX)
		CScrollManager::Get_Instance()->Set_ScrollX(_copyPlayer->Get_Speed());

	if (iOffSetmaxX < _copyPlayer->Get_Info().fX + iScrollX)
		CScrollManager::Get_Instance()->Set_ScrollX(-_copyPlayer->Get_Speed());

	int		iOffSetminY = 260;
	int		iOffSetmaxY = 460;

	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

	if (iOffSetminY > _copyPlayer->Get_Info().fY + iScrollY)
		CScrollManager::Get_Instance()->Set_ScrollY(_copyPlayer->Get_Speed());

	if (iOffSetmaxY < _copyPlayer->Get_Info().fY + iScrollY)
		CScrollManager::Get_Instance()->Set_ScrollY(-_copyPlayer->Get_Speed());
}
