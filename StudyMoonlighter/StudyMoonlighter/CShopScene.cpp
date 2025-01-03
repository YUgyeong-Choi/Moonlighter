#include "pch.h"
#include "CShopScene.h"
#include "CSoundManager.h"
#include "CObjectManager.h"
#include "CAbstractFactory.h"
#include "CPlayer.h"
#include "CScrollManager.h"
#include "CKeyManager.h"

CShopScene::CShopScene():m_bShop(true)
{
}

void CShopScene::Initialize()
{
	CSoundManager::Get_Instance()->StopAll();
	CSoundManager::Get_Instance()->PlayBGM(L"shop_day_open.wav", g_fBackgroundVolume, true);
	CObjectManager::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CPlayer>::Create(564, 704));
	CScrollManager::Get_Instance()->Set_Scroll(0, -260);
	m_fMapXSize = WINCX;
	m_fMapYSize = 1440;
	CScrollManager::Get_Instance()->Set_ScrollLock(m_fMapXSize, m_fMapYSize);
}

int CShopScene::Update()
{
	Key_Input();
	Offset();
	CObjectManager::Get_Instance()->Update();
	return 0;
}

void CShopScene::LateUpdate()
{
	CObjectManager::Get_Instance()->Late_Update();
}

void CShopScene::Render(HDC hDC)
{
	COLORREF color = RGB(0, 0, 0);
	HBRUSH hBrush = CreateSolidBrush(color);
	HPEN hPen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
	HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
	Rectangle(hDC, 0, 0, m_fMapXSize, m_fMapYSize);

	SelectObject(hDC, hOldBrush);
	SelectObject(hDC, hOldPen);
	DeleteObject(hBrush);
	DeleteObject(hPen);
	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();
	Image* image = Image::FromFile(L"../MoonlighterAssets/Map/Shop/shop_background.png");
	Graphics graphics(hDC);
	graphics.DrawImage(image, WINCX/2-339 + iScrollX, iScrollY, 0, 0, 678, 868, UnitPixel);

	delete image;
	CObjectManager::Get_Instance()->Render(hDC);
}

void CShopScene::Release()
{
}

void CShopScene::Key_Input()
{
	if (CKeyManager::Get_Instance()->Key_Down('1')) {
		m_bShop = !m_bShop;
	}
}

void CShopScene::Create_MapObj()
{
}

void CShopScene::Offset()
{
	if (m_bShop) {
		if (CScrollManager::Get_Instance()->Get_ScrollY() != -260) {
			CScrollManager::Get_Instance()->Set_ScrollY(-5);
		}
	}
	else {
		if (CScrollManager::Get_Instance()->Get_ScrollY() != 100) {
			CScrollManager::Get_Instance()->Set_ScrollY(5);
		}
	}
}
