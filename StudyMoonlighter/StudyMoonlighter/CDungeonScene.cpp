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
#include "CFileManager.h"


CDungeonScene::CDungeonScene():m_iMapXIndex(0), m_iMapYIndex(0), m_IsLoad(false), pFilePath(nullptr), pFilePath2(nullptr)
{
}

CDungeonScene::CDungeonScene(const TCHAR* _path1, const TCHAR* _path2, int _x, int _y) :m_iMapXIndex(0), m_iMapYIndex(0), m_IsLoad(false), pFilePath(nullptr), pFilePath2(nullptr)
{
	pFilePath = _path1;
	pFilePath2 = _path2;
	m_iMapXIndex = _x;
	m_iMapYIndex = _y;
}

void CDungeonScene::Initialize()
{
	ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/background.bmp", L"DungeonBackground");
	m_fMapXSize = 1024.f;
	m_fMapYSize = 720.f;
}

int CDungeonScene::Update()
{
	Key_Input();
	
    return 0;
}

void CDungeonScene::LateUpdate()
{
}

void CDungeonScene::Render(HDC hDC)
{
	HDC hMemDC = CBitManager::GetInstance()->FindImage(L"DungeonBackground");
	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();
	GdiTransparentBlt(hDC, (m_iMapYIndex *m_fMapXSize) + iScrollX, (m_iMapXIndex * (int)m_fMapYSize) + iScrollY, (int)m_fMapXSize, (int)m_fMapYSize, hMemDC, 0, 0, (int)m_fMapXSize, (int)m_fMapYSize, RGB(0, 0, 0));
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
	if (!m_IsLoad) {
		CObjectManager::Get_Instance()->Delete_ID(OBJ_MAPOBJ);
		CObjectManager::Get_Instance()->Delete_ID(OBJ_PORTAL);
		CObjectManager::Get_Instance()->Delete_ID(OBJ_MONSTER);
		CObjectManager::Get_Instance()->Delete_ID(OBJ_MONSTER_BULLET);

		m_IsLoad = true;
		CFileManager::GetInstance()->Load_MapObjFile(pFilePath2, m_iMapXIndex, m_iMapYIndex);
		CFileManager::GetInstance()->Load_MonsterFile(pFilePath, m_iMapXIndex, m_iMapYIndex);
	}
	else {
		CObjectManager::Get_Instance()->Delete_ID(OBJ_MAPOBJ);
		CObjectManager::Get_Instance()->Delete_ID(OBJ_PORTAL);
		CObjectManager::Get_Instance()->Delete_ID(OBJ_MONSTER);
		CObjectManager::Get_Instance()->Delete_ID(OBJ_MONSTER_BULLET);
		CFileManager::GetInstance()->Load_MapObjFile(pFilePath2, m_iMapXIndex, m_iMapYIndex);
	}
}
