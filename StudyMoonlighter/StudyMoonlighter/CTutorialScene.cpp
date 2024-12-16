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

CTutorialScene::CTutorialScene() :m_iTutorialIndex(0)
{
	m_TutorialDungeon[0] = new CDungeonScene(L"../Data/SceneMapObj/CTutorialMapObj1.dat", 0, 0);	
	m_TutorialDungeon[1] = new CDungeonScene(L"../Data/SceneMapObj/CTutorialMapObj1.dat", 0 ,1);	
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
				CObjectManager::Get_Instance()->Delete_ID(OBJ_MAPOBJ);
				CObjectManager::Get_Instance()->Delete_ID(OBJ_PORTAL);
				CObjectManager::Get_Instance()->RenderListClear();
				m_iTutorialIndex++;
				m_TutorialDungeon[m_iTutorialIndex]->Load_Map();
				break;
			case UP:
				break;
			case DOWN:
				break;
			}
		}
	}
	m_TutorialDungeon[m_iTutorialIndex]->Update();
    return 0;
}

void CTutorialScene::LateUpdate()
{
	m_TutorialDungeon[m_iTutorialIndex]->LateUpdate();
	CScrollManager::Get_Instance()->Scroll_Lock();
}

void CTutorialScene::Render(HDC hDC)
{
	for (auto dungeon : m_TutorialDungeon) {
		dungeon->Render(hDC);
	}
}

void CTutorialScene::Release()
{
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



