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

CTutorialScene::CTutorialScene() :m_iTutorialIndex(0)
{
}

void CTutorialScene::Initialize()
{
	CObjectManager::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CPlayer>::Create(WINCX / 2, WINCY / 2));
	m_TutorialDungeon[m_iTutorialIndex] = new CDungeonScene;
	m_TutorialDungeon[m_iTutorialIndex]->Initialize();
}

int CTutorialScene::Update()
{
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
	m_TutorialDungeon[m_iTutorialIndex]->Render(hDC);
}

void CTutorialScene::Release()
{
	CObjectManager::Get_Instance()->Delete_ID(OBJ_MAPOBJ);
	CObjectManager::Get_Instance()->Delete_ID(OBJ_PORTAL);
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
}



