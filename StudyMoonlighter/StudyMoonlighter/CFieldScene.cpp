#include "pch.h"
#include "CFieldScene.h"
#include "CKeyManager.h"
#include "CObjectManager.h"
#include "CPlayer.h"
#include "CScrollManager.h"
#include "CBitManager.h"
#include "CAbstractFactory.h"
#include "CPortal.h"
#include "CUiManager.h"

CFieldScene::CFieldScene()
{
}

void CFieldScene::Initialize()
{
	ADD_BMP(L"../MoonlighterAssets/Map/DungeonsEntrance_Base.bmp", L"DungeonField");
	static_cast<CPlayer*>(CObjectManager::Get_Instance()->Get_Player())->Set_Pos(1225, 1851);
	m_fMapXSize = 2400.f;
	m_fMapYSize = 2048.f;
	CScrollManager::Get_Instance()->Set_Scroll(-613, -1327);
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
	Offset();
	CObjectManager::Get_Instance()->Late_Update();
	CScrollManager::Get_Instance()->Scroll_Lock();
}

void CFieldScene::Render(HDC hDC)
{
	HDC hMemDC = CBitManager::GetInstance()->FindImage(L"DungeonField");
	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();
	GdiTransparentBlt(hDC, iScrollX, iScrollY, (int)m_fMapXSize, m_fMapYSize, hMemDC, 0, 0, (int)m_fMapXSize, m_fMapYSize, RGB(0, 0, 0));
	CObjectManager::Get_Instance()->Render(hDC);
	CUiManager::GetInstance()->Render(hDC);
}

void CFieldScene::Release()
{
	CObjectManager::Get_Instance()->Delete_ID(OBJ_PORTAL);
	CObjectManager::Get_Instance()->RenderListClear();
}

void CFieldScene::Key_Input()
{
	if (CKeyManager::Get_Instance()->Key_Down(KEY_MODE,VK_F1)) {
		g_bDevmode = !g_bDevmode;
	}
}

void CFieldScene::Create_MapObj()
{
	CObjectManager::Get_Instance()->Add_Object(OBJ_PORTAL, CAbstractFactory<CPortal>::Create(700, 930, 100, 100));
	static_cast<CPortal*>(CObjectManager::Get_Instance()->Get_LastPortal())->Set_PortalType(DUNGEON);
}

void CFieldScene::Offset()
{
	CObject* _copyPlayer = CObjectManager::Get_Instance()->Get_Player();
	int		iOffSetminX = 412;
	int		iOffSetmaxX = 612;

	int iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	if (iOffSetminX > _copyPlayer->Get_Info().fX + iScrollX)
		CScrollManager::Get_Instance()->Set_ScrollX(_copyPlayer->Get_Speed());

	if (iOffSetmaxX < _copyPlayer->Get_Info().fX + iScrollX)
		CScrollManager::Get_Instance()->Set_ScrollX(-_copyPlayer->Get_Speed());

	int		iOffSetminY = 260;
	int		iOffSetmaxY = 460;

	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

	if (iOffSetminY > _copyPlayer->Get_Info().fY + iScrollY)
		CScrollManager::Get_Instance()->Set_ScrollY(_copyPlayer->Get_Speed());

	if (iOffSetmaxY < _copyPlayer->Get_Info().fY + iScrollY)
		CScrollManager::Get_Instance()->Set_ScrollY(-_copyPlayer->Get_Speed());
}
