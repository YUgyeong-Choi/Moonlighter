#include "pch.h"
#include "CDungeonEditScene.h"
#include "CBitManager.h"
#include "CScrollManager.h"
#include "CKeyManager.h"
#include "CObjectManager.h"
#include "CMapObj.h"
#include "CScrollWasd.h"
#include "CCollisionBox.h"
#include "CAbstractFactory.h"
#include "CGolemDoor.h"
#include "CScrollRoll.h"
#include "CGolemHole.h"

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
	Rectangle(hDC, -100, -100, 1100, 1100);
	HDC hMemDC = CBitManager::GetInstance()->FindImage(L"DungeonBackground");
	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();
	GdiTransparentBlt(hDC, iScrollX, iScrollY, (int)m_fMapXSize, (int)m_fMapYSize, hMemDC, 0, 0, (int)m_fMapXSize, (int)m_fMapYSize, RGB(0, 0, 0));
	CObjectManager::Get_Instance()->Render(hDC);
}

void CDungeonEditScene::Release()
{
	CObjectManager::Get_Instance()->Delete_ID(OBJ_MAPOBJ);
	CObjectManager::Get_Instance()->Delete_ID(OBJ_PORTAL);
	CObjectManager::Get_Instance()->RenderListClear();
}

void CDungeonEditScene::Key_Input()
{

	if (CKeyManager::Get_Instance()->Key_Down(VK_F1)) {
		g_bDevmode = !g_bDevmode;
	}

	if (CKeyManager::Get_Instance()->Key_Down('O'))
	{
		SaveMapObj();
	}

	if (CKeyManager::Get_Instance()->Key_Down('P'))
	{
		LoadMapObj();
	}
}

void CDungeonEditScene::Create_MapObj()
{
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CCollisionBox>::Create(WINCX / 2, 30, 1024, 60));
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CCollisionBox>::Create(WINCX / 2, 690, 1024, 60));
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CCollisionBox>::Create(30, WINCY / 2, 60, 720));
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CCollisionBox>::Create(994, WINCY / 2, 60, 720));

	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CScrollRoll>::Create(WINCX / 2, 60, 0, 0));
	dynamic_cast<CScrollRoll*>(CObjectManager::Get_Instance()->Get_LastMapObj())->Set_ScrollType(SCROLL_PROJECTILE);

	CObjectManager::Get_Instance()->Add_Object(OBJ_PORTAL, CAbstractFactory<CGolemDoor>::Create(50, WINCY / 2));
	dynamic_cast<CGolemDoor*>(CObjectManager::Get_Instance()->Get_LastPortal())->Set_DIR(LEFT);
	CObjectManager::Get_Instance()->Add_Object(OBJ_PORTAL, CAbstractFactory<CGolemDoor>::Create(980, WINCY / 2));
	dynamic_cast<CGolemDoor*>(CObjectManager::Get_Instance()->Get_LastPortal())->Set_DIR(RIGHT);

	CObjectManager::Get_Instance()->Add_Object(OBJ_FLOOR, CAbstractFactory<CGolemHole>::Create(300, 200));
	dynamic_cast<CGolemHole*>(CObjectManager::Get_Instance()->Get_LastFloor())->Set_HoleType(HOLE_3);

	CObjectManager::Get_Instance()->Add_Object(OBJ_FLOOR, CAbstractFactory<CGolemHole>::Create(700, 200));
	dynamic_cast<CGolemHole*>(CObjectManager::Get_Instance()->Get_LastFloor())->Set_HoleType(HOLE_3);

	CObjectManager::Get_Instance()->Add_Object(OBJ_FLOOR, CAbstractFactory<CGolemHole>::Create(300, 520));
	dynamic_cast<CGolemHole*>(CObjectManager::Get_Instance()->Get_LastFloor())->Set_HoleType(HOLE_3);

	CObjectManager::Get_Instance()->Add_Object(OBJ_FLOOR, CAbstractFactory<CGolemHole>::Create(700, 520));
	dynamic_cast<CGolemHole*>(CObjectManager::Get_Instance()->Get_LastFloor())->Set_HoleType(HOLE_3);
}

void CDungeonEditScene::Offset()
{
}


void CDungeonEditScene::SaveMapObj()
{
	HANDLE hFile = CreateFile(L"../Data/CTutorialMapObj3.dat", GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD	dwByte(0);
	DIRECTION _dir;
	list<CObject*> mapObjList = CObjectManager::Get_Instance()->Get_MapObjList();
	for (auto& mapObj : mapObjList)
	{
		CMapObj* obj = static_cast<CMapObj*>(mapObj);
		WriteFile(hFile, obj, sizeof(CMapObj), &dwByte, NULL);

		if (dynamic_cast<CMapObj*>(mapObj)->Get_MapObjType() == SCROLLROLL) {
			TUTORIALSCROLL _type;
			_type = static_cast<CScrollRoll*>(mapObj)->Get_ScrollType();
			WriteFile(hFile, &_type, sizeof(TUTORIALSCROLL), &dwByte, NULL);
		}
	}

	list<CObject*> protalList = CObjectManager::Get_Instance()->Get_PortalList();
	for (auto& mapObj : protalList)
	{
		_dir = static_cast<CGolemDoor*>(mapObj)->Get_DIR();
		CMapObj* obj = static_cast<CMapObj*>(mapObj);
		WriteFile(hFile, mapObj, sizeof(CMapObj), &dwByte, NULL);
		WriteFile(hFile, &_dir, sizeof(DIRECTION), &dwByte, NULL);
	}

	list<CObject*> floorlList = CObjectManager::Get_Instance()->Get_FloorList();
	for (auto& mapObj : floorlList)
	{
		HOLETYPE _type = dynamic_cast<CGolemHole*>(mapObj)->Get_HoleType();
		CMapObj* obj = static_cast<CMapObj*>(mapObj);
		WriteFile(hFile, obj, sizeof(CMapObj), &dwByte, NULL);
		WriteFile(hFile, &_type, sizeof(HOLETYPE), &dwByte, NULL);
	}



	CloseHandle(hFile);
	MessageBox(g_hWnd, L"MapObj Save", L"성공", MB_OK);
}

void CDungeonEditScene::LoadMapObj()
{
	HANDLE hFile = CreateFile(L"../Data/CTutorialMapObj3.dat", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD	dwByte(0);
	CMapObj	_MapObj;

	Release();

	while (true)
	{
		bool a = ReadFile(hFile, &_MapObj, sizeof(CMapObj), &dwByte, NULL);

		if (0 == dwByte)
			break;

		if (_MapObj.Get_MapObjType() == COLLISION) {
			CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CCollisionBox>::Create(_MapObj.Get_Info().fX, _MapObj.Get_Info().fY, _MapObj.Get_Info().fCX, _MapObj.Get_Info().fCY));
		}
		else if (_MapObj.Get_MapObjType() == SCROLLWASD) {
			CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CScrollWasd>::Create(_MapObj.Get_Info().fX, _MapObj.Get_Info().fY, 0, 0));
		}
		else if (_MapObj.Get_MapObjType() == SCROLLROLL) {
			CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CScrollRoll>::Create(_MapObj.Get_Info().fX, _MapObj.Get_Info().fY, 0, 0));
			TUTORIALSCROLL _type;
			bool b= ReadFile(hFile, &_type, sizeof(TUTORIALSCROLL), &dwByte, NULL);
			static_cast<CScrollRoll*>(CObjectManager::Get_Instance()->Get_LastMapObj())->Set_ScrollType(_type);
		}
		else if (_MapObj.Get_MapObjType() == GOLEM_DOOR) {
			CObjectManager::Get_Instance()->Add_Object(OBJ_PORTAL, CAbstractFactory<CGolemDoor>::Create(_MapObj.Get_Info().fX, _MapObj.Get_Info().fY, _MapObj.Get_Info().fCX, _MapObj.Get_Info().fCY));
			DIRECTION _dir;
			bool b = ReadFile(hFile, &_dir, sizeof(DIRECTION), &dwByte, NULL);
			static_cast<CGolemDoor*>(CObjectManager::Get_Instance()->Get_LastPortal())->Set_DIR(_dir);
		}
		else if (_MapObj.Get_MapObjType() == GOLEM_HOLE) {
			CObjectManager::Get_Instance()->Add_Object(OBJ_FLOOR, CAbstractFactory<CGolemHole>::Create(_MapObj.Get_Info().fX, _MapObj.Get_Info().fY, _MapObj.Get_Info().fCX, _MapObj.Get_Info().fCY));
			HOLETYPE _type;
			bool b = ReadFile(hFile, &_type, sizeof(HOLETYPE), &dwByte, NULL);
			dynamic_cast<CGolemHole*>(CObjectManager::Get_Instance()->Get_LastFloor())->Set_HoleType(_type);
		}

	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"MapObj Load", L"성공", MB_OK);
}