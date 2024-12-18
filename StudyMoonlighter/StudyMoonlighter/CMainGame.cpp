#include "pch.h"
#include "CMainGame.h"
#include "CSceneManager.h"
#include "CBitManager.h"
#include "CObjectManager.h"
#include "CKeyManager.h"
#include "CScrollManager.h"

bool g_bDevmode = false;

CMainGame::CMainGame():m_hDC(nullptr), m_dwTime(GetTickCount()), m_iFPS(0)
{
	ZeroMemory(m_szFPS, sizeof(m_szFPS));
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
}

void CMainGame::Render()
{
#pragma region  FPS Ãâ·Â
	++m_iFPS;

	if (m_dwTime + 1000 < GetTickCount())
	{
		swprintf_s(m_szFPS, L"FPS : %d", m_iFPS);

		SetWindowText(g_hWnd, m_szFPS);

		m_iFPS = 0;
		m_dwTime = GetTickCount();
	}
#pragma endregion
	HDC		hMemDC = CBitManager::GetInstance()->FindImage(L"Back");
	CSceneManager::GetInstance()->Render(hMemDC);
	GdiTransparentBlt(m_hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, 1024, 720, SRCCOPY);
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
