#include "pch.h"
#include "CDungeonScene.h"
#include "CKeyManager.h"
#include "CObjectManager.h"
#include "CBitManager.h"
#include "CPlayer.h"
#include "CScrollManager.h"
#include "CMapObj.h"
#include "CCollisionManager.h"
#include "CAbstractFactory.h"
#include "CCollisionBox.h"
#include "CScrollWasd.h"
#include "CGolemDoor.h"

CDungeonScene::CDungeonScene()
{
}

void CDungeonScene::Initialize()
{
	ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/background.bmp", L"DungeonBackground");
	static_cast<CPlayer*>(CObjectManager::Get_Instance()->Get_Player())->Set_Pos(WINCX/2, WINCY/2);
	m_fMapXSize = 1024.f;
	m_fMapYSize = 720.f;
	CScrollManager::Get_Instance()->Set_ScrollLock(m_fMapXSize, m_fMapYSize);

	Load_Map();
}

int CDungeonScene::Update()
{
	Key_Input();
	CObjectManager::Get_Instance()->Update();
    return 0;
}

void CDungeonScene::LateUpdate()
{
	CObjectManager::Get_Instance()->Late_Update();
	CScrollManager::Get_Instance()->Scroll_Lock();
}

void CDungeonScene::Render(HDC hDC)
{
	HDC hMemDC = CBitManager::GetInstance()->FindImage(L"DungeonBackground");
	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();
	GdiTransparentBlt(hDC, iScrollX, iScrollY, m_fMapXSize, m_fMapYSize, hMemDC, 0, 0, m_fMapXSize, m_fMapYSize, RGB(0, 0, 0));
	CObjectManager::Get_Instance()->Render(hDC);
}

void CDungeonScene::Release()
{
	CObjectManager::Get_Instance()->Delete_ID(OBJ_MAPOBJ);
	CObjectManager::Get_Instance()->Delete_ID(OBJ_PORTAL);
	CObjectManager::Get_Instance()->RenderListClear();
}

void CDungeonScene::Key_Input()
{
	if (CKeyManager::Get_Instance()->Key_Down(VK_F1)) {
		g_bDevmode = !g_bDevmode;
	}
}

void CDungeonScene::Create_MapObj()
{
}

void CDungeonScene::Offset()
{
}

void CDungeonScene::Load_Map()
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
	MessageBox(g_hWnd, L"MapObj Load", L"¼º°ø", MB_OK);
}