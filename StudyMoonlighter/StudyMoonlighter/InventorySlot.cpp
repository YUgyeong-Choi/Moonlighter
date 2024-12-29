#include "pch.h"
#include "InventorySlot.h"

InventorySlot::InventorySlot(): rowIndex(0), columnIndex(0)
{
}

InventorySlot::InventorySlot(int _x, int _y): rowIndex(_x), columnIndex(_y)
{
	ZeroMemory(&item, sizeof(ITEM));
	item.itemId = 9999;
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
	
	if (item.itemId != 9999) {
		//아이템 이미지 출력
	}

	delete image;
}

void InventorySlot::Release()
{
}
