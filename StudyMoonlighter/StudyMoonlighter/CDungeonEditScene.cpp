#include "pch.h"
#include "CDungeonEditScene.h"
#include "CBitManager.h"
#include "CObjectManager.h"
#include "CKeyManager.h"
#include "CAbstractFactory.h"
#include "CFileManager.h"
#include "CUiManager.h"

//여기 밑에는 지워도 되는 헤더 파일
#include "CCollisionBox.h"
#include "CScrollManager.h"
#include "CGolemBreakable.h"
#include "CPlayer.h"
#include "CScrollWasd.h"
#include "CGolemDoor.h"
#include "CScrollRoll.h"
#include "CGolemHole.h"
#include "CTurretBroken.h"
#include "CBabySlime.h"
#include "CGolemSlimePuddle.h"
#include "CSlimeHermit.h"
#include "CGolemSoldier.h"
#include "CGolemHead.h"
#include "CGolemMiniboss.h"
#include "CSoundManager.h"
#include "CGolemScroll.h"
#include "CGolemBreakable2.h"

CDungeonEditScene::CDungeonEditScene()
{
}

void CDungeonEditScene::Initialize()
{
	ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/background.bmp", L"DungeonBackground");

	ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/slime_puddle1.bmp", L"DungeonBackgroundSlime");
	CSoundManager::Get_Instance()->PlaySound(L"golem_dungeon_floor.wav", SOUND_BGM, g_fBackgroundVolume, true);
	m_fMapXSize = 1024.f;
	m_fMapYSize = 720.f;
	CObjectManager::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CPlayer>::Create(150, WINCY / 2));
	CUiManager::GetInstance()->AddItem(SWORD);
	CUiManager::GetInstance()->Set_UiType(UI_DUNGEON);
	Create_MapObj();
}

int CDungeonEditScene::Update()
{
	Key_Input();
	CObjectManager::Get_Instance()->Update();
	CUiManager::GetInstance()->Update();
    return 0;
}

void CDungeonEditScene::LateUpdate()
{
	CObjectManager::Get_Instance()->Late_Update();
}

void CDungeonEditScene::Render(HDC hDC)
{
	//Rectangle(hDC, -100, -100, 1100, 1100);
	HDC hMemDC = CBitManager::GetInstance()->FindImage(L"DungeonBackground");
	GdiTransparentBlt(hDC, 0, 0, (int)m_fMapXSize, (int)m_fMapYSize, hMemDC, 0, 0, (int)m_fMapXSize, (int)m_fMapYSize, RGB(0, 0, 0));
	CObjectManager::Get_Instance()->Render(hDC);
	CUiManager::GetInstance()->Render(hDC);
}

void CDungeonEditScene::Release()
{
}

void CDungeonEditScene::Key_Input()
{

	if (CKeyManager::Get_Instance()->Key_Down(KEY_MODE, VK_F1)) {
		g_bDevmode = !g_bDevmode;
	}

	if (CKeyManager::Get_Instance()->Key_Down(KEY_MODE, 'O'))
	{
		CFileManager::GetInstance()->Save_MapObj(L"../Data/MapMonster/GolemDungeon1.dat");
		CFileManager::GetInstance()->Save_MonsterObj(L"../Data/MapObj/GolemDungeon1.dat");
	}

	if (CKeyManager::Get_Instance()->Key_Down(KEY_MODE, 'P'))
	{
		CFileManager::GetInstance()->Load_MapObjFile(L"../Data/MapMonster/GolemDungeon1.dat", 0,0);
		CFileManager::GetInstance()->Load_MonsterFile(L"../Data/MapObj/GolemDungeon1.dat",0,0);
	}

}

void CDungeonEditScene::Create_MapObj()
{
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CCollisionBox>::Create(WINCX / 2, 30, 1024, 60));
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CCollisionBox>::Create(WINCX / 2, 670, 1024, 60));
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CCollisionBox>::Create(30, WINCY / 2, 60, 720));
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CCollisionBox>::Create(994, WINCY / 2, 60, 720));

	CObjectManager::Get_Instance()->Add_Object(OBJ_PORTAL, CAbstractFactory<CGolemDoor>::Create(980, WINCY / 2));
	dynamic_cast<CGolemDoor*>(CObjectManager::Get_Instance()->Get_LastPortal())->Set_DIR(RIGHT);

	CObjectManager::Get_Instance()->Add_Object(OBJ_SLIME, CAbstractFactory<CGolemSlimePuddle>::Create(250, 200));
	CObjectManager::Get_Instance()->Add_Object(OBJ_SLIME, CAbstractFactory<CGolemSlimePuddle>::Create(700, 500));

	CObjectManager::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CBabySlime>::Create(150, 150));
	CObjectManager::Get_Instance()->Get_LastMonster()->Set_TargetObj(CObjectManager::Get_Instance()->Get_Player());

	CObjectManager::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CBabySlime>::Create(300, 150));
	CObjectManager::Get_Instance()->Get_LastMonster()->Set_TargetObj(CObjectManager::Get_Instance()->Get_Player());

	CObjectManager::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CBabySlime>::Create(400, 150));
	CObjectManager::Get_Instance()->Get_LastMonster()->Set_TargetObj(CObjectManager::Get_Instance()->Get_Player());

	CObjectManager::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CBabySlime>::Create(500, 150));
	CObjectManager::Get_Instance()->Get_LastMonster()->Set_TargetObj(CObjectManager::Get_Instance()->Get_Player());

	CObjectManager::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CBabySlime>::Create(150, 500));
	CObjectManager::Get_Instance()->Get_LastMonster()->Set_TargetObj(CObjectManager::Get_Instance()->Get_Player());

	CObjectManager::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CBabySlime>::Create(300, 500));
	CObjectManager::Get_Instance()->Get_LastMonster()->Set_TargetObj(CObjectManager::Get_Instance()->Get_Player());

	CObjectManager::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CBabySlime>::Create(400, 500));
	CObjectManager::Get_Instance()->Get_LastMonster()->Set_TargetObj(CObjectManager::Get_Instance()->Get_Player());

	CObjectManager::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CBabySlime>::Create(500, 500));
	CObjectManager::Get_Instance()->Get_LastMonster()->Set_TargetObj(CObjectManager::Get_Instance()->Get_Player());
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CGolemBreakable2>::Create(WINCX / 2 + 300 , WINCY/2));
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CGolemBreakable2>::Create(WINCX / 2 + 300 , WINCY/2-100));
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CGolemBreakable2>::Create(WINCX / 2 + 300 , WINCY/2+100));
}

void CDungeonEditScene::Offset()
{
}
