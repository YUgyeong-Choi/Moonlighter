#include "pch.h"
#include "CInvenSlot.h"

CInvenSlot::CInvenSlot():rowIndex(0), columnIndex(0)
{
	item.itemId = ITEM_END;
	item.num = 0;
	m_OutfitType = OUTFIT_ITEM;
}

CInvenSlot::CInvenSlot(int _row, int _column)
{
	rowIndex = _row, columnIndex = _column;
	item.itemId = ITEM_END;
	item.num = 0;
}

void CInvenSlot::Initialize()
{
}

int CInvenSlot::Update()
{
	return 0;
}

void CInvenSlot::Late_Update()
{
}

void CInvenSlot::Render(HDC hDC)
{
	Image* image(nullptr);
	Graphics graphics(hDC);
	image = Image::FromFile(L"../MoonlighterAssets/Ui/Bag_slot_Outline.png");
	if (rowIndex == 0) {
		graphics.DrawImage(image, 172 + (columnIndex * 57), 160, 0, 0, 54, 54, UnitPixel);
	}
	else {
		graphics.DrawImage(image, 172 + (columnIndex * 57), 230 + ((rowIndex-1) * 57), 0, 0, 54, 54, UnitPixel);
	}

	if (item.itemId != ITEM_END) {
		image = Image::FromFile(item.pImageUrl);
		if (rowIndex == 0) {
			graphics.DrawImage(image, 172 + (columnIndex * 57), 160 , 0, 0, 48, 48, UnitPixel);
		}
		else {
			graphics.DrawImage(image, 172 + (columnIndex * 57), 230 + ((rowIndex-1) * 57), 0, 0, 48, 48, UnitPixel);
		}

		TCHAR szItemNum[64];
		_stprintf_s(szItemNum, _T("%d"), item.num);
		SetTextColor(hDC, RGB(0, 0, 0));
		SetBkMode(hDC, TRANSPARENT);

		HFONT hFont1 = CreateFont(
			25, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"m3x6"
		);


		HFONT OldFont = (HFONT)SelectObject(hDC, hFont1);


		if (rowIndex == 0) {
			TextOut(hDC, 210 + (columnIndex * 57), 187, szItemNum, _tcslen(szItemNum));
		}
		else {
			TextOut(hDC, 210 + (columnIndex * 57), 255 + ((rowIndex-1) * 57), szItemNum, _tcslen(szItemNum));
		}
		SelectObject(hDC, OldFont);
		DeleteObject(hFont1);
	}


	delete image;
}

void CInvenSlot::Release()
{
}

void CInvenSlot::Item_Init()
{
	item.itemId = ITEM_END;
}

void CInvenSlot::Set_ItemType(ITEMTYPE _item)
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
