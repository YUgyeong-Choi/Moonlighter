#include "pch.h"
#include "CDungeonEditScene.h"
#include "CBitManager.h"
#include "CScrollManager.h"
#include "CTileManager.h"
#include "CKeyManager.h"
#include "CObjectManager.h"
#include "CMapObj.h"
#include "CScrollWasd.h"
#include "CCollisionBox.h"
#include "CAbstractFactory.h"
#include "CGolemDoor.h"

CDungeonEditScene::CDungeonEditScene():m_bIsShowTile(false)
{
}

void CDungeonEditScene::Initialize()
{
	ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/background.bmp", L"DungeonBackground");
	ADD_BMP(L"../MoonlighterAssets/Tile/CanWalk48.bmp", L"CanWalk");
	ADD_BMP(L"../MoonlighterAssets/Tile/CantWalk48.bmp", L"CantWalk");
	m_fMapXSize = 1024.f;
	m_fMapYSize = 720.f;
	CTileManager::Get_Instance()->Initialize();
	Create_MapObj();
}

int CDungeonEditScene::Update()
{
	Key_Input();
	CObjectManager::Get_Instance()->Update();
	CTileManager::Get_Instance()->Update();
    return 0;
}

void CDungeonEditScene::LateUpdate()
{
	CObjectManager::Get_Instance()->Late_Update();
	CTileManager::Get_Instance()->Late_Update();
}

void CDungeonEditScene::Render(HDC hDC)
{
	Rectangle(hDC, -100, -100, 1100, 1100);
	HDC hMemDC = CBitManager::GetInstance()->FindImage(L"DungeonBackground");
	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();
	GdiTransparentBlt(hDC, iScrollX, iScrollY, (int)m_fMapXSize, (int)m_fMapYSize, hMemDC, 0, 0, (int)m_fMapXSize, (int)m_fMapYSize, RGB(0, 0, 0));
	CObjectManager::Get_Instance()->Render(hDC);
	if (m_bIsShowTile) {
		CTileManager::Get_Instance()->Render(hDC);
	}
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
		m_bIsShowTile = !m_bIsShowTile;
		g_bDevmode = !g_bDevmode;
	}

	if (m_bIsShowTile) {
		if (CKeyManager::Get_Instance()->Key_Down(VK_LBUTTON))
		{
			POINT	ptMouse{};
			GetCursorPos(&ptMouse);
			ScreenToClient(g_hWnd, &ptMouse);

			ptMouse.x -= (int)CScrollManager::Get_Instance()->Get_ScrollX() + 80;
			ptMouse.y -= (int)CScrollManager::Get_Instance()->Get_ScrollY() + 96;

			CTileManager::Get_Instance()->Picking_Tile(ptMouse);
		}
	}


	if (CKeyManager::Get_Instance()->Key_Down('O'))
	{
		CTileManager::Get_Instance()->Save_Tile();
		SaveMapObj();
	}

	if (CKeyManager::Get_Instance()->Key_Down('P'))
	{
		CTileManager::Get_Instance()->Load_Tile();
		LoadMapObj();
	}
}

void CDungeonEditScene::Create_MapObj()
{
	//CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CCollisionBox>::Create(WINCX / 2, 30, 1024, 60));
	//CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CCollisionBox>::Create(WINCX / 2, 690, 1024, 60));
	//CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CCollisionBox>::Create(30, WINCY / 2, 60, 720));
	//CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CCollisionBox>::Create(994, WINCY / 2, 60, 720));
	//CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CScrollWasd>::Create(WINCX / 2, 60, 0, 0));
	//CObjectManager::Get_Instance()->Add_Object(OBJ_PORTAL, CAbstractFactory<CGolemDoor>::Create(980, WINCY / 2, 80, 50));
	//dynamic_cast<CGolemDoor*>(CObjectManager::Get_Instance()->Get_LastPortal())->Set_DIR(RIGHT);
}

void CDungeonEditScene::Offset()
{
}


void CDungeonEditScene::SaveMapObj()
{
	HANDLE hFile = CreateFile(L"../Data/SceneMapObj/CTutorialMapObj1.dat", GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD	dwByte(0);
	DIRECTION _dir;
	list<CObject*> mapObjList = CObjectManager::Get_Instance()->Get_MapObjList();

	for (auto& mapObj : mapObjList)
	{
		CMapObj* obj = static_cast<CMapObj*>(mapObj);
		WriteFile(hFile, obj, sizeof(CMapObj), &dwByte, NULL);
	}

	list<CObject*> protalList = CObjectManager::Get_Instance()->Get_PortalList(); //여기는 골렘 던전만 있다고 가정
	for (auto& mapObj : protalList)
	{
		_dir = static_cast<CGolemDoor*>(mapObj)->Get_DIR();
		CMapObj* obj = static_cast<CMapObj*>(mapObj);
		WriteFile(hFile, mapObj, sizeof(CMapObj), &dwByte, NULL);
		WriteFile(hFile, &_dir, sizeof(DIRECTION), &dwByte, NULL);
	}


	CloseHandle(hFile);
	MessageBox(g_hWnd, L"MapObj Save", L"성공", MB_OK);
}

void CDungeonEditScene::LoadMapObj()
{
	HANDLE hFile = CreateFile(L"../Data/SceneMapObj/CTutorialMapObj1.dat", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD	dwByte(0);
	DIRECTION _dir;
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
		else if (_MapObj.Get_MapObjType() == GOLEM_DOOR) {
			bool b = ReadFile(hFile, &_dir, sizeof(DIRECTION), &dwByte, NULL);
			CObjectManager::Get_Instance()->Add_Object(OBJ_PORTAL, CAbstractFactory<CGolemDoor>::Create(_MapObj.Get_Info().fX, _MapObj.Get_Info().fY, _MapObj.Get_Info().fCX, _MapObj.Get_Info().fCY));
			static_cast<CGolemDoor*>(CObjectManager::Get_Instance()->Get_LastPortal())->Set_DIR(_dir);
		}

	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"MapObj Load", L"성공", MB_OK);
}