#include "pch.h"
#include "InventorySlot.h"

InventorySlot::InventorySlot(): rowIndex(0), columnIndex(0)
{
}

InventorySlot::InventorySlot(int _x, int _y): rowIndex(_x), columnIndex(_y)
{
	ZeroMemory(&item, sizeof(ITEM));
	item.itemId = ITEM_END;
}

void InventorySlot::Initialize()
{
}

int InventorySlot::Update()
{
	return 0;
}

void InventorySlot::Late_Update()
{
}

void InventorySlot::Render(HDC hDC)
{
	Image* image(nullptr);
	Graphics graphics(hDC);
	image = Image::FromFile(L"../MoonlighterAssets/Ui/Bag_slot_Outline.png");
	if (rowIndex == 0) {
		graphics.DrawImage(image, 141 + (columnIndex * 71), 126 + (rowIndex * 72), 0, 0, 68, 68, UnitPixel);
	}
	else {
		graphics.DrawImage(image, 141 + (columnIndex * 71), 140 + (rowIndex * 72), 0, 0, 68, 68, UnitPixel);
	}
	
	if (item.itemId != ITEM_END) {
		image = Image::FromFile(item.pImageUrl);
		if (rowIndex == 0) {
			graphics.DrawImage(image, 145 + (columnIndex * 71), 130 + (rowIndex * 72), 0, 0, 60, 60, UnitPixel);
		}
		else {
			graphics.DrawImage(image, 145 + (columnIndex * 71), 144 + (rowIndex * 72), 0, 0, 60, 60, UnitPixel);
		}
	}

	if (item.num != 0) {
		TCHAR szItemNum[64];
		_stprintf_s(szItemNum, _T("%d"), item.num);
		SetTextColor(hDC, RGB(0, 0, 0));
		SetBkMode(hDC, TRANSPARENT);
		if (rowIndex == 0) {
			TextOut(hDC, 193 + (columnIndex * 71), 173 + (rowIndex * 72), szItemNum, _tcslen(szItemNum));
		}
		else {
			TextOut(hDC, 193 + (columnIndex * 71), 186 + (rowIndex * 72), szItemNum, _tcslen(szItemNum));
		}
	}
	delete image;
}

void InventorySlot::Release()
{
}

void InventorySlot::Item_Init()
{
	ZeroMemory(&item, sizeof(ITEM));
	item.itemId = ITEM_END;
}

void InventorySlot::Set_ItemType(ITEMTYPE _item)
{
	switch (_item)
	{
	case BROKENSWORD:
		item.itemId = BROKENSWORD;
		item.itemName = "BrokenSword";
		item.maxNum = 5;
		item.num = 1;
		item.pImageUrl = L"../MoonlighterAssets/Items/0_BrokenSword.png";
		break;
	case FABRIC:
		break;
	case HARDENEDSTEEL:
		item.itemId = HARDENEDSTEEL;
		item.itemName = "HardenedSteel";
		item.maxNum = 5;
		item.num = 1;
		item.pImageUrl = L"../MoonlighterAssets/Items/2_HardenedSteel.png";
		break;
	case IRONBAR:
		item.itemId = IRONBAR;
		item.itemName = "IronBar";
		item.maxNum = 10;
		item.num = 1;
		item.pImageUrl = L"../MoonlighterAssets/Items/3_IronBar.png";
		break;
	case RICHJELLY:
		item.itemId = RICHJELLY;
		item.itemName = "RichJelly";
		item.maxNum = 10;
		item.num = 1;
		item.pImageUrl = L"../MoonlighterAssets/Items/4_RichJelly.png";
		break;
	case TEETHSTONE:
		item.itemId = TEETHSTONE;
		item.itemName = "TeethStone";
		item.maxNum = 10;
		item.num = 1;
		item.pImageUrl = L"../MoonlighterAssets/Items/5_TeethStone.png";
		break;
	case WATERSPHERE:
		item.itemId = WATERSPHERE;
		item.itemName = "WaterSphere";
		item.maxNum = 10;
		item.num = 1;
		item.pImageUrl = L"../MoonlighterAssets/Items/6_WaterSphere.png";
		break;
	case WHETSTONE:
		item.itemId = WHETSTONE;
		item.itemName = "Whetstone";
		item.maxNum = 10;
		item.num = 1;
		item.pImageUrl = L"../MoonlighterAssets/Items/7_Whetstone.png";
		break;
	case ITEM_END:
		break;
	default:
		break;
	}
}
