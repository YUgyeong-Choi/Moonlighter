#include "pch.h"
#include "CDungeonEditScene.h"
#include "CBitManager.h"
#include "CScrollManager.h"
#include "CTileManager.h"
#include "CKeyManager.h"
#include "CScrollManager.h"

CDungeonEditScene::CDungeonEditScene():m_bIsShowTile(false)
{
}

void CDungeonEditScene::Initialize()
{
	ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/background.bmp", L"DungeonBackground");
	ADD_BMP(L"../MoonlighterAssets/Tile/CanWalk32.bmp", L"CanWalk");
	ADD_BMP(L"../MoonlighterAssets/Tile/CantWalk32.bmp", L"CantWalk");
	m_fMapXSize = 1024.f;
	m_fMapYSize = 720.f;
	CTileManager::Get_Instance()->Initialize();
}

int CDungeonEditScene::Update()
{
	CTileManager::Get_Instance()->Update();
    return 0;
}

void CDungeonEditScene::LateUpdate()
{
	Key_Input();
	CTileManager::Get_Instance()->Late_Update();
}

void CDungeonEditScene::Render(HDC hDC)
{
	Rectangle(hDC, -100, -100, 1100, 1100);
	HDC hMemDC = CBitManager::GetInstance()->FindImage(L"DungeonBackground");
	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();
	GdiTransparentBlt(hDC, iScrollX, iScrollY, m_fMapXSize, m_fMapYSize, hMemDC, 0, 0, m_fMapXSize, m_fMapYSize, RGB(0, 0, 0));
	CTileManager::Get_Instance()->Render(hDC);
}

void CDungeonEditScene::Release()
{
}

void CDungeonEditScene::Key_Input()
{
	if (CKeyManager::Get_Instance()->Key_Pressing(VK_LEFT))
	{
		CScrollManager::Get_Instance()->Set_ScrollX(5.f);
	}

	if (CKeyManager::Get_Instance()->Key_Pressing(VK_RIGHT))
	{
		CScrollManager::Get_Instance()->Set_ScrollX(-5.f);
	}

	if (CKeyManager::Get_Instance()->Key_Down(VK_F1)) {
		m_bIsShowTile = !m_bIsShowTile;
	}

	if (m_bIsShowTile) {
		if (CKeyManager::Get_Instance()->Key_Pressing(VK_LBUTTON))
		{
			POINT	ptMouse{};
			GetCursorPos(&ptMouse);
			ScreenToClient(g_hWnd, &ptMouse);

			ptMouse.x -= (int)CScrollManager::Get_Instance()->Get_ScrollX();
			ptMouse.y -= (int)CScrollManager::Get_Instance()->Get_ScrollY();

			CTileManager::Get_Instance()->Picking_Tile(ptMouse);
		}
	}


	if (CKeyManager::Get_Instance()->Key_Down('S'))
	{
		CTileManager::Get_Instance()->Save_Tile();
		return;
	}

	if (CKeyManager::Get_Instance()->Key_Down('L'))
	{
		CTileManager::Get_Instance()->Load_Tile();
		return;
	}
}

void CDungeonEditScene::Create_MapObj()
{
}
