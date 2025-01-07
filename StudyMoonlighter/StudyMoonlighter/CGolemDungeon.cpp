#include "pch.h"
#include "CGolemDungeon.h"
#include "CObjectManager.h"
#include "CScrollManager.h"
#include "CGolemDoor.h"
#include "CUiManager.h"
#include "CAbstractFactory.h"
#include "CPlayer.h"
#include "CGolemScroll.h"
#include "CSoundManager.h"
#include "CBitManager.h"
#include "CKeyManager.h"

CGolemDungeon::CGolemDungeon():m_iTutorialIndeX(0), m_iTutorialIndeY(0), m_dir(DIR_END), m_iMove(0), m_bMapMove(false)
{
	for (auto& row : m_TutorialDungeon) {
		for (auto& dungeon : row) {
			dungeon = nullptr;
		}
	}

	m_TutorialDungeon[1][1] = new CDungeonScene(L"../Data/MapObj/GolemDungeon0.dat", L"../Data/MapMonster/GolemDungeon0.dat", 1, 1);
	m_TutorialDungeon[1][0] = new CDungeonScene(L"../Data/MapObj/GolemDungeon1.dat", L"../Data/MapMonster/GolemDungeon1.dat", 1, 0);
	m_TutorialDungeon[0][1] = new CDungeonScene(L"../Data/MapObj/GolemDungeon2.dat", L"../Data/MapMonster/GolemDungeon2.dat", 0, 1);
	m_TutorialDungeon[1][2] = new CDungeonScene(L"../Data/MapObj/GolemDungeon3.dat", L"../Data/MapMonster/GolemDungeon3.dat", 1, 2);
	m_TutorialDungeon[1][3] = new CDungeonScene(L"../Data/MapObj/GolemDungeon4.dat", L"../Data/MapMonster/GolemDungeon4.dat", 1, 3);
}

void CGolemDungeon::Initialize()
{
	ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/slime_puddle1.bmp", L"DungeonBackgroundSlime");
	CSoundManager::Get_Instance()->StopAll();
	CSoundManager::Get_Instance()->PlayBGM(L"golem_dungeon_floor.wav", g_fBackgroundVolume, true);

	m_iTutorialIndeX = 1;
	m_iTutorialIndeY = 1;

	CScrollManager::Get_Instance()->Set_ScrollLock(4096, 1440);
	CScrollManager::Get_Instance()->Set_Scroll(-1024, -720);
	CObjectManager::Get_Instance()->Get_Player()->Set_Pos((1024 * m_iTutorialIndeY) + WINCX / 2, (720 * m_iTutorialIndeX) + WINCY / 2);
	CObjectManager::Get_Instance()->Set_MapIndex(1, 1, DOWN);
	for (auto& row : m_TutorialDungeon) {
		for (auto& dungeon : row) {
			if (dungeon != nullptr) {
				dungeon->Initialize();
			}
		}
	}
	CUiManager::GetInstance()->Set_UiType(UI_END);
	m_TutorialDungeon[m_iTutorialIndeX][m_iTutorialIndeY]->Load_Map();
	Create_MapObj();
}

int CGolemDungeon::Update()
{
	Key_Input();
	list<CObject*> protalList = CObjectManager::Get_Instance()->Get_PortalList();
	for (auto portal : protalList) {
		if (static_cast<CGolemDoor*>(portal)->Get_Load_Next()) {
			switch (static_cast<CGolemDoor*>(portal)->Get_DIR())
			{
			case LEFT:
				m_dir = LEFT;
				m_iTutorialIndeY--;
				m_bMapMove = true;
				CObjectManager::Get_Instance()->Get_Player()->Set_Pos((1024 * m_iTutorialIndeY) + 900, (720 * m_iTutorialIndeX)+WINCY / 2);
				CObjectManager::Get_Instance()->Set_MapIndex(m_iTutorialIndeX, m_iTutorialIndeY, LEFT);
				m_TutorialDungeon[m_iTutorialIndeX][m_iTutorialIndeY]->Load_Map();
				break;
			case RIGHT:
				m_dir = RIGHT;
				m_iTutorialIndeY++;
				m_bMapMove = true;
				CObjectManager::Get_Instance()->Get_Player()->Set_Pos((1024 * m_iTutorialIndeY) + 150, (720 * m_iTutorialIndeX) + WINCY / 2);
				CObjectManager::Get_Instance()->Set_MapIndex(m_iTutorialIndeX, m_iTutorialIndeY, RIGHT);
				m_TutorialDungeon[m_iTutorialIndeX][m_iTutorialIndeY]->Load_Map();

				if (m_iTutorialIndeX == 1 && m_iTutorialIndeY == 3) {
					CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CGolemScroll>::Create((1024 * m_iTutorialIndeY) + WINCX / 2, (720 * m_iTutorialIndeX) + 500));
					static_cast<CGolemScroll*>(CObjectManager::Get_Instance()->Get_LastMapObj())->Set_Text(L"Áß°£ º¸½º");
				}
				break;
			case UP:
				m_dir = UP;
				m_iTutorialIndeX--;
				m_bMapMove = true;
				CObjectManager::Get_Instance()->Get_Player()->Set_Pos((1024 * m_iTutorialIndeY) + WINCX/2, (720 * m_iTutorialIndeX) + 600);
				CObjectManager::Get_Instance()->Set_MapIndex(m_iTutorialIndeX, m_iTutorialIndeY, UP);
				m_TutorialDungeon[m_iTutorialIndeX][m_iTutorialIndeY]->Load_Map();
				break;
			case DOWN:
				m_dir = DOWN;
				m_iTutorialIndeX++;
				m_bMapMove = true;
				CObjectManager::Get_Instance()->Get_Player()->Set_Pos((1024 * m_iTutorialIndeY) + WINCX / 2, (720 * m_iTutorialIndeX) + 150);
				CObjectManager::Get_Instance()->Set_MapIndex(m_iTutorialIndeX, m_iTutorialIndeY, DOWN);
				m_TutorialDungeon[m_iTutorialIndeX][m_iTutorialIndeY]->Load_Map();
				break;
			}
		}
	}

	for (auto& row : m_TutorialDungeon) {
		for (auto& dungeon : row) {
			if (dungeon != nullptr) {
				dungeon->Update();
			}
		}
	}
	CObjectManager::Get_Instance()->Update();
	CUiManager::GetInstance()->Update();
	CUiManager::GetInstance()->Update();
	return 0;
}

void CGolemDungeon::LateUpdate()
{
	Offset();
	for (auto& row : m_TutorialDungeon) {
		for (auto& dungeon : row) {
			if (dungeon != nullptr) {
				dungeon->LateUpdate();
			}
		}
	}
	
	CObjectManager::Get_Instance()->Late_Update();
}

void CGolemDungeon::Render(HDC hDC)
{
	for (auto& row : m_TutorialDungeon) {
		for (auto& dungeon : row) {
			if (dungeon != nullptr) {
				dungeon->Render(hDC);
			}
		}
	}
	CObjectManager::Get_Instance()->Render(hDC);
	CUiManager::GetInstance()->Render(hDC);
}

void CGolemDungeon::Release()
{
	for (auto& row : m_TutorialDungeon) {
		for (auto& dungeon : row) {
			Safe_Delete<CDungeonScene*>(dungeon);
		}
	}
	CObjectManager::Get_Instance()->Delete_ALLNotPlayer();
	CObjectManager::Get_Instance()->RenderListClear();
}

void CGolemDungeon::Key_Input()
{
	if (CKeyManager::Get_Instance()->Key_Down(KEY_MODE, VK_F1)) {
		g_bDevmode = !g_bDevmode;
	}
}

void CGolemDungeon::Create_MapObj()
{
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CGolemScroll>::Create((1024 * m_iTutorialIndeY) + WINCX / 2, (720 * m_iTutorialIndeX) + 500));
	static_cast<CGolemScroll*>(CObjectManager::Get_Instance()->Get_LastMapObj())->Set_Text(L"°ñ·½ ´øÀü");
}

void CGolemDungeon::Offset()
{
	if (m_bMapMove) {
		m_iMove += 16;
		switch (m_dir)
		{
		case LEFT:
		case RIGHT:
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
				}
			}
			break;
		case UP:
		case DOWN:
			if (m_iMove > 720) {
				m_bMapMove = false;
				m_iMove = 0;
			}
			else {
				switch (m_dir)
				{
				case UP:
					CScrollManager::Get_Instance()->Set_ScrollY(16);
					break;
				case DOWN:
					CScrollManager::Get_Instance()->Set_ScrollY(-16);
					break;
				}
			}
			break;
		}
	}
}
