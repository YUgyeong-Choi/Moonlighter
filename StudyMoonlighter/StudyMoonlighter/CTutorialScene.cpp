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
#include "CGolemDoor.h"
#include "CKeyManager.h"
#include "CScrollManager.h"

CTutorialScene::CTutorialScene() :m_iTutorialIndex(0), m_iLimitMoveX(0)
{
	m_TutorialDungeon[0] = new CDungeonScene(L"../Data/SceneMapObj/CTutorialMapObj1.dat", 0, 0);	
	m_TutorialDungeon[1] = new CDungeonScene(L"../Data/SceneMapObj/CTutorialMapObj1.dat", 0 ,1);	
	m_TutorialDungeon[2] = new CDungeonScene(L"../Data/SceneMapObj/CTutorialMapObj1.dat", 0 ,2);	
}

void CTutorialScene::Initialize()
{
	CObjectManager::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CPlayer>::Create(WINCX / 2, WINCY / 2));
	for (auto dungeon : m_TutorialDungeon) {
		dungeon->Initialize();
	}
	m_TutorialDungeon[m_iTutorialIndex]->Load_Map();
}

int CTutorialScene::Update()
{
	Key_Input();
	list<CObject*> protalList = CObjectManager::Get_Instance()->Get_PortalList();
	for (auto portal : protalList) {
		if (static_cast<CGolemDoor*>(portal)->Get_Load_Next()) {
			switch (static_cast<CGolemDoor*>(portal)->Get_DIR())
			{
			case LEFT:
				m_iTutorialIndex--;
				break;
			case RIGHT:
				//CObjectManager::Get_Instance()->Delete_ID(OBJ_MAPOBJ);
				//CObjectManager::Get_Instance()->Delete_ID(OBJ_PORTAL);
				//CObjectManager::Get_Instance()->RenderListClear();
				
				m_iTutorialIndex++;
				m_TutorialDungeon[m_iTutorialIndex]->Load_Map();
				CObjectManager::Get_Instance()->Set_IsMapMove(true);
				break;
			case UP:
				break;
			case DOWN:
				break;
			}
		}
	}

	for (auto dungeon : m_TutorialDungeon) {
		dungeon->Update();
	}
	CObjectManager::Get_Instance()->Update();
    return 0;
}

void CTutorialScene::LateUpdate()
{
	MapMove();
	for (auto dungeon : m_TutorialDungeon) {
		dungeon->LateUpdate();
	}
	CObjectManager::Get_Instance()->Late_Update();
	//CScrollManager::Get_Instance()->Scroll_Lock();
}

void CTutorialScene::Render(HDC hDC)
{
	for (auto dungeon : m_TutorialDungeon) {
		dungeon->Render(hDC);
	}
	CObjectManager::Get_Instance()->Render(hDC);
}

void CTutorialScene::Release()
{
	for (auto dungeon : m_TutorialDungeon) {
		Safe_Delete<CDungeonScene*>(dungeon);
	}
	CObjectManager::Get_Instance()->Delete_ID(OBJ_MAPOBJ);
	CObjectManager::Get_Instance()->Delete_ID(OBJ_PORTAL);
	CObjectManager::Get_Instance()->RenderListClear();
}

void CTutorialScene::Key_Input()
{
	if (CKeyManager::Get_Instance()->Key_Pressing(VK_LEFT))
	{
		CScrollManager::Get_Instance()->Set_ScrollX(5.f);
	}

	if (CKeyManager::Get_Instance()->Key_Pressing(VK_RIGHT))
	{
		CScrollManager::Get_Instance()->Set_ScrollX(-5.f);
	}

	if (CKeyManager::Get_Instance()->Key_Pressing(VK_UP))
	{
		CScrollManager::Get_Instance()->Set_ScrollY(5.f);
	}

	if (CKeyManager::Get_Instance()->Key_Pressing(VK_DOWN))
	{
		CScrollManager::Get_Instance()->Set_ScrollY(-5.f);
	}
}

void CTutorialScene::Create_MapObj()
{
}

void CTutorialScene::Offset()
{
}

void CTutorialScene::MapMove()
{
	if (CObjectManager::Get_Instance()->Get_IsMapMove()) {
		m_iLimitMoveX += 16;
		if (m_iLimitMoveX > 1024) {
			CObjectManager::Get_Instance()->Set_IsMapMove(false);
			m_iLimitMoveX = 0;
		}
		else {
			CObjectManager::Get_Instance()->Set_MapMoveX(m_iLimitMoveX + ((m_iTutorialIndex-1) * 1024));
		}
	}
}



