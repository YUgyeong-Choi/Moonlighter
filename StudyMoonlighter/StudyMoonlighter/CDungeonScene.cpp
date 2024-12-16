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

CDungeonScene::CDungeonScene():m_iMapXIndex(0), m_iMapYIndex(0), m_fMoveIndex(1), m_bMove(false)
{
}

CDungeonScene::CDungeonScene(const TCHAR* _pFilePath, int _x, int _y) : m_fMoveIndex(1), m_bMove(false)
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
	if (m_bMove) {
		m_fMoveIndex *= 1.3;
		if (m_fMoveIndex > 1024) {
			m_bMove = false;
			m_fMoveIndex = 1;
		}
	}
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
	GdiTransparentBlt(hDC, (m_iMapYIndex *m_fMapXSize) - m_fMoveIndex + iScrollX, (m_iMapXIndex * m_fMapYSize) + iScrollY, m_fMapXSize, m_fMapYSize, hMemDC, 0, 0, m_fMapXSize, m_fMapYSize, RGB(0, 0, 0));
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

	Release();

	while (true)
	{
		bool a = ReadFile(hFile, &_MapObj, sizeof(CMapObj), &dwByte, NULL);

		if (0 == dwByte)
			break;

		if (_MapObj.Get_MapObjType() == COLLISION) {
			CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CCollisionBox>::Create((m_iMapYIndex * m_fMapXSize) + _MapObj.Get_Info().fX, (m_iMapXIndex * m_fMapYSize) + _MapObj.Get_Info().fY, _MapObj.Get_Info().fCX, _MapObj.Get_Info().fCY));
		}
		else if (_MapObj.Get_MapObjType() == SCROLLWASD) {
			CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CScrollWasd>::Create((m_iMapYIndex * m_fMapXSize) + _MapObj.Get_Info().fX, (m_iMapXIndex * m_fMapYSize) + _MapObj.Get_Info().fY, 0, 0));
		}
		else if (_MapObj.Get_MapObjType() == GOLEM_DOOR) {
			bool b = ReadFile(hFile, &_dir, sizeof(DIRECTION), &dwByte, NULL);
			CObjectManager::Get_Instance()->Add_Object(OBJ_PORTAL, CAbstractFactory<CGolemDoor>::Create((m_iMapYIndex * m_fMapXSize) + _MapObj.Get_Info().fX, (m_iMapXIndex * m_fMapYSize) + _MapObj.Get_Info().fY, _MapObj.Get_Info().fCX, _MapObj.Get_Info().fCY));
			static_cast<CGolemDoor*>(CObjectManager::Get_Instance()->Get_LastPortal())->Set_DIR(_dir);
		}

	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"MapObj Load", L"����", MB_OK);
}
