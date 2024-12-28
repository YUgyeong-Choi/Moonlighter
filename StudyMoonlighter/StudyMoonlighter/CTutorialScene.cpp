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
#include "CUiManager.h"
#include "CSceneManager.h"

CTutorialScene::CTutorialScene() :m_iTutorialIndex(0), m_dir(DIR_END), m_iMove(0), m_bMapMove(false)
{
	m_TutorialDungeon[0] = new CDungeonScene(L"../Data/MapObj/Tutorial1.dat", L"../Data/MapMonster/Tutorial1.dat", 0, 0);
	m_TutorialDungeon[1] = new CDungeonScene(L"../Data/MapObj/Tutorial2.dat", L"../Data/MapMonster/Tutorial2.dat", 0, 1);
	m_TutorialDungeon[2] = new CDungeonScene(L"../Data/MapObj/Tutorial3.dat", L"../Data/MapMonster/Tutorial3.dat", 0, 2);
	m_TutorialDungeon[3] = new CDungeonScene(L"../Data/MapObj/Tutorial4.dat", L"../Data/MapMonster/Tutorial4.dat", 0, 3);
}

void CTutorialScene::Initialize()
{
	CScrollManager::Get_Instance()->Set_ScrollLock(4000, 760);
	CScrollManager::Get_Instance()->Set_Scroll(0, 0);
	CObjectManager::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CPlayer>::Create(WINCX / 2, WINCY / 2));
	CObjectManager::Get_Instance()->Set_MapIndex(0, 0, RIGHT);
	for (auto dungeon : m_TutorialDungeon) {
		dungeon->Initialize();
	}
	m_TutorialDungeon[m_iTutorialIndex]->Load_Map();
}

int CTutorialScene::Update()
{
	list<CObject*> protalList = CObjectManager::Get_Instance()->Get_PortalList();
	for (auto portal : protalList) {
		if (static_cast<CGolemDoor*>(portal)->Get_Load_Next()) {
			switch (static_cast<CGolemDoor*>(portal)->Get_DIR())
			{
			case LEFT:
				m_dir = LEFT;
				m_iTutorialIndex--;
				m_bMapMove = true;
				CObjectManager::Get_Instance()->Get_Player()->Set_Pos((1024 * m_iTutorialIndex) + 900, WINCY / 2);
				CObjectManager::Get_Instance()->Set_MapIndex(m_iTutorialIndex, 0, LEFT);
				m_TutorialDungeon[m_iTutorialIndex]->Load_Map();
				break;
			case RIGHT:
				m_dir = RIGHT;
				m_iTutorialIndex++;
				if (m_iTutorialIndex == 4) {
					CObjectManager::Get_Instance()->Delete_ALL();
					CSceneManager::GetInstance()->SetScene(SC_MENU);
					return 0;
				}
				m_bMapMove = true;
				CObjectManager::Get_Instance()->Get_Player()->Set_Pos((1024*m_iTutorialIndex) + 150, WINCY / 2);
				CObjectManager::Get_Instance()->Set_MapIndex(m_iTutorialIndex, 0, RIGHT);
				m_TutorialDungeon[m_iTutorialIndex]->Load_Map();
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
	Offset();
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
	CUiManager::GetInstance()->Render(hDC);
}

void CTutorialScene::Release()
{
	for (auto dungeon : m_TutorialDungeon) {
		Safe_Delete<CDungeonScene*>(dungeon);
	}
	CObjectManager::Get_Instance()->Delete_ID(OBJ_MAPOBJ);
	CObjectManager::Get_Instance()->Delete_ID(OBJ_PORTAL);
	CObjectManager::Get_Instance()->Delete_ID(OBJ_PLAYER);
	CObjectManager::Get_Instance()->RenderListClear();
}

void CTutorialScene::Key_Input()
{
}

void CTutorialScene::Create_MapObj()
{
}

void CTutorialScene::Offset()
{
	if (m_bMapMove) {
		m_iMove += 16;
		if (m_iMove > 1024) {
			m_bMapMove = false;
			m_iMove = 0;
		}
		else {
			switch (m_dir)
			{
			case LEFT:
				CScrollManager::Get_Instance()->Set_ScrollX(16);
				break;
			case RIGHT:
				CScrollManager::Get_Instance()->Set_ScrollX(-16);
				break;
			case UP:
				break;
			case DOWN:
				break;
			}
		}
	}
}



