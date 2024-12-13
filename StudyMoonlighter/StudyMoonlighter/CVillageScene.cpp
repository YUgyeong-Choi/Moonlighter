#include "pch.h"
#include "CVillageScene.h"
#include "CBitManager.h"
#include "CAbstractFactory.h"
#include "CObjectManager.h"
#include "CPlayer.h"
#include "CScrollManager.h"
CVillageScene::CVillageScene()
{
}

void CVillageScene::Initialize()
{
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/Map/MainVillage.bmp", L"VillageBackground");
	CObjectManager::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CPlayer>::Create());
}

int CVillageScene::Update()
{

	CObjectManager::Get_Instance()->Update();
	return 0;
}

void CVillageScene::LateUpdate()
{
	CObjectManager::Get_Instance()->Late_Update();
}

void CVillageScene::Render(HDC hDC)
{
	HDC hMemDC = CBitManager::GetInstance()->FindImage(L"VillageBackground");
	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();
	GdiTransparentBlt(hDC, iScrollX, iScrollY, 1301, 1067, hMemDC, 0, 0, 1301, 1067, RGB(0, 0, 0));
	CObjectManager::Get_Instance()->Render(hDC);
}

void CVillageScene::Release()
{
}
