#include "pch.h"
#include "CUiManager.h"
#include "CObjectManager.h"
#include "CSceneManager.h"
#include "CPlayer.h"
#include "CShopPlayer.h"
#include "CSpecialSlot.h"
#include "CBitManager.h"
CUiManager* CUiManager::m_pInstance = nullptr;

void CUiManager::Initialize()
{
	ADD_BMP(L"../MoonlighterAssets/Ui/Inventory_base.bmp", L"InventoryBase");
	ADD_BMP(L"../MoonlighterAssets/Ui/Showcase_base.bmp", L"ShowcaseBase");
	ADD_BMP(L"../MoonlighterAssets/Ui/Bag_paperbase.bmp", L"BagPaperbase");
	ADD_BMP(L"../MoonlighterAssets/Ui/Gold_circle.bmp", L"GoldCircle");
	ADD_BMP(L"../MoonlighterAssets/Gamble/slotmachine_ui.bmp", L"Slotmachine");
	ADD_BMP(L"../MoonlighterAssets/Gamble/Gold_Falling.bmp", L"GoldFalling");

	ADD_BMP(L"../MoonlighterAssets/Gamble/banana.bmp", L"Banana");
	ADD_BMP(L"../MoonlighterAssets/Gamble/cherry.bmp", L"Cherry");
	ADD_BMP(L"../MoonlighterAssets/Gamble/grape.bmp", L"Grape");
	ADD_BMP(L"../MoonlighterAssets/Gamble/lemon.bmp", L"Lemon");
	ADD_BMP(L"../MoonlighterAssets/Gamble/orange.bmp", L"Orange");
	ADD_BMP(L"../MoonlighterAssets/Gamble/strawberry.bmp", L"Strawberry");
	ADD_BMP(L"../MoonlighterAssets/Gamble/watermelon.bmp", L"Watermelon");

	inventory.resize(4);
	for (int i = 0; i < 4; ++i) {
		inventory[i].resize(7);
		for (int j = 0; j < 7; ++j) {
			inventory[i][j] = nullptr;
		}
	}

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 5; ++j) {
			inventory[i][j] = new CInvenSlot(i, j);
		}
	}

	inventory[1][5] = new CSpecialSlot(HELMET);
	inventory[2][5] = new CSpecialSlot(ARMOR);
	inventory[3][5] = new CSpecialSlot(BOOTS);
	inventory[0][5] = new CSpecialSlot(WEAPON1);
	inventory[0][6] = new CSpecialSlot(WEAPON2);
	inventory[2][6] = new CSpecialSlot(OUTFIT_POTION);

	m_Inven = new CInventory();
	m_Inven->Copy_Inven(inventory);

	m_InvenShop = new CInventoryShop();
	m_InvenShop->Initialize();
	m_InvenShop->Copy_Inven(inventory);
	m_bGetMoney = false;

	m_potionShop = new CPotionShop();
	m_potionShop->Initialize();

	m_gamble = new CGambleKey();
	m_gamble->Initialize();

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 4;
	m_tFrame.dwSpeed = 120;
	m_tFrame.dwTime = GetTickCount64();
}

void CUiManager::Update()
{
	m_Inven->Update();
	m_InvenShop->Update();
	m_potionShop->Update();
	m_gamble->Update();

	if (m_bGetMoney) {
		if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount64())
		{
			++m_tFrame.iFrameStart;

			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd) {
				m_tFrame.iFrameStart = 0;
				m_bGetMoney = false;
			}
			m_tFrame.dwTime = GetTickCount64();
		}
	}
}

void CUiManager::Late_Update()
{
	m_potionShop->Late_Update();
	m_gamble->Late_Update();
}

void CUiManager::Render(HDC hDC)
{
	if (m_eCurUi == UI_END) {
		SCENEID _id = CSceneManager::GetInstance()->Get_SceneID();
		switch (_id)
		{
		case SC_MENU:
			break;
		case SC_EDIT:
		case SC_GOLEMDUNGEON:
			m_eCurUi = UI_DUNGEON;
			break;
		case SC_VILLAGE:
		case SC_FIELD:
		case SC_TUTORIAL:
			m_eCurUi = UI_WEAPON;
			break;
		case SC_GOLEMBOSS:
			m_eCurUi = UI_BOSS;
			break;
		case SC_SHOP:
			m_eCurUi = UI_SHOPBASIC;
			break;
		case SC_FINALANI:
			break;
		case SC_END:
			break;
		default:
			break;
		}
	}

	switch (m_eCurUi)
	{
	case UI_DUNGEON:
		Basic_Ui(hDC);
		Weapon_Ui(hDC);
		Dungeon_Ui(hDC);
		break;
	case UI_INVEN:
		Basic_Ui(hDC);
		Inven_Ui(hDC);
		break;
	case UI_SHOPBASIC:
		Basic_Ui(hDC);
		break;
	case UI_INVENSHOP:
		Basic_Ui(hDC);
		Shop_Ui(hDC);
		break;
	case UI_WEAPON:
		Basic_Ui(hDC);
		Weapon_Ui(hDC);
		break;
	case UI_BOSS:
		Basic_Ui(hDC);
		Weapon_Ui(hDC);
		Boss_Ui(hDC);
		break;
	case UI_POTIONSHOP:
		PotionShop_Ui(hDC);
		break;
	case UI_GAMBLE:
		GambleUi(hDC);
		break;
	case UI_END:
		break;
	default:
		break;
	}
}

void CUiManager::Release()
{
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 7; ++j) {
			Safe_Delete<CInvenSlot*>(inventory[i][j]);
		}
	}
	Safe_Delete<CInventory*>(m_Inven);
	Safe_Delete<CInventoryShop*>(m_InvenShop);
	Safe_Delete<CPotionShop*>(m_potionShop);
	Safe_Delete<CGambleKey*>(m_gamble);
}

void CUiManager::Basic_Ui(HDC hDC)
{
	Image* image(nullptr);
	Graphics graphics(hDC);
	HDC hMemDC = CBitManager::GetInstance()->FindImage(L"GoldCircle");
	GdiTransparentBlt(hDC, 5, 20, 70, 70, hMemDC, 0, 0, 70, 70, RGB(255, 255, 255));

	if (CPlayer* _player = dynamic_cast<CPlayer*>(CObjectManager::Get_Instance()->Get_Player())) {
		if (_player->Get_Money() > 9999) {
			image = Image::FromFile(L"../MoonlighterAssets/Ui/Gold2.png");
			graphics.DrawImage(image, 11, 26, 0, 0, 64, 64, UnitPixel);
		}
		else {
			image = Image::FromFile(L"../MoonlighterAssets/Ui/Gold1.png");
			graphics.DrawImage(image, 11, 26, 0, 0, 64, 64, UnitPixel);
		}
	}

	if (CShopPlayer* _player = dynamic_cast<CShopPlayer*>(CObjectManager::Get_Instance()->Get_Player())) {
		if (_player->Get_Money() > 9999) {
			image = Image::FromFile(L"../MoonlighterAssets/Ui/Gold2.png");
			graphics.DrawImage(image, 11, 26, 0, 0, 64, 64, UnitPixel);
		}
		else {
			image = Image::FromFile(L"../MoonlighterAssets/Ui/Gold1.png");
			graphics.DrawImage(image, 11, 26, 0, 0, 64, 64, UnitPixel);
		}
	}

	

	//hp 바
	image = Image::FromFile(L"../MoonlighterAssets/Ui/HealthBar_Circle.png");
	graphics.DrawImage(image, 80, 20, 0, 0, 48, 48, UnitPixel);

	image = Image::FromFile(L"../MoonlighterAssets/Ui/HealthBar_Heart_Big.png");
	graphics.DrawImage(image, 88, 30, 0, 0, 32, 32, UnitPixel);

	image = Image::FromFile(L"../MoonlighterAssets/Ui/HealthBar_Base.png");
	graphics.DrawImage(image, 115, 22, 0, 0, 146, 44, UnitPixel);

	int hpWidth = 0;
	if (CObjectManager::Get_Instance()->Get_Player()->Get_Hp() < 0) {
		hpWidth = (132 * 0 / CObjectManager::Get_Instance()->Get_Player()->Get_MaxHp());
	}
	else {
		hpWidth = (132 * CObjectManager::Get_Instance()->Get_Player()->Get_Hp()) / CObjectManager::Get_Instance()->Get_Player()->Get_MaxHp();
	}
	COLORREF color = RGB(237, 52, 52);

	HBRUSH hBrush = CreateSolidBrush(color);
	HPEN hPen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
	HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
	RoundRect(hDC, 128, 32, 128 + hpWidth, 57, 10, 10);

	SelectObject(hDC, hOldBrush);
	SelectObject(hDC, hOldPen);
	DeleteObject(hBrush);
	DeleteObject(hPen);

	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkMode(hDC, TRANSPARENT);

	HFONT hFont1 = CreateFont(
		25, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"m3x6"
	);

	HFONT OldFont = (HFONT)SelectObject(hDC, hFont1);

	TCHAR szHpBar[64];
	_stprintf_s(szHpBar, _T("%d/%d"), CObjectManager::Get_Instance()->Get_Player()->Get_Hp(), CObjectManager::Get_Instance()->Get_Player()->Get_MaxHp());

	RECT rect = { 160, 50, 260, 70 };
	DrawText(hDC, szHpBar, _tcslen(szHpBar), &rect, DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

	//Money
	TCHAR szMoney[64];
	if (CPlayer* _player = dynamic_cast<CPlayer*>(CObjectManager::Get_Instance()->Get_Player())) {
		_stprintf_s(szMoney, _T("%d"), _player->Get_Money());
		RECT rect2 = { 20, 100, 80, 120 };
		DrawText(hDC, szMoney, _tcslen(szMoney), &rect2, DT_RIGHT | DT_SINGLELINE | DT_VCENTER);
	}
	else if (CShopPlayer* _player = dynamic_cast<CShopPlayer*>(CObjectManager::Get_Instance()->Get_Player())) {
		_stprintf_s(szMoney, _T("%d"), _player->Get_Money());
		RECT rect2 = { 20, 100, 80, 120 };
		DrawText(hDC, szMoney, _tcslen(szMoney), &rect2, DT_RIGHT | DT_SINGLELINE | DT_VCENTER);
	}

	SelectObject(hDC, OldFont);
	DeleteObject(hFont1);

	image = Image::FromFile(L"../MoonlighterAssets/Ui/Coin.png");
	graphics.DrawImage(image, 10, 105, 0, 0, 18, 18, UnitPixel);

	if (m_bGetMoney) {
		image = Image::FromFile(L"../MoonlighterAssets/Ui/Gold_Falling.png");
		graphics.DrawImage(image, 25, -5 + m_tFrame.iFrameStart * 7, 0, m_tFrame.iFrameStart * 32, 32, 32, UnitPixel);
	}


	delete image;
}

void CUiManager::Dungeon_Ui(HDC hDC)
{
	Image* image(nullptr);
	Graphics graphics(hDC);

	image = Image::FromFile(L"../MoonlighterAssets/Ui/Gold_circle.png");
	graphics.DrawImage(image, 920, 570, 0, 0, 70, 70, UnitPixel);

	image = Image::FromFile(L"../MoonlighterAssets/Ui/Bag_Pendant.png");
	graphics.DrawImage(image, 920, 570, 0, 0, 70, 70, UnitPixel);

	delete image;
}

void CUiManager::Inven_Ui(HDC hDC)
{
	Image* image(nullptr);
	Graphics graphics(hDC);

	HDC hMemDC = CBitManager::GetInstance()->FindImage(L"InventoryBase");
	GdiTransparentBlt(hDC, 100, 100, 830, 438, hMemDC, 0, 0, 830, 438, RGB(255, 255, 255));
	m_Inven->Render(hDC);
	delete image;
}

void CUiManager::Shop_Ui(HDC hDC)
{
	Image* image(nullptr);
	Graphics graphics(hDC);

	HDC hMemDC = CBitManager::GetInstance()->FindImage(L"BagPaperbase");
	GdiTransparentBlt(hDC, 100, 100, 385, 435, hMemDC, 0, 0, 385, 435, RGB(255, 255, 255));

	hMemDC = CBitManager::GetInstance()->FindImage(L"ShowcaseBase");
	GdiTransparentBlt(hDC, 500, 120, 198, 190, hMemDC, 0, 0, 198, 190, RGB(255, 255, 255));
	GdiTransparentBlt(hDC, 700, 120, 198, 190, hMemDC, 0, 0, 198, 190, RGB(255, 255, 255));
	GdiTransparentBlt(hDC, 500, 320, 198, 190, hMemDC, 0, 0, 198, 190, RGB(255, 255, 255));
	GdiTransparentBlt(hDC, 700, 320, 198, 190, hMemDC, 0, 0, 198, 190, RGB(255, 255, 255));

	m_InvenShop->Render(hDC);
	delete image;
}

void CUiManager::Weapon_Ui(HDC hDC)
{
	Image* image(nullptr);
	Graphics graphics(hDC);
	bool check = static_cast<CPlayer*>(CObjectManager::Get_Instance()->Get_Player())->Get_FirstWeapon();
	image = Image::FromFile(L"../MoonlighterAssets/Ui/Gold_circle.png");
	graphics.DrawImage(image, 945, 50, 0, 0, 70, 70, UnitPixel);
	if (check) {
		if (Get_Wepon1()->Get_Item().itemId == SWORD) {
			image = Image::FromFile(Get_Wepon1()->Get_Item().pImageUrl);
			graphics.DrawImage(image, 955, 60, 0, 0, 70, 70, UnitPixel);
		}
		else if (Get_Wepon1()->Get_Item().itemId == BOW) {
			image = Image::FromFile(Get_Wepon1()->Get_Item().pImageUrl);
			graphics.DrawImage(image, 955, 60, 0, 0, 70, 70, UnitPixel);
		}
	}
	else {
		if (Get_Wepon2()->Get_Item().itemId == SWORD) {
			image = Image::FromFile(Get_Wepon2()->Get_Item().pImageUrl);
			graphics.DrawImage(image, 955, 60, 0, 0, 70, 70, UnitPixel);
		}
		else if (Get_Wepon2()->Get_Item().itemId == BOW) {
			image = Image::FromFile(Get_Wepon2()->Get_Item().pImageUrl);
			graphics.DrawImage(image, 955, 60, 0, 0, 70, 70, UnitPixel);
		}
	}

	image = Image::FromFile(L"../MoonlighterAssets/Ui/button_Z.png");
	graphics.DrawImage(image, 925, 95, 0, 0, 64, 64, UnitPixel);


	image = Image::FromFile(L"../MoonlighterAssets/Items/potion.png");
	graphics.DrawImage(image, 970, 2, 0, 0, 48, 48, UnitPixel);

	image = Image::FromFile(L"../MoonlighterAssets/Ui/button_E.png");
	graphics.DrawImage(image, 930, -4, 0, 0, 64, 64, UnitPixel);

	image = Image::FromFile(L"../MoonlighterAssets/Ui/button_Space.png");
	graphics.DrawImage(image, 880, -4, 0, 0, 64, 64, UnitPixel);

	image = Image::FromFile(L"../MoonlighterAssets/Ui/will_roll.png");
	graphics.DrawImage(image, 865, 30, 0, 0, 35, 40, UnitPixel);

	image = Image::FromFile(L"../MoonlighterAssets/Ui/Gold_circle.png");
	graphics.DrawImage(image, 945, 150, 0, 0, 70, 70, UnitPixel);

	image = Image::FromFile(L"../MoonlighterAssets/Ui/Bag.png");
	graphics.DrawImage(image, 958, 158, 0, 0, 46, 52, UnitPixel);

	image = Image::FromFile(L"../MoonlighterAssets/Ui/Button_I.png");
	graphics.DrawImage(image, 925, 195, 0, 0, 64, 64, UnitPixel);

	delete image;
}

void CUiManager::Boss_Ui(HDC hDC)
{
	Image* image(nullptr);
	Graphics graphics(hDC);

	image = Image::FromFile(L"../MoonlighterAssets/Ui/boss_healbar.png");
	graphics.DrawImage(image, WINCX/2-442, 680, 0, 0, 884, 24, UnitPixel);

	int hpWidth = 0;
	if (CObjectManager::Get_Instance()->Get_Boss()->Get_Hp() < 0) {
		hpWidth = (0 / CObjectManager::Get_Instance()->Get_Player()->Get_MaxHp());
	}
	else {
		hpWidth = (865 * CObjectManager::Get_Instance()->Get_Boss()->Get_Hp() / CObjectManager::Get_Instance()->Get_Boss()->Get_MaxHp());
	}
	COLORREF color = RGB(237, 52, 52);

	HBRUSH hBrush = CreateSolidBrush(color);
	HPEN hPen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
	HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
	RoundRect(hDC, WINCX / 2 - 441 +10, 685, WINCX / 2 - 441 + 10 + hpWidth, 700, 20, 20);

	SelectObject(hDC, hOldBrush);
	SelectObject(hDC, hOldPen);
	DeleteObject(hBrush);
	DeleteObject(hPen);
	delete image;
}

void CUiManager::PotionShop_Ui(HDC hDC)
{
	Image* image(nullptr);
	Graphics graphics(hDC);

	image = Image::FromFile(L"../MoonlighterAssets/Back/Back.png");
	ImageAttributes imgAttr;
	ColorMatrix cm = {
	1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 240 / 255.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	imgAttr.SetColorMatrix(&cm);
	graphics.DrawImage(image, Gdiplus::Rect(0, 0, 1024, 720), 0, 0, 1024, 720, UnitPixel, &imgAttr);


	//왼쪽 위
	image = Image::FromFile(L"../MoonlighterAssets/Ui/Gold_circle.png");
	graphics.DrawImage(image, 5, 20, 0, 0, 70, 70, UnitPixel);

	image = Image::FromFile(L"../MoonlighterAssets/Ui/Gold1.png");
	graphics.DrawImage(image, 11, 26, 0, 0, 64, 64, UnitPixel);

	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkMode(hDC, TRANSPARENT);

	HFONT hFont1 = CreateFont(
		25, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"m3x6"
	);

	HFONT OldFont = (HFONT)SelectObject(hDC, hFont1);

	TCHAR szMoney[64];
	if (CPlayer* _player = dynamic_cast<CPlayer*>(CObjectManager::Get_Instance()->Get_Player())) {
		_stprintf_s(szMoney, _T("%d"), _player->Get_Money());
		RECT rect2 = { 20, 100, 70, 120 };
		DrawText(hDC, szMoney, _tcslen(szMoney), &rect2, DT_RIGHT | DT_SINGLELINE | DT_VCENTER);
	}
	else if (CShopPlayer* _player = dynamic_cast<CShopPlayer*>(CObjectManager::Get_Instance()->Get_Player())) {
		_stprintf_s(szMoney, _T("%d"), _player->Get_Money());
		RECT rect2 = { 20, 100, 70, 120 };
		DrawText(hDC, szMoney, _tcslen(szMoney), &rect2, DT_RIGHT | DT_SINGLELINE | DT_VCENTER);
	}

	SelectObject(hDC, OldFont);
	DeleteObject(hFont1);

	image = Image::FromFile(L"../MoonlighterAssets/Ui/Coin.png");
	graphics.DrawImage(image, 10, 105, 0, 0, 18, 18, UnitPixel);

	image = Image::FromFile(L"../MoonlighterAssets/PotionNpc/Witch_Figure_Potions.png");
	graphics.DrawImage(image, 50, 250, 0, 0, 300, 412, UnitPixel);

	image = Image::FromFile(L"../MoonlighterAssets/PotionNpc/Items_Base.png");
	graphics.DrawImage(image, 350, 100, 0, 0, 450, 227, UnitPixel);

	SetTextColor(hDC, RGB(0, 0, 0));
	SetBkMode(hDC, TRANSPARENT);

	HFONT hFont2 = CreateFont(
		25, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"m3x6"
	);

	HFONT OldFont2 = (HFONT)SelectObject(hDC, hFont2);

	TCHAR szTitle[64];
	_stprintf_s(szTitle, _T("물약 목록: "));
	RECT rect2 = { 380, 120, 480, 140 };
	DrawText(hDC, szTitle, _tcslen(szTitle), &rect2, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

	SelectObject(hDC, OldFont2);
	DeleteObject(hFont2);

	image = Image::FromFile(L"../MoonlighterAssets/PotionNpc/Witch_Jar.png");
	graphics.DrawImage(image, 420, 350, 0, 0, 300, 326, UnitPixel);

	delete image;

	m_potionShop->Render(hDC);
}

void CUiManager::GambleUi(HDC hDC)
{
	Image* image(nullptr);
	Graphics graphics(hDC);

	image = Image::FromFile(L"../MoonlighterAssets/Back/Back.png");
	ImageAttributes imgAttr;
	ColorMatrix cm = {
	1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 240 / 255.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	imgAttr.SetColorMatrix(&cm);
	graphics.DrawImage(image, Gdiplus::Rect(0, 0, 1024, 720), 0, 0, 1024, 720, UnitPixel, &imgAttr);

	HDC hMemDC = CBitManager::GetInstance()->FindImage(L"Slotmachine");
	GdiTransparentBlt(hDC, (int)(WINCX / 2 - 355 / 2), (int)(WINCY / 2 - 600 / 2), 355, 600, hMemDC, 0, 0, 355, 600, RGB(255, 255, 255));

	image = Image::FromFile(L"../MoonlighterAssets/Ui/button_Space.png");
	graphics.DrawImage(image, 750, WINCY/2-32, 0, 0, 64, 64, UnitPixel);

	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkMode(hDC, TRANSPARENT);

	HFONT hFont1 = CreateFont(
		25, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"m3x6"
	);

	HFONT OldFont = (HFONT)SelectObject(hDC, hFont1);

	TCHAR szClick[64];
	_stprintf_s(szClick, _T("Space!"));
	RECT rect2 = { 790, WINCY/2-50, 890, WINCY / 2 + 50 };
	DrawText(hDC, szClick, _tcslen(szClick), &rect2, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	image = Image::FromFile(L"../MoonlighterAssets/Ui/button_R.png");
	graphics.DrawImage(image, 750, WINCY / 2 - 32-100, 0, 0, 64, 64, UnitPixel);

	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkMode(hDC, TRANSPARENT);

	TCHAR szAgain[64];
	_stprintf_s(szAgain, _T("Again!"));
	RECT rect3 = { 790, WINCY / 2 - 50-100, 890, WINCY / 2 + 50 -100};
	DrawText(hDC, szAgain, _tcslen(szAgain), &rect3, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	TCHAR szMoney[64];
	if (CPlayer* _player = dynamic_cast<CPlayer*>(CObjectManager::Get_Instance()->Get_Player())) {
		_stprintf_s(szMoney, _T("%d"), _player->Get_Money());
		RECT rect2 = { 20, 100, 70, 120 };
		DrawText(hDC, szMoney, _tcslen(szMoney), &rect2, DT_RIGHT | DT_SINGLELINE | DT_VCENTER);
	}
	else if (CShopPlayer* _player = dynamic_cast<CShopPlayer*>(CObjectManager::Get_Instance()->Get_Player())) {
		_stprintf_s(szMoney, _T("%d"), _player->Get_Money());
		RECT rect2 = { 20, 100, 70, 120 };
		DrawText(hDC, szMoney, _tcslen(szMoney), &rect2, DT_RIGHT | DT_SINGLELINE | DT_VCENTER);
	}

	image = Image::FromFile(L"../MoonlighterAssets/Ui/Coin.png");
	graphics.DrawImage(image, 10, 105, 0, 0, 18, 18, UnitPixel);


	SelectObject(hDC, OldFont);
	DeleteObject(hFont1);

	delete image;
	m_gamble->Render(hDC);
}


void CUiManager::AddItem(ITEMTYPE _item)
{
	if (FindItem(_item)) {
		return;
	}
	else {
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 5; ++j) {
				if (inventory[i][j]->Get_Item().itemId == ITEM_END) {
					inventory[i][j]->Set_ItemType(_item);
					return;
				}
			}
		}
	}
}

bool CUiManager::FindItem(ITEMTYPE _item)
{
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 5; ++j) {
			if (_item == inventory[i][j]->Get_Item().itemId) {
				if (inventory[i][j]->Get_Item().maxNum > inventory[i][j]->Get_Item().num) {
					inventory[i][j]->Add_ItemNum();
					return true;
				}
			}
		}
	}
	return false;
}