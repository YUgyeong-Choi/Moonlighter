#include "pch.h"
#include "CVillageScene.h"
#include "CBitManager.h"
CVillageScene::CVillageScene()
{
}

void CVillageScene::Initialize()
{
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/Map/Village_Background_Christmas.bmp", L"VillageBackground");
}

int CVillageScene::Update()
{
	return 0;
}

void CVillageScene::LateUpdate()
{
}

void CVillageScene::Render(HDC hDC)
{
	HDC hMemDC = CBitManager::GetInstance()->FindImage(L"VillageBackground");
	GdiTransparentBlt(hDC, 0, 0, 1305, 1071, hMemDC, 0, 0, 1305, 1071, RGB(0, 0, 0));
}

void CVillageScene::Release()
{
}
