#include "pch.h"
#include "CAnimationScene.h"
#include "CScrollManager.h"
#include "CBitManager.h"
#include "CSoundManager.h"
#include "CKeyManager.h"
#include "CAbstractFactory.h"
#include "CUiManager.h"

#include "CPlayer.h"
#include "CObjectManager.h"

CAnimationScene::CAnimationScene():m_bPortalFinish(false), m_pGrandpa(nullptr)
{
	ZeroMemory(&m_tPortalFrame, sizeof(FRAME));
}

void CAnimationScene::Initialize()
{
	CUiManager::GetInstance()->AddItem(BOW);
	CSoundManager::Get_Instance()->StopAll();
	CSoundManager::Get_Instance()->PlayBGM(L"intro.wav", g_fBackgroundVolume, true);
	ADD_BMP(L"../MoonlighterAssets/Map/DungeonsEntrance_Base.bmp", L"DungeonField");
	m_fMapXSize = 2400.f;
	m_fMapYSize = 2048.f;
	CScrollManager::Get_Instance()->Set_Scroll(-353, -676);
	CScrollManager::Get_Instance()->Set_ScrollLock(m_fMapXSize, m_fMapYSize);

	m_tPortalFrame.iFrameStart = 0;
	m_tPortalFrame.iFrameEnd = 34;
	m_tPortalFrame.dwSpeed = 150;
	m_tPortalFrame.dwTime = GetTickCount64();

	Create_MapObj();
	m_pGrandpa = CAbstractFactory<CVisitorMale>::Create(1200, 1200);
	CObjectManager::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CPlayer>::Create(2040, 646));
}

int CAnimationScene::Update()
{
	if (m_tPortalFrame.iFrameStart == m_tPortalFrame.iFrameEnd) {
		m_bPortalFinish = true;
	}
	m_pGrandpa->Update();
    return 0;
}

void CAnimationScene::LateUpdate()
{
	if (!m_bPortalFinish) {
		if (m_tPortalFrame.dwTime + m_tPortalFrame.dwSpeed < GetTickCount64())
		{
			++m_tPortalFrame.iFrameStart;

			m_tPortalFrame.dwTime = GetTickCount64();
		}
	}
	m_pGrandpa->Late_Update();

}

void CAnimationScene::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

	HDC hMemDC = CBitManager::GetInstance()->FindImage(L"DungeonField");
	GdiTransparentBlt(hDC, iScrollX, iScrollY, (int)m_fMapXSize, m_fMapYSize, hMemDC, 0, 0, (int)m_fMapXSize, m_fMapYSize, RGB(0, 0, 0));

	Image* image = Image::FromFile(L"../MoonlighterAssets/Map/Animation/will_dungeon_out.png");
	Graphics graphics(hDC);
	graphics.DrawImage(image, 297 ,205, 88 * m_tPortalFrame.iFrameStart, 0, (int)88, (int)140, UnitPixel);
	m_pGrandpa->Render(hDC);
}

void CAnimationScene::Release()
{

	Safe_Delete<CObject*>(m_pGrandpa);
}

void CAnimationScene::Key_Input()
{
}

void CAnimationScene::Create_MapObj()
{
}

void CAnimationScene::Offset()
{
}
