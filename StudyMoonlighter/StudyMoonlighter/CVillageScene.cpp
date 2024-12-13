#include "pch.h"
#include "CVillageScene.h"
#include "CBitManager.h"
#include "CAbstractFactory.h"
#include "CObjectManager.h"
#include "CPlayer.h"
#include "CScrollManager.h"
#include "CKeyManager.h"
#include "CTileManager.h"
CVillageScene::CVillageScene()
{
}

void CVillageScene::Initialize()
{
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/Map/MainVillage.bmp", L"VillageBackground");
	CObjectManager::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CPlayer>::Create());
	m_fMapXSize = 2602.f;
	m_fMapYSize = 2134.f;
	CScrollManager::Get_Instance()->Set_ScrollLock(m_fMapXSize, m_fMapYSize);

	//타일 전용
	CTileManager::Get_Instance()->Initialize();	
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/Tile/CanWalk32.bmp", L"CanWalk");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/Tile/CantWalk32.bmp", L"CantWalk");
}

int CVillageScene::Update()
{
	KeyInput();
	CObjectManager::Get_Instance()->Update();
	CTileManager::Get_Instance()->Update();
	return 0;
}

void CVillageScene::LateUpdate()
{
	CObjectManager::Get_Instance()->Late_Update();
	CScrollManager::Get_Instance()->Scroll_Lock();
	CTileManager::Get_Instance()->Late_Update();
}

void CVillageScene::Render(HDC hDC)
{
	HDC hMemDC = CBitManager::GetInstance()->FindImage(L"VillageBackground");
	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();
	GdiTransparentBlt(hDC, iScrollX, iScrollY, 2602, 2134, hMemDC, 0, 0, 2602, 2134, RGB(0, 0, 0));
	CObjectManager::Get_Instance()->Render(hDC);

	if (g_bDevmode) {
		CTileManager::Get_Instance()->Render(hDC);
	}
}

void CVillageScene::Release()
{
}

//타일 전용
void CVillageScene::KeyInput()
{
	if (CKeyManager::Get_Instance()->Key_Down(VK_F1))
	{
		g_bDevmode = !g_bDevmode;
	}

	if (CKeyManager::Get_Instance()->Key_Down(VK_F2))
	{
		//Load
	}

	if (CKeyManager::Get_Instance()->Key_Down(VK_F3))
	{
		//Save
	}
}
