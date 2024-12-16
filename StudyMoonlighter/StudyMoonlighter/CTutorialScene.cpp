#include "pch.h"
#include "CTutorialScene.h"
#include "CKeyManager.h"
#include "CObjectManager.h"
#include "CBitManager.h"
#include "CPlayer.h"
#include "CScrollManager.h"
#include "CAbstractFactory.h"
#include "CCollisionBox.h"
#include "CScrollWasd.h"

CTutorialScene::CTutorialScene()
{
}

void CTutorialScene::Initialize()
{
	ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/background.bmp", L"DungeonBackground");
	CObjectManager::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CPlayer>::Create(WINCX / 2, WINCY / 2));
	m_fMapXSize = 1024.f;
	m_fMapYSize = 720.f;
	CScrollManager::Get_Instance()->Set_ScrollLock(m_fMapXSize, m_fMapYSize);

	Create_MapObj();
}

int CTutorialScene::Update()
{
	Key_Input();
	CObjectManager::Get_Instance()->Update();
    return 0;
}

void CTutorialScene::LateUpdate()
{
	CObjectManager::Get_Instance()->Late_Update();
	CScrollManager::Get_Instance()->Scroll_Lock();
}

void CTutorialScene::Render(HDC hDC)
{
	HDC hMemDC = CBitManager::GetInstance()->FindImage(L"DungeonBackground");
	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();
	GdiTransparentBlt(hDC, iScrollX, iScrollY, m_fMapXSize, m_fMapYSize, hMemDC, 0, 0, m_fMapXSize, m_fMapYSize, RGB(0, 0, 0));
	CObjectManager::Get_Instance()->Render(hDC);
}

void CTutorialScene::Release()
{
	CObjectManager::Get_Instance()->Delete_ID(OBJ_MAPOBJ);
	CObjectManager::Get_Instance()->Delete_ID(OBJ_PORTAL);
	CObjectManager::Get_Instance()->RenderListClear();
}

void CTutorialScene::Key_Input()
{
	if (CKeyManager::Get_Instance()->Key_Down(VK_F1)) {
		g_bDevmode = !g_bDevmode;
	}
}

void CTutorialScene::Create_MapObj()
{
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CCollisionBox>::Create(WINCX/2, 30, 1024, 60));
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CCollisionBox>::Create(WINCX/2, 690, 1024, 60));
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CCollisionBox>::Create(30, WINCY/2, 60, 720));
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CCollisionBox>::Create(994, WINCY/2, 60, 720));
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CScrollWasd>::Create(WINCX/2,60, 0, 0));
}

void CTutorialScene::Offset()
{

}


//void CTutorialScene::SaveMapObj()
//{
//	HANDLE hFile = CreateFile(L"../Data/Tile.dat", GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
//
//	if (INVALID_HANDLE_VALUE == hFile)
//		return;
//
//	DWORD	dwByte(0);
//	list<CObject*> mapObjList = CObjectManager::Get_Instance()->Get_MapObjList();
//	for (auto& mapObj : mapObjList)
//	{
//
//		WriteFile(hFile, mapObj, sizeof(CObject), &dwByte, NULL);
//	}
//
//	CloseHandle(hFile);
//	MessageBox(g_hWnd, L"Tile Save", L"성공", MB_OK);
//}
//
//void CTutorialScene::LoadMapObj()
//{
//	HANDLE hFile = CreateFile(L"../Data/Tile.dat", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
//
//	if (INVALID_HANDLE_VALUE == hFile)
//		return;
//
//	DWORD	dwByte(0);
//	INFO	tInfo{};
//
//	Release();
//
//	while (true)
//	{
//		bool a = ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, NULL);
//
//		if (0 == dwByte)
//			break;
//
//
//		CObject* pTile = CAbstractFactory<CTile>::Create(tTile.fX, tTile.fY);
//		static_cast<CTile*>(pTile)->Set_DrawID(iDrawID);
//
//		m_vecTile.push_back(pTile);
//	}
//
//	CloseHandle(hFile);
//	MessageBox(g_hWnd, L"Load Save", L"성공", MB_OK);
//}
