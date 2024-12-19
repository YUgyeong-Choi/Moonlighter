#include "pch.h"
#include "CDungeonEditScene.h"
#include "CBitManager.h"
#include "CObjectManager.h"
#include "CKeyManager.h"
#include "CAbstractFactory.h"
#include "CFileManager.h"

//여기 밑에는 지워도 되는 헤더 파일
#include "CCollisionBox.h"
#include "CScrollRoll.h"
#include "CGolemDoor.h"
#include "CGolemHole.h"
#include "CTurretBroken.h"
#include "CScrollWasd.h"
#include "CPlayer.h"
#include "CScrollManager.h"

CDungeonEditScene::CDungeonEditScene()
{
}

void CDungeonEditScene::Initialize()
{
	ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/background.bmp", L"DungeonBackground");
	ADD_BMP(L"../MoonlighterAssets/Tile/CanWalk48.bmp", L"CanWalk");
	ADD_BMP(L"../MoonlighterAssets/Tile/CantWalk48.bmp", L"CantWalk");
	m_fMapXSize = 1024.f;
	m_fMapYSize = 720.f;
	Create_MapObj();
	//CObjectManager::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CPlayer>::Create(WINCX / 2, WINCY / 2));
}

int CDungeonEditScene::Update()
{
	Key_Input();
	CObjectManager::Get_Instance()->Update();
    return 0;
}

void CDungeonEditScene::LateUpdate()
{
	CObjectManager::Get_Instance()->Late_Update();
}

void CDungeonEditScene::Render(HDC hDC)
{
	//Rectangle(hDC, -100, -100, 1100, 1100);
	HDC hMemDC = CBitManager::GetInstance()->FindImage(L"DungeonBackground");
	GdiTransparentBlt(hDC, 0, 0, (int)m_fMapXSize, (int)m_fMapYSize, hMemDC, 0, 0, (int)m_fMapXSize, (int)m_fMapYSize, RGB(0, 0, 0));
	CObjectManager::Get_Instance()->Render(hDC);
}

void CDungeonEditScene::Release()
{
	CObjectManager::Get_Instance()->Delete_ID(OBJ_PLAYER);
	CObjectManager::Get_Instance()->Delete_ID(OBJ_MAPOBJ);
	CObjectManager::Get_Instance()->Delete_ID(OBJ_PORTAL);
	CObjectManager::Get_Instance()->Delete_ID(OBJ_MONSTER);
	CObjectManager::Get_Instance()->Delete_ID(OBJ_MONSTER_BULLET);
	CObjectManager::Get_Instance()->RenderListClear();
}

void CDungeonEditScene::Key_Input()
{

	if (CKeyManager::Get_Instance()->Key_Down(VK_F1)) {
		g_bDevmode = !g_bDevmode;
	}

	if (CKeyManager::Get_Instance()->Key_Down('O'))
	{
		CFileManager::GetInstance()->Save_MapObj(L"../Data/MapMonster/Tutorial3.dat");
		CFileManager::GetInstance()->Save_MonsterObj(L"../Data/MapObj/Tutorial3.dat");
	}

	if (CKeyManager::Get_Instance()->Key_Down('P'))
	{
		CFileManager::GetInstance()->Load_MapObjFile(L"../Data/MapMonster/Tutorial3.dat", 3);
		CFileManager::GetInstance()->Load_MonsterFile(L"../Data/MapObj/Tutorial3.dat",3);
	}

}

void CDungeonEditScene::Create_MapObj()
{
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CCollisionBox>::Create(WINCX / 2, 30, 1024, 60));
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CCollisionBox>::Create(WINCX / 2, 690, 1024, 60));
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CCollisionBox>::Create(30, WINCY / 2, 60, 720));
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CCollisionBox>::Create(994, WINCY / 2, 60, 720));

	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CScrollRoll>::Create(WINCX / 2 - 200, 60, 0, 0));
	dynamic_cast<CScrollRoll*>(CObjectManager::Get_Instance()->Get_LastMapObj())->Set_ScrollType(SCROLL_PROJECTILE);

	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CScrollRoll>::Create(WINCX / 2 + 200, 60, 0, 0));
	dynamic_cast<CScrollRoll*>(CObjectManager::Get_Instance()->Get_LastMapObj())->Set_ScrollType(SCROLL_ENEMY);

	CObjectManager::Get_Instance()->Add_Object(OBJ_PORTAL, CAbstractFactory<CGolemDoor>::Create(50, WINCY / 2));
	dynamic_cast<CGolemDoor*>(CObjectManager::Get_Instance()->Get_LastPortal())->Set_DIR(LEFT);
	CObjectManager::Get_Instance()->Add_Object(OBJ_PORTAL, CAbstractFactory<CGolemDoor>::Create(980, WINCY / 2));
	dynamic_cast<CGolemDoor*>(CObjectManager::Get_Instance()->Get_LastPortal())->Set_DIR(RIGHT);

	CObjectManager::Get_Instance()->Add_Object(OBJ_FLOOR, CAbstractFactory<CGolemHole>::Create(300, 200));
	dynamic_cast<CGolemHole*>(CObjectManager::Get_Instance()->Get_LastFloor())->Set_HoleType(HOLE_3);

	CObjectManager::Get_Instance()->Add_Object(OBJ_FLOOR, CAbstractFactory<CGolemHole>::Create(300, 520));
	dynamic_cast<CGolemHole*>(CObjectManager::Get_Instance()->Get_LastFloor())->Set_HoleType(HOLE_3);

	CObjectManager::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CTurretBroken>::Create(300, 110, DOWN));
	CObjectManager::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CTurretBroken>::Create(300, 610, UP));
}

void CDungeonEditScene::Offset()
{
}
