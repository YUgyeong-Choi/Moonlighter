#include "pch.h"
#include "CShowCase.h"
#include "CScrollManager.h"
#include "CObjectManager.h"
#include "CKeyManager.h"
#include "CShopPlayer.h"
#include "CUiManager.h"
#include "CInventory.h"

CShowCase::CShowCase():m_bActive(false)
{
	m_sellItem.itemId = ITEM_END;
}

void CShowCase::Initialize()
{
	m_eOBJID = OBJ_MAPOBJ;
	m_tInfo.fCX = 48;
	m_tInfo.fCY = 64;

	m_eRender = RENDER_GAMEOBJECT;
	m_tRenderSizeX = 48;
	m_tRenderSizeY = 64;
	m_HitBox = { (int)m_tInfo.fX - 40,(int)m_tInfo.fY - 40,(int)m_tInfo.fX + 40,(int)m_tInfo.fY + 40 };
}

int CShowCase::Update()
{
	KeyInput();
	__super::Update_Rect();
	return 0;
}

void CShowCase::Late_Update()
{
	m_bActive = false;
	OnCollision();
}

void CShowCase::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

	Image* image(nullptr);
	Graphics graphics(hDC);
	image = Image::FromFile(L"../MoonlighterAssets/Map/Shop/shop_itemtable.png");
	graphics.DrawImage(image, (int)m_tRenderRect.left + iScrollX, (int)m_tRenderRect.top + iScrollY, 0, 0, (int)m_tRenderSizeX, (int)m_tRenderSizeY, UnitPixel);

	if (m_bActive) {
		COLORREF color = RGB(234, 221, 187);

		HBRUSH hBrush = CreateSolidBrush(color);
		HPEN hPen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
		RoundRect(hDC, 450 + iScrollX + 20, 640 - 70 + iScrollY,450 + 140 + iScrollX, 640 - 20 + iScrollY, 30, 30);

		SelectObject(hDC, hOldBrush);
		SelectObject(hDC, hOldPen);
		DeleteObject(hBrush);
		DeleteObject(hPen);

		Image* image(nullptr);
		Graphics graphics(hDC);
		image = Image::FromFile(L"../MoonlighterAssets/Ui/button_J.png");
		graphics.DrawImage(image, (int)450 + iScrollX + 15, (int)640 - 75 + iScrollY, 0, 0, 64, 64, UnitPixel);

		SetTextColor(hDC, RGB(0, 0, 0));
		TCHAR szEnter[64];
		_stprintf_s(szEnter, _T("¹èÄ¡"));
		TextOut(hDC, (int)450 + iScrollX + 80, (int)640 - 55 + iScrollY, szEnter, _tcslen(szEnter));

	}

	if (g_bDevmode) {
		Hitbox(hDC, m_tRect, iScrollX, iScrollY);
		Renderbox(hDC, m_tRenderRect, iScrollX, iScrollY);
		DetectRect(hDC, m_HitBox, iScrollX, iScrollY);
	}
}

void CShowCase::Release()
{
}

void CShowCase::OnCollision()
{
	RECT rc{};
	if (IntersectRect(&rc, &m_HitBox, CObjectManager::Get_Instance()->Get_Player()->Get_Rect()))
	{
		m_bActive = true;

	}
}

void CShowCase::KeyInput()
{
	CObject* _player = CObjectManager::Get_Instance()->Get_Player();
	CShopPlayer* _shopPlayer = static_cast<CShopPlayer*>(_player);

	if (CKeyManager::Get_Instance()->Key_Down(KEY_INVEN,'J')&&m_bActive) {
		_shopPlayer->Set_Inven();
		if (_shopPlayer->Get_Inven()) {
			CUiManager::GetInstance()->Set_UiType(UI_INVENSHOP);
			//static_cast<CInventory*>(CUiManager::GetInstance()->Get_Inven())->InitXY();
		}
		else {
			CUiManager::GetInstance()->Set_UiType(UI_END);
		}
	}
}
