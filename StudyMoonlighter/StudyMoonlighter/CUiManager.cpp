#include "pch.h"
#include "CUiManager.h"
#include "CObjectManager.h"
#include "CSceneManager.h"
#include "CPlayer.h"
#include "CShopPlayer.h"
#include "CSpecialSlot.h"
CUiManager* CUiManager::m_pInstance = nullptr;

void CUiManager::Initialize()
{
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
	inventory[2][6] = new CSpecialSlot(POTION);

	m_Inven = new CInventory();
	m_Inven->Copy_Inven(inventory);

	m_InvenShop = new CInventoryShop();
	m_InvenShop->Initialize();
	m_InvenShop->Copy_Inven(inventory);
}

void CUiManager::Update()
{
	m_Inven->Update();
	m_InvenShop->Update();
}

void CUiManager::Render(HDC hDC)
{
	if (m_eCurUi == UI_END) {
		SCENEID _id = CSceneManager::GetInstance()->Get_SceneID();
		switch (_id)
		{
		case SC_MENU:
			break;
		case SC_TUTORIAL:
		case SC_EDIT:
		case SC_GOLEMDUNGEON:
		case SC_GOLEMBOSS:
			m_eCurUi = UI_DUNGEON;
			break;
		case SC_VILLAGE:
		case SC_FIELD:
			break;
		case SC_SHOP:
			break;
		case SC_ANIM:
			break;
		case SC_END:
			break;
		default:
			break;
		}
	}

	Image* image(nullptr);
	Graphics graphics(hDC);
	switch (m_eCurUi)
	{
	case UI_DUNGEON:
		Dungeon_Ui(hDC);
		break;
	case UI_INVEN:
		Inven_Ui(hDC);
		break;
	case UI_INVENCHEST:
		break;
	case UI_INVENSHOP:
		Shop_Ui(hDC);
		break;
	case UI_END:
		break;
	default:
		break;
	}

	image = Image::FromFile(L"../MoonlighterAssets/Ui/Gold_circle.png");
	graphics.DrawImage(image, 5, 10, 0, 0, 70, 70, UnitPixel);

	image = Image::FromFile(L"../MoonlighterAssets/Ui/Gold1.png");
	graphics.DrawImage(image, 11, 16, 0, 0, 64, 64, UnitPixel);

	//hp ¹Ù
	image = Image::FromFile(L"../MoonlighterAssets/Ui/HealthBar_Circle.png");
	graphics.DrawImage(image, 80, 20, 0, 0, 48, 48, UnitPixel);

	image = Image::FromFile(L"../MoonlighterAssets/Ui/HealthBar_Heart_Big.png");
	graphics.DrawImage(image, 88, 30, 0, 0, 32, 32, UnitPixel);

	image = Image::FromFile(L"../MoonlighterAssets/Ui/HealthBar_Base.png");
	graphics.DrawImage(image, 115, 22, 0, 0, 146, 44, UnitPixel);

	int hpWidth =  (132 * CObjectManager::Get_Instance()->Get_Player()->Get_Hp()) / CObjectManager::Get_Instance()->Get_Player()->Get_MaxHp();
	COLORREF color = RGB(237, 52, 52);

	HBRUSH hBrush = CreateSolidBrush(color);
	HPEN hPen = CreatePen(PS_NULL, 0, RGB(0, 0, 0)); 
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
	HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
	RoundRect(hDC, 128, 32, 128+ hpWidth, 57, 10, 10);

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

	TCHAR szMoney[64];
	if (CPlayer* _player = dynamic_cast<CPlayer*>(CObjectManager::Get_Instance()->Get_Player())) {
		_stprintf_s(szMoney, _T("%d"), _player->Get_Money());
		RECT rect2 = { 20, 90, 70, 110 };
		DrawText(hDC, szMoney, _tcslen(szMoney), &rect2, DT_RIGHT | DT_SINGLELINE | DT_VCENTER);
	}
	else if (CShopPlayer* _player = dynamic_cast<CShopPlayer*>(CObjectManager::Get_Instance()->Get_Player())) {
		_stprintf_s(szMoney, _T("%d"), _player->Get_Money());
		RECT rect2 = { 20, 90, 70, 110 };
		DrawText(hDC, szMoney, _tcslen(szMoney), &rect2, DT_RIGHT | DT_SINGLELINE | DT_VCENTER);
	}

	SelectObject(hDC, OldFont);
	DeleteObject(hFont1);

	image = Image::FromFile(L"../MoonlighterAssets/Ui/Coin.png");
	graphics.DrawImage(image, 10, 95, 0, 0, 18, 18, UnitPixel);

	delete image;
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

	image = Image::FromFile(L"../MoonlighterAssets/Ui/Inventory_base.png");
	graphics.DrawImage(image, 100, 100, 0, 0, 1038, 548, UnitPixel);
	m_Inven->Render(hDC);
	delete image;
}

void CUiManager::Shop_Ui(HDC hDC)
{
	Image* image(nullptr);
	Graphics graphics(hDC);

	image = Image::FromFile(L"../MoonlighterAssets/Ui/Bag_paperbase.png");
	graphics.DrawImage(image, 100, 100, 0, 0, 385, 435, UnitPixel);

	image = Image::FromFile(L"../MoonlighterAssets/Ui/Showcase_base.png");
	graphics.DrawImage(image, 500, 120, 0, 0, 198, 190, UnitPixel);
	graphics.DrawImage(image, 700, 120, 0, 0, 198, 190, UnitPixel);
	graphics.DrawImage(image, 500, 320, 0, 0, 198, 190, UnitPixel);
	graphics.DrawImage(image, 700, 320, 0, 0, 198, 190, UnitPixel);

	m_InvenShop->Render(hDC);
	delete image;
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