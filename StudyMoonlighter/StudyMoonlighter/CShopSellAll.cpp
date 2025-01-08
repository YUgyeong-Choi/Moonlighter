#include "pch.h"
#include "CShopSellAll.h"
#include "CScrollManager.h"
#include "CObjectManager.h"
#include "CKeyManager.h"
#include "CUiManager.h"
#include "CShopPlayer.h"
#include "CSoundManager.h"

CShopSellAll::CShopSellAll():m_bActive(false)
{
}

void CShopSellAll::Initialize()
{
	m_eOBJID = OBJ_MAPOBJ;
	m_eRender = RENDER_GAMEOBJECT;
	m_tInfo.fCX = 50;
	m_tInfo.fCY = 50;
	m_tRenderSizeX = 64;
	m_tRenderSizeY = 64;
	m_HitBox = { (int)m_tInfo.fX+30, (int)m_tInfo.fY - 20,(int)m_tInfo.fX + 60, (int)m_tInfo.fY + 20 };
}

int CShopSellAll::Update()
{
	Key_Input();
	__super::Update_Rect();
	return 0;
}

void CShopSellAll::Late_Update()
{
	m_bActive = false;
	OnCollision();
}

void CShopSellAll::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();
	Graphics graphics(hDC);
	Image* image(nullptr);
	image = Image::FromFile(L"../MoonlighterAssets/Map/Shop/sellall.png");
	graphics.DrawImage(image, (int)m_tRenderRect.left + iScrollX, (int)m_tRenderRect.top + iScrollY, 0, 0, (int)m_tRenderSizeX, (int)m_tRenderSizeY, UnitPixel);

	if (m_bActive) {
		COLORREF color = RGB(234, 221, 187);

		HBRUSH hBrush = CreateSolidBrush(color);
		HPEN hPen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
		RoundRect(hDC, m_tInfo.fX + iScrollX + 20 - 80, m_tInfo.fY - 70 + iScrollY, m_tInfo.fX + 160 + iScrollX - 80, m_tInfo.fY - 20 + iScrollY, 30, 30);

		SelectObject(hDC, hOldBrush);
		SelectObject(hDC, hOldPen);
		DeleteObject(hBrush);
		DeleteObject(hPen);

		Image* image(nullptr);
		Graphics graphics(hDC);
		image = Image::FromFile(L"../MoonlighterAssets/Ui/button_K.png");
		graphics.DrawImage(image, (int)m_tInfo.fX + iScrollX + 15 - 80, (int)m_tInfo.fY - 75 + iScrollY, 0, 0, 64, 64, UnitPixel);

		SetTextColor(hDC, RGB(0, 0, 0));
		TCHAR szEnter[64];
		_stprintf_s(szEnter, _T("¸ðµÎ ÆÈ±â"));
		TextOut(hDC, (int)m_tInfo.fX + iScrollX + 80 - 80, (int)m_tInfo.fY - 55 + iScrollY, szEnter, _tcslen(szEnter));
	}


	delete image;
	if (g_bDevmode) {
		Renderbox(hDC, m_tRenderRect, iScrollX, iScrollY);
		Hitbox(hDC, m_tRect, iScrollX, iScrollY);
		DetectRect(hDC, m_HitBox, iScrollX, iScrollY);
	}

}

void CShopSellAll::Release()
{
}

void CShopSellAll::SellAll()
{
	vector<vector<CInvenSlot*>> inven = CUiManager::GetInstance()->Get_Inventory();
	CObject* _player = CObjectManager::Get_Instance()->Get_Player();
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 5; ++j) {
			if (inven[i][j] != nullptr) {
				if (inven[i][j]->Get_Item().itemId != ITEM_END) {
					static_cast<CShopPlayer*>(_player)->Set_Money(inven[i][j]->Get_Item().price * inven[i][j]->Get_Item().num);
					inven[i][j]->Item_Init();
				}
			}
		}
	}
}

void CShopSellAll::OnCollision()
{
	RECT rc;
	CObject* _player = CObjectManager::Get_Instance()->Get_Player();
	if (IntersectRect(&rc, _player->Get_Rect(), &m_HitBox)) {
		m_bActive = true;
	}
}

void CShopSellAll::Key_Input()
{
	if (CKeyManager::Get_Instance()->Key_Down(KEY_SHOPOPEN, 'K') && m_bActive) {
		SellAll();
		CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
		CSoundManager::Get_Instance()->PlaySound(L"shop_item_sold.wav", SOUND_EFFECT, g_fEffectVolume, true);
	}
}
