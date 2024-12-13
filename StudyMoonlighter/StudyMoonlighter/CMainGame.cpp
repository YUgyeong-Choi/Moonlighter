#include "pch.h"
#include "CMainGame.h"
#include "CSceneManager.h"
#include "CBitManager.h"
#include "CObjectManager.h"
#include "CKeyManager.h"
#include "CScrollManager.h"

CMainGame::CMainGame():m_hDC(nullptr)
{
}

void CMainGame::Initialize()
{
	m_hDC = GetDC(g_hWnd);

	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/Back/Back.bmp", L"Back");

	CSceneManager::GetInstance()->SetScene(SC_MENU);
}

void CMainGame::Update()
{
	CSceneManager::GetInstance()->Update();
}

void CMainGame::LateUpdate()
{
	CSceneManager::GetInstance()->LateUpdate();
	CKeyManager::Get_Instance()->Update();
	CScrollManager::Get_Instance()->Scroll_Lock();
}

void CMainGame::Render()
{
	HDC		hMemDC = CBitManager::GetInstance()->FindImage(L"Back");
	CSceneManager::GetInstance()->Render(hMemDC);
	GdiTransparentBlt(m_hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, 1280, 720, SRCCOPY);
}

void CMainGame::Release()
{
	CSceneManager::DestroyInstance();
	CBitManager::DestroyInstance();
	CObjectManager::DestroyInstance();
	CKeyManager::Destroy_Instance();
	CScrollManager::Destroy_Instance();
	ReleaseDC(g_hWnd, m_hDC);
}
