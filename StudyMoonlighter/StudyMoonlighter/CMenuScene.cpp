#include "pch.h"
#include "CMenuScene.h"
#include "CBitManager.h"
#include "CKeyManager.h"
#include "CSceneManager.h"
#include "CSoundManager.h"
#include "CAbstractFactory.h"
#include "CObjectManager.h"
#include "CShopPlayer.h"
#include "CUiManager.h"

CMenuScene::CMenuScene():m_bIsOpen(false), m_iOpenSize(0), m_iOpenTime(0), m_iWidth(0)
{
}

void CMenuScene::Initialize()
{
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/StartMenu/GameLogo_1.bmp", L"GameLogo_1");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/StartMenu/GameLogo_2.bmp", L"GameLogo_2");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/StartMenu/GameLogo_3.bmp", L"GameLogo_3");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/StartMenu/Main_menu_1_1.bmp", L"MainMenu_1_1");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/StartMenu/Main_menu_1_2.bmp", L"MainMenu_1_2");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/StartMenu/Main_menu_1_3.bmp", L"MainMenu_1_3");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/StartMenu/Main_menu_1_4.bmp", L"MainMenu_1_4");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/StartMenu/Main_menu_1_5.bmp", L"MainMenu_1_5");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/StartMenu/Main_menu_1_6.bmp", L"MainMenu_1_6");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/StartMenu/Main_menu_1_7.bmp", L"MainMenu_1_7");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/StartMenu/Main_menu_1_8.bmp", L"MainMenu_1_8");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/StartMenu/Main_menu_1_9.bmp", L"MainMenu_1_9");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/StartMenu/Main_menu_1_10.bmp", L"MainMenu_1_10");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/StartMenu/Main_menu_1_11.bmp", L"MainMenu_1_11");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/StartMenu/Main_menu_1_12.bmp", L"MainMenu_1_12");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/StartMenu/Main_menu_1_13.bmp", L"MainMenu_1_13");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/StartMenu/Main_menu_1_14.bmp", L"MainMenu_1_14");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/StartMenu/Main_menu_1_15.bmp", L"MainMenu_1_15");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/StartMenu/Main_menu_1_16.bmp", L"MainMenu_1_16");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/StartMenu/Main_menu_2_1.bmp", L"MainMenu_2_1");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/StartMenu/Main_menu_2_2.bmp", L"MainMenu_2_2");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/StartMenu/Main_menu_2_3.bmp", L"MainMenu_2_3");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/StartMenu/Main_menu_2_4.bmp", L"MainMenu_2_4");    
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/StartMenu/Main_menu_2_5.bmp", L"MainMenu_2_5");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/StartMenu/Main_menu_2_6.bmp", L"MainMenu_2_6");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/StartMenu/Main_menu_2_7.bmp", L"MainMenu_2_7");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/StartMenu/Main_menu_2_8.bmp", L"MainMenu_2_8");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/StartMenu/Main_menu_2_9.bmp", L"MainMenu_2_9");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/StartMenu/Main_menu_2_10.bmp", L"MainMenu_2_10");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/StartMenu/Main_menu_2_11.bmp", L"MainMenu_2_11");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/StartMenu/Main_menu_2_12.bmp", L"MainMenu_2_12");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/StartMenu/Main_menu_2_13.bmp", L"MainMenu_2_13");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/StartMenu/Main_menu_2_14.bmp", L"MainMenu_2_14");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/StartMenu/Main_menu_2_15.bmp", L"MainMenu_2_15");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/StartMenu/Main_menu_2_16.bmp", L"MainMenu_2_16");
}

int CMenuScene::Update()
{
	if (CKeyManager::Get_Instance()->Key_Down(KEY_MODE, VK_SPACE)) {
		CSoundManager::Get_Instance()->PlaySound(L"main_menu_door_open.wav", SOUND_EFFECT, g_fEffectVolume, true);
		m_bIsOpen = true;
	}

	if (m_iOpenSize >= 350) {
		if (CKeyManager::Get_Instance()->Key_Down(KEY_MODE,VK_RETURN))
		{
			CSceneManager::GetInstance()->SetScene(SC_STARTANI);
		}
		m_bIsOpen = false;
		CSoundManager::Get_Instance()->PlaySound(L"main_menu_door_opened_loop.wav", SOUND_EFFECT, g_fEffectVolume, false);
		CSoundManager::Get_Instance()->PlayBGM(L"main_menu_screen.wav", g_fBackgroundVolume, false);
	}

	if (CKeyManager::Get_Instance()->Key_Down(KEY_MODE, VK_F2)) {
		CSceneManager::GetInstance()->SetScene(SC_EDIT);
	}

	if (CKeyManager::Get_Instance()->Key_Down(KEY_MODE, VK_F1)) {
		CObjectManager::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CPlayer>::Create(2040, 646));
		CSceneManager::GetInstance()->SetScene(SC_GOLEMDUNGEON);
		CUiManager::GetInstance()->AddItem(BOW);
		CUiManager::GetInstance()->AddItem(SWORD);
	}

	if (CKeyManager::Get_Instance()->Key_Down(KEY_MODE, VK_F3)) {
		CObjectManager::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CPlayer>::Create(2040, 646));
		CUiManager::GetInstance()->AddItem(RICHJELLY);
		CUiManager::GetInstance()->AddItem(BOW);
		CUiManager::GetInstance()->AddItem(SWORD);
		CSceneManager::GetInstance()->SetScene(SC_SHOP);
	}

	if (CKeyManager::Get_Instance()->Key_Down(KEY_MODE, VK_F4)) {
		CObjectManager::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CPlayer>::Create(WINCX / 2, WINCY / 2));
		static_cast<CPlayer*>(CObjectManager::Get_Instance()->Get_Player())->Set_Pos(1160, 1182);
		CSceneManager::GetInstance()->SetScene(SC_GOLEMBOSS);
		CUiManager::GetInstance()->AddItem(BOW);
		CUiManager::GetInstance()->AddItem(SWORD);
	}

	if (CKeyManager::Get_Instance()->Key_Down(KEY_MODE, VK_F5)) {
		CObjectManager::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CPlayer>::Create(WINCX / 2, WINCY / 2));
		CSceneManager::GetInstance()->SetScene(SC_FIELD);
	}

	if (CKeyManager::Get_Instance()->Key_Down(KEY_MODE, VK_F6)) {
		CSceneManager::GetInstance()->SetScene(SC_FINALANI);
	}

	if (m_bIsOpen) {
		m_iOpenSize += 2;
	}

	m_iAnimationTime++;
	if (m_iAnimationTime >= 81) {
		m_iAnimationTime = 0;
	}
	return 0;
}

void CMenuScene::LateUpdate()
{
}

void CMenuScene::Render(HDC hDC)
{
	//HDC hMemDC2 = CBitManager::GetInstance()->FindImage(L"GameLogo_4");
	//GdiTransparentBlt(hDC, 0, 0, 400, 600, hMemDC2, 0, 0, 2, 149, RGB(0, 0, 0));

	TCHAR szImageName[256];
	_stprintf_s(szImageName, 256, _T("MainMenu_2_%d"), m_iAnimationTime /5);

	HDC hMemDC = CBitManager::GetInstance()->FindImage(szImageName);
	GdiTransparentBlt(hDC, 0 , 0, WINCX, WINCY, hMemDC, 0, 0, 640, 360, RGB(0, 0, 0));

	TCHAR szImageName2[256];
	_stprintf_s(szImageName2, 256, _T("MainMenu_1_%d"), m_iAnimationTime /5);

	hMemDC = CBitManager::GetInstance()->FindImage(szImageName2);
	GdiTransparentBlt(hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, 640, 360, RGB(255, 255, 255));

	HFONT hFont1 = CreateFont(
		60, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"m3x6"
	);

	HFONT OldFont = (HFONT)SelectObject(hDC, hFont1);

	TCHAR szMainGame[64];
	_stprintf_s(szMainGame, _T("@ Main Game @"));
	TextOut(hDC, WINCX / 2 - 150, 400, szMainGame, _tcslen(szMainGame));

	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkMode(hDC, TRANSPARENT);

	SelectObject(hDC, OldFont);
	DeleteObject(hFont1);

	hMemDC = CBitManager::GetInstance()->FindImage(L"GameLogo_2"); 
	GdiTransparentBlt(hDC, 0 - m_iOpenSize, 0, WINCX/2, WINCY, hMemDC, 0, 0, 320, 362, RGB(0, 0, 0));

	hMemDC = CBitManager::GetInstance()->FindImage(L"GameLogo_3");
	GdiTransparentBlt(hDC, WINCX / 2 + m_iOpenSize, 0, WINCX / 2, WINCY, hMemDC, 0, 0, 320, 362, RGB(255, 255, 255));

	hMemDC = CBitManager::GetInstance()->FindImage(L"GameLogo_1");
	GdiTransparentBlt(hDC, WINCX / 2 - 150, 80, 300, 250, hMemDC, 0, 0, 172, 140, RGB(255, 255, 255));
}

void CMenuScene::Release()
{
}

void CMenuScene::Key_Input()
{
}

void CMenuScene::Create_MapObj()
{
}

void CMenuScene::Offset()
{
}
