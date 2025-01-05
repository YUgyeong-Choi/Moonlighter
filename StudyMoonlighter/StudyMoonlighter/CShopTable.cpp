#include "pch.h"
#include "CShopTable.h"
#include "CScrollManager.h"
#include "CObjectManager.h"

CShopTable::CShopTable():m_bActive(false)
{
}

void CShopTable::Initialize()
{
	m_eOBJID = OBJ_MAPOBJ;
	m_eRender = RENDER_GAMEOBJECT;
	m_tInfo.fCX = 180;
	m_tInfo.fCY = 60;
	m_tRenderSizeX = 200;
	m_tRenderSizeY = 74;
	m_HitBox = { 610, 530, 670, 600 };
}

int CShopTable::Update()
{
	__super::Update_Rect();
	return 0;
}

void CShopTable::Late_Update()
{
	m_bActive = false;
	OnCollision();
}

void CShopTable::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();
	Graphics graphics(hDC);
	Image* image = Image::FromFile(L"../MoonlighterAssets/Map/Shop/shop_main_table.png");
	graphics.DrawImage(image, (int)m_tRenderRect.left + iScrollX, (int)m_tRenderRect.top + iScrollY, 0, 0, (int)m_tRenderSizeX, (int)m_tRenderSizeY, UnitPixel);

	image = Image::FromFile(L"../MoonlighterAssets/Map/Shop/cash_register01.png");
	graphics.DrawImage(image, (int)m_tRenderRect.left + iScrollX +15, (int)m_tRenderRect.top + iScrollY, 0, 0, 52, 46, UnitPixel);
	delete image;

	if (m_bActive) {
		COLORREF color = RGB(234, 221, 187);

		HBRUSH hBrush = CreateSolidBrush(color);
		HPEN hPen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
		RoundRect(hDC, m_tInfo.fX + iScrollX + 20, m_tInfo.fY - 70 + iScrollY, m_tInfo.fX + 140 + iScrollX, m_tInfo.fY - 20 + iScrollY, 30, 30);

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
		_stprintf_s(szEnter, _T("°è»ê"));
		TextOut(hDC, (int)m_tInfo.fX + iScrollX + 80, (int)m_tInfo.fY - 55 + iScrollY, szEnter, _tcslen(szEnter));
	}

	if (g_bDevmode) {
		Hitbox(hDC, m_tRect, iScrollX, iScrollY);
		Renderbox(hDC, m_tRenderRect, iScrollX, iScrollY);
		DetectRect(hDC, m_HitBox, iScrollX, iScrollY);
	}
}

void CShopTable::Release()
{
}

void CShopTable::OnCollision()
{
	RECT rc;
	CObject* _player = CObjectManager::Get_Instance()->Get_Player();
	if (IntersectRect(&rc, _player->Get_Rect(), &m_HitBox)) {
		m_bActive = true;
	}
}
