#include "pch.h"
#include "CMainGame.h"
#include "CSceneManager.h"
#include "CBitManager.h"

CMainGame::CMainGame()
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
}

void CMainGame::Render()
{
	HDC		hMemDC = CBitManager::GetInstance()->FindImage(L"Back");
	CSceneManager::GetInstance()->Render(hMemDC);
	BitBlt(m_hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);
}

void CMainGame::Release()
{
}
