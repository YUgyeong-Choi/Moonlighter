#include "pch.h"
#include "CMenuScene.h"
#include "CBitManager.h"

CMenuScene::CMenuScene()
{
}

void CMenuScene::Initialize()
{
	/*CBitManager::GetInstance()->InsertBmpList(L"../MoonlighterAssets/GameLogo_", 4);
	CBitManager::GetInstance()->InsertBmpList(L"../MoonlighterAssets/Main_menu_1_", 16);
	CBitManager::GetInstance()->InsertBmpList(L"../MoonlighterAssets/Main_menu_2_", 16);*/
}

int CMenuScene::Update()
{
	m_iAnimationTime++;
	return 0;
}

void CMenuScene::LateUpdate()
{
}

void CMenuScene::Render(HDC hDC)
{
}

void CMenuScene::Release()
{
}
