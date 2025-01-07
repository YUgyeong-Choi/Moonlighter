#include "pch.h"
#include "CMainGame.h"
#include "CSceneManager.h"
#include "CBitManager.h"
#include "CObjectManager.h"
#include "CKeyManager.h"
#include "CScrollManager.h"
#include "CFileManager.h"
#include "CUiManager.h"
#include "CSoundManager.h"

bool g_bDevmode = false;
float	g_fEffectVolume(0.6f);
float	g_fBackgroundVolume(0.5f);
float	g_fPlayerVolume(0.7f);
float	g_fMonsterVolume(0.6f);
//float	g_fEffectVolume(0.2f);
//float	g_fBackgroundVolume(0.1f);
//float	g_fPlayerVolume(0.3f);
//float	g_fMonsterVolume(0.2f);

CMainGame::CMainGame():m_hDC(nullptr), m_dwTime(GetTickCount64()), m_iFPS(0)
{
	ZeroMemory(m_szFPS, sizeof(m_szFPS));
}

void CMainGame::Initialize()
{
	srand((unsigned)time(NULL));
	m_hDC = GetDC(g_hWnd);

	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/Back/Back.bmp", L"Back");

	CSceneManager::GetInstance()->SetScene(SC_MENU);
	CSoundManager::Get_Instance()->Initialize();
	CUiManager::GetInstance()->Initialize();
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

	if (m_dwTime + 1000 < GetTickCount64())
	{
		swprintf_s(m_szFPS, L"FPS : %d", m_iFPS);

		SetWindowText(g_hWnd, m_szFPS);

		m_iFPS = 0;
		m_dwTime = GetTickCount64();
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
	CFileManager::DestroyInstance();
	CUiManager::GetInstance()->Release();
	CUiManager::DestroyInstance();
	CSoundManager::Destroy_Instance();
	ReleaseDC(g_hWnd, m_hDC);
}
