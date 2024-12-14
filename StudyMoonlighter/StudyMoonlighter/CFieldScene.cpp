#include "pch.h"
#include "CFieldScene.h"
#include "CKeyManager.h"
#include "CObjectManager.h"
#include "CPlayer.h"
#include "CScrollManager.h"
#include "CBitManager.h"

CFieldScene::CFieldScene()
{
}

void CFieldScene::Initialize()
{
	ADD_BMP(L"../MoonlighterAssets/Map/DungeonsEntrance_Base.bmp", L"DungeonField");
	static_cast<CPlayer*>(CObjectManager::Get_Instance()->Get_Player())->Set_Pos(1200, 1800);
	m_fMapXSize = 2400.f;
	m_fMapYSize = 2048.f;
	CScrollManager::Get_Instance()->Set_ScrollLock(m_fMapXSize, m_fMapYSize);
	
	Create_MapObj();
}

int CFieldScene::Update()
{
	Key_Input();
	CObjectManager::Get_Instance()->Update();
	return 0;
}

void CFieldScene::LateUpdate()
{
	CObjectManager::Get_Instance()->Late_Update();
	CScrollManager::Get_Instance()->Scroll_Lock();
}

void CFieldScene::Render(HDC hDC)
{
	HDC hMemDC = CBitManager::GetInstance()->FindImage(L"DungeonField");
	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();
	GdiTransparentBlt(hDC, iScrollX, iScrollY, m_fMapXSize, m_fMapYSize, hMemDC, 0, 0, m_fMapXSize, m_fMapYSize, RGB(0, 0, 0));
	CObjectManager::Get_Instance()->Render(hDC);
}

void CFieldScene::Release()
{
	CObjectManager::Get_Instance()->Delete_ID(OBJ_MAPOBJ);
}

void CFieldScene::Key_Input()
{
	if (CKeyManager::Get_Instance()->Key_Down(VK_F1)) {
		g_bDevmode = !g_bDevmode;
	}
}

void CFieldScene::Create_MapObj()
{

}
