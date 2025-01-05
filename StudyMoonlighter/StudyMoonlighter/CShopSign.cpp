#include "pch.h"
#include "CShopSign.h"
#include "CSceneManager.h"
#include "CShopScene.h"
#include "CScrollManager.h"
#include "CObjectManager.h"
#include "CKeyManager.h"

CShopSign::CShopSign():m_bActive(false)
{
}

void CShopSign::Initialize()
{
	m_eOBJID = OBJ_MAPOBJ;
	m_eRender = RENDER_GAMEOBJECT;
	m_tInfo.fCX = 50;
	m_tInfo.fCY = 40;
	m_tRenderSizeX = 62;
	m_tRenderSizeY = 46;
	m_HitBox = { (int)m_tInfo.fX - 50,(int)m_tInfo.fY + 10,(int)m_tInfo.fX + 50,(int)m_tInfo.fY + 50 };
}

int CShopSign::Update()
{
	Key_Input();
	__super::Update_Rect();
	return 0;
}

void CShopSign::Late_Update()
{
	m_bActive = false;
	OnCollision();
}

void CShopSign::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

	Graphics graphics(hDC);
	Image* image(nullptr);
	if (dynamic_cast<CShopScene*>(CSceneManager::GetInstance()->Get_Scene())->Get_ShopOpen()) {
		image = Image::FromFile(L"../MoonlighterAssets/Map/Shop/sign_open.png");
		graphics.DrawImage(image, m_tRenderRect.left + iScrollX, m_tRenderRect.top + iScrollY, 0, 0,(int)m_tRenderSizeX, (int)m_tRenderSizeY, UnitPixel);
	}
	else {
		image = Image::FromFile(L"../MoonlighterAssets/Map/Shop/sign_close.png");
		graphics.DrawImage(image, m_tRenderRect.left + iScrollX, m_tRenderRect.top + iScrollY, 0, 0, (int)m_tRenderSizeX, (int)m_tRenderSizeY, UnitPixel);
	}

	if (m_bActive) {
		COLORREF color = RGB(234, 221, 187);

		HBRUSH hBrush = CreateSolidBrush(color);
		HPEN hPen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
		RoundRect(hDC, m_tInfo.fX + iScrollX + 20, m_tInfo.fY - 70 + iScrollY, m_tInfo.fX + 160 + iScrollX, m_tInfo.fY - 20 + iScrollY, 30, 30);

		SelectObject(hDC, hOldBrush);
		SelectObject(hDC, hOldPen);
		DeleteObject(hBrush);
		DeleteObject(hPen);

		Image* image(nullptr);
		Graphics graphics(hDC);
		image = Image::FromFile(L"../MoonlighterAssets/Ui/button_J.png");
		graphics.DrawImage(image, (int)m_tInfo.fX + iScrollX + 15, (int)m_tInfo.fY - 75 + iScrollY, 0, 0, 64, 64, UnitPixel);

		SetTextColor(hDC, RGB(0, 0, 0));
		TCHAR szEnter[64];
		_stprintf_s(szEnter, _T("상점 관리"));
		TextOut(hDC, (int)m_tInfo.fX + iScrollX + 80, (int)m_tInfo.fY - 55 + iScrollY, szEnter, _tcslen(szEnter));
	}

	delete image;

	if (g_bDevmode) {
		Renderbox(hDC, m_tRenderRect, iScrollX, iScrollY);
		Hitbox(hDC, m_tRenderRect, iScrollX, iScrollY);
		DetectRect(hDC, m_HitBox, iScrollX, iScrollY);
	}
}

void CShopSign::Release()
{
}

void CShopSign::OnCollision()
{
	RECT rc;
	CObject* _player = CObjectManager::Get_Instance()->Get_Player();
	if (IntersectRect(&rc, _player->Get_Rect(), &m_HitBox)) {
		m_bActive = true;
	}
}

void CShopSign::Key_Input()
{
	if (CKeyManager::Get_Instance()->Key_Down(KEY_SHOPOPEN, 'J')&& m_bActive) {
		dynamic_cast<CShopScene*>(CSceneManager::GetInstance()->Get_Scene())->Set_ShopOpen();
	}
}
