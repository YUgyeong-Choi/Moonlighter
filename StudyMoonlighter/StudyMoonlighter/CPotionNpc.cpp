#include "pch.h"
#include "CPotionNpc.h"
#include "CScrollManager.h"
#include "CObjectManager.h"
#include "CKeyManager.h"
#include "CPlayer.h"
#include "CUiManager.h"

CPotionNpc::CPotionNpc():m_bOpen(false), m_bCollision(false)
{
}

void CPotionNpc::Initialize()
{
	m_eOBJID = OBJ_NPC;

	m_tInfo.fCX = 0.f;
	m_tInfo.fCY = 0.f;

	m_tRenderSizeX = 80.f;
	m_tRenderSizeY = 80.f;
	m_eRender = RENDER_GAMEOBJECT;

	m_HitBox = { (int)m_tInfo.fX - 20, (int)m_tInfo.fY,(int)m_tInfo.fX + 20,(int)m_tInfo.fY + 60 };
}

int CPotionNpc::Update()
{
	Key_Input();
	__super::Update_Rect();
	return 0;
}

void CPotionNpc::Late_Update()
{
	m_bCollision = false;
	OnCollision();
}

void CPotionNpc::Render(HDC hDC)
{
	Image* image(nullptr);
	Graphics graphics(hDC);

	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();
	image = Image::FromFile(L"../MoonlighterAssets/PotionNpc/Witch_idle.png");

	graphics.DrawImage(image, (int)m_tRenderRect.left + iScrollX, (int)m_tRenderRect.top + iScrollY, 0, 0, (int)m_tRenderSizeX, (int)m_tRenderSizeY, UnitPixel);

	if (m_bCollision) {
		COLORREF color = RGB(234, 221, 187);

		HBRUSH hBrush = CreateSolidBrush(color);
		HPEN hPen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
		RoundRect(hDC, m_tInfo.fX + iScrollX + 20, m_tInfo.fY - 70 + iScrollY, m_tInfo.fX + 170 + iScrollX, m_tInfo.fY - 20 + iScrollY, 30, 30);

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
		_stprintf_s(szEnter, _T("물약 상점"));
		TextOut(hDC, (int)m_tInfo.fX + iScrollX + 80, (int)m_tInfo.fY - 55 + iScrollY, szEnter, _tcslen(szEnter));

		delete image;
	}
	delete image;

	if (g_bDevmode) {
		Hitbox(hDC, m_HitBox, iScrollX, iScrollY);
	}
}

void CPotionNpc::Release()
{
}

void CPotionNpc::OnCollision()
{
	
	RECT rc;
	if (IntersectRect(&rc, &m_HitBox, CObjectManager::Get_Instance()->Get_Player()->Get_Rect())){
		m_bCollision =true;
	}
}

void CPotionNpc::Key_Input()
{
	if (CKeyManager::Get_Instance()->Key_Down(KEY_SHOP, 'J') && m_bCollision) {
		m_bOpen = !m_bOpen;
		static_cast<CPlayer*>(CObjectManager::Get_Instance()->Get_Player())->Set_Inven();
		if (CUiManager::GetInstance()->Get_UiType() == UI_POTIONSHOP) {
			CUiManager::GetInstance()->Set_UiType(UI_END);
		}
		else {
			CUiManager::GetInstance()->Set_UiType(UI_POTIONSHOP);
			CUiManager::GetInstance()->Get_PotionShop()->InitXY();
		}
	}
}
