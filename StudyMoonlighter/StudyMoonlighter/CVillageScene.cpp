#include "pch.h"
#include "CVillageScene.h"
#include "CBitManager.h"
#include "CAbstractFactory.h"
#include "CObjectManager.h"
#include "CPlayer.h"
#include "CScrollManager.h"
#include "CKeyManager.h"
#include "CTree1.h"
CVillageScene::CVillageScene()
{
}

void CVillageScene::Initialize()
{
	ADD_BMP(L"../MoonlighterAssets/Map/MainVillage.bmp", L"VillageBackground");
	CObjectManager::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CPlayer>::Create(WINCX / 2, WINCY / 2));
	m_fMapXSize = 2602.f;
	m_fMapYSize = 2134.f;
	CScrollManager::Get_Instance()->Set_ScrollLock(m_fMapXSize, m_fMapYSize);

	Create_MapObj();
}

int CVillageScene::Update()
{
	Key_Input();
	CObjectManager::Get_Instance()->Update();
	return 0;
}

void CVillageScene::LateUpdate()
{
	CObjectManager::Get_Instance()->Late_Update();
	CScrollManager::Get_Instance()->Scroll_Lock();
}

void CVillageScene::Render(HDC hDC)
{
	HDC hMemDC = CBitManager::GetInstance()->FindImage(L"VillageBackground");
	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();
	GdiTransparentBlt(hDC, iScrollX, iScrollY, 2602, 2134, hMemDC, 0, 0, 2602, 2134, RGB(0, 0, 0));
	CObjectManager::Get_Instance()->Render(hDC);
}

void CVillageScene::Release()
{
}

void CVillageScene::Key_Input()
{
	if (CKeyManager::Get_Instance()->Key_Down(VK_F1)) {
		g_bDevmode = !g_bDevmode;
	}
}

//void CVillageScene::Load_Assets()
//{
//	
//	
//	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/Map/Object/Tree2/Tree2.bmp", L"Tree2");
//}

void CVillageScene::Create_MapObj()
{
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CTree1>::Create(WINCX/2, WINCY/2));
}

