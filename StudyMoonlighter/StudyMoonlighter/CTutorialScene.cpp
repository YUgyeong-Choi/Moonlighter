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
#include "CSoundManager.h"
#include "CGolemScroll.h"
#include "CInventoryManager.h"


CTutorialScene::CTutorialScene() :m_iTutorialIndex(0), m_dir(DIR_END), m_iMove(0), m_bMapMove(false)
{
	m_TutorialDungeon[0] = new CDungeonScene(L"../Data/MapObj/Tutorial1.dat", L"../Data/MapMonster/Tutorial1.dat", 0, 0);
	m_TutorialDungeon[1] = new CDungeonScene(L"../Data/MapObj/Tutorial2.dat", L"../Data/MapMonster/Tutorial2.dat", 0, 1);
	m_TutorialDungeon[2] = new CDungeonScene(L"../Data/MapObj/Tutorial3.dat", L"../Data/MapMonster/Tutorial3.dat", 0, 2);
	m_TutorialDungeon[3] = new CDungeonScene(L"../Data/MapObj/Tutorial4.dat", L"../Data/MapMonster/Tutorial4.dat", 0, 3);
}

void CTutorialScene::Initialize()
{
	CSoundManager::Get_Instance()->StopAll();
	CSoundManager::Get_Instance()->PlayBGM(L"golem_dungeon_floor.wav", g_fBackgroundVolume, true);
	CScrollManager::Get_Instance()->Set_ScrollLock(4096, 720);
	CScrollManager::Get_Instance()->Set_Scroll(0, 0);
	CObjectManager::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CPlayer>::Create(WINCX / 2, WINCY / 2));
	CObjectManager::Get_Instance()->Set_MapIndex(0, 0, RIGHT);
	for (auto dungeon : m_TutorialDungeon) {
		dungeon->Initialize();
	}
	m_TutorialDungeon[m_iTutorialIndex]->Load_Map();

	CInventoryManager::GetInstance()->AddItem(SWORD);
	CUiManager::GetInstance()->Set_UiType(UI_END);

	Create_MapObj();
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
					CSceneManager::GetInstance()->SetScene(SC_VILLAGE);
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
	CUiManager::GetInstance()->Update();
	CInventoryManager::GetInstance()->Update();
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
	CObjectManager::Get_Instance()->Delete_ALL();
	CObjectManager::Get_Instance()->RenderListClear();
	CObjectManager::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CPlayer>::Create(2040, 646));
}

void CTutorialScene::Key_Input()
{
}

void CTutorialScene::Create_MapObj()
{
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CGolemScroll>::Create(WINCX / 2, 500));
	static_cast<CGolemScroll*>(CObjectManager::Get_Instance()->Get_LastMapObj())->Set_Text(L"Ʃ�丮��");
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



