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
#include "CScrollRoll.h"
#include "CGolemHole.h"

CDungeonScene::CDungeonScene():m_iMapXIndex(0), m_iMapYIndex(0)
{
}

CDungeonScene::CDungeonScene(const TCHAR* _pFilePath, int _x, int _y) :m_iMapXIndex(0), m_iMapYIndex(0)
{
	pFilePath = _pFilePath;
	m_iMapXIndex = _x;
	m_iMapYIndex = _y;
}

void CDungeonScene::Initialize()
{
	ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/background.bmp", L"DungeonBackground");
	m_fMapXSize = 1024.f;
	m_fMapYSize = 720.f;
	CScrollManager::Get_Instance()->Set_ScrollLock(2000, 2000);
}

int CDungeonScene::Update()
{
	Key_Input();
	
    return 0;
}

void CDungeonScene::LateUpdate()
{
	CScrollManager::Get_Instance()->Scroll_Lock();
}

void CDungeonScene::Render(HDC hDC)
{
	HDC hMemDC = CBitManager::GetInstance()->FindImage(L"DungeonBackground");
	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();
	GdiTransparentBlt(hDC, (m_iMapYIndex *m_fMapXSize) - (CObjectManager::Get_Instance()->Get_MapMoveX()) + iScrollX, (m_iMapXIndex * m_fMapYSize) + iScrollY, m_fMapXSize, m_fMapYSize, hMemDC, 0, 0, m_fMapXSize, m_fMapYSize, RGB(0, 0, 0));
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
	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD	dwByte(0);
	DIRECTION _dir;
	CMapObj	_MapObj;


	while (true)
	{
		bool a = ReadFile(hFile, &_MapObj, sizeof(CMapObj), &dwByte, NULL);

		if (0 == dwByte)
			break;

		if (_MapObj.Get_MapObjType() == COLLISION) {
			CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CCollisionBox>::Create((m_iMapYIndex * m_fMapXSize) + _MapObj.Get_Info().fX, _MapObj.Get_Info().fY, _MapObj.Get_Info().fCX, _MapObj.Get_Info().fCY));
		}
		else if (_MapObj.Get_MapObjType() == SCROLLWASD) {
			CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CScrollWasd>::Create((m_iMapYIndex * m_fMapXSize) + _MapObj.Get_Info().fX, _MapObj.Get_Info().fY, 0, 0));
		}
		else if (_MapObj.Get_MapObjType() == SCROLLROLL) {
			CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CScrollRoll>::Create((m_iMapYIndex * m_fMapXSize) + _MapObj.Get_Info().fX, _MapObj.Get_Info().fY, 0, 0));
		}
		else if (_MapObj.Get_MapObjType() == GOLEM_DOOR) {
			CObjectManager::Get_Instance()->Add_Object(OBJ_PORTAL, CAbstractFactory<CGolemDoor>::Create((m_iMapYIndex * m_fMapXSize) + _MapObj.Get_Info().fX, _MapObj.Get_Info().fY, _MapObj.Get_Info().fCX, _MapObj.Get_Info().fCY));
			bool b = ReadFile(hFile, &_dir, sizeof(DIRECTION), &dwByte, NULL);
			static_cast<CGolemDoor*>(CObjectManager::Get_Instance()->Get_LastPortal())->Set_DIR(_dir);
		}
		else if (_MapObj.Get_MapObjType() == GOLEM_HOLE) {
			CObjectManager::Get_Instance()->Add_Object(OBJ_FLOOR, CAbstractFactory<CGolemHole>::Create((m_iMapYIndex * m_fMapXSize) + _MapObj.Get_Info().fX, _MapObj.Get_Info().fY, _MapObj.Get_Info().fCX, _MapObj.Get_Info().fCY));
			HOLETYPE _type;
			bool b = ReadFile(hFile, &_type, sizeof(HOLETYPE), &dwByte, NULL);
			dynamic_cast<CGolemHole*>(CObjectManager::Get_Instance()->Get_LastFloor())->Set_HoleType(_type);
		}
	}

	CloseHandle(hFile);
	
}
