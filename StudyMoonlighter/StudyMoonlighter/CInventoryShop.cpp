#include "pch.h"
#include "CInventoryShop.h"
#include "CUiManager.h"
#include "CKeyManager.h"

CInventoryShop::CInventoryShop() :rowIndex(0), columnIndex(0)
{
	_pickUpItem.itemId = ITEM_END;
}

void CInventoryShop::Initialize()
{
}

int CInventoryShop::Update()
{
	KeyInput();
	return 0;
}

void CInventoryShop::Late_Update()
{
}

void CInventoryShop::Render(HDC hDC)
{
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (inventory[i][j] != nullptr) {
                inventory[i][j]->Render(hDC);
            }
        }
    }
    Image* image(nullptr);
    Graphics graphics(hDC);
    image = Image::FromFile(L"../MoonlighterAssets/Ui/selector.png");
    if (rowIndex == 0) {
        graphics.DrawImage(image, 173 + (columnIndex * 57), 162, 0, 0, 64, 64, UnitPixel);
    }
    else {
        graphics.DrawImage(image, 173 + (columnIndex * 57), 232 + ((rowIndex - 1) * 57), 0, 0, 64, 64, UnitPixel);
    }

    if (_pickUpItem.itemId != ITEM_END) {
        image = Image::FromFile(L"../MoonlighterAssets/Ui/selector_circle.png");
        if (rowIndex == 1 && columnIndex == 5) {
            graphics.DrawImage(image, 524, 229 - 65, 0, 0, 64, 64, UnitPixel);
        }
        else if (rowIndex == 2 && columnIndex == 5) {
            graphics.DrawImage(image, 524, 288 - 65, 0, 0, 64, 64, UnitPixel);
        }
        else if (rowIndex == 3 && columnIndex == 5) {
            graphics.DrawImage(image, 524, 345 - 65, 0, 0, 64, 64, UnitPixel);
        }
        else if (rowIndex == 0 && columnIndex == 5) {
            graphics.DrawImage(image, 591, 158 - 65, 0, 0, 64, 64, UnitPixel);
        }
        else if (rowIndex == 0 && columnIndex == 6) {
            graphics.DrawImage(image, 731, 158 - 65, 0, 0, 64, 64, UnitPixel);
        }
        else if (rowIndex == 2 && columnIndex == 6) {
            graphics.DrawImage(image, 581, 288 - 65, 0, 0, 64, 64, UnitPixel);
        }
        else {
            if (rowIndex == 0) {
                graphics.DrawImage(image, 168 + (columnIndex * 57), 162 - 65, 0, 0, 64, 64, UnitPixel);
            }
            else {
                graphics.DrawImage(image, 168 + (columnIndex * 57), 232 + ((rowIndex - 1) * 57) - 65, 0, 0, 64, 64, UnitPixel);
            }
        }

        image = Image::FromFile(_pickUpItem.pImageUrl);
        if (rowIndex == 1 && columnIndex == 5) {
            graphics.DrawImage(image, 529, 229 - 60, 0, 0, 48, 48, UnitPixel);
        }
        else if (rowIndex == 2 && columnIndex == 5) {
            graphics.DrawImage(image, 529, 288 - 60, 0, 0, 48, 48, UnitPixel);
        }
        else if (rowIndex == 3 && columnIndex == 5) {
            graphics.DrawImage(image, 529, 345 - 60, 0, 0, 48, 48, UnitPixel);
        }
        else if (rowIndex == 0 && columnIndex == 5) {
            graphics.DrawImage(image, 596, 158 - 60, 0, 0, 48, 48, UnitPixel);
        }
        else if (rowIndex == 0 && columnIndex == 6) {
            graphics.DrawImage(image, 736, 158 - 60, 0, 0, 48, 48, UnitPixel);
        }
        else if (rowIndex == 2 && columnIndex == 6) {
            graphics.DrawImage(image, 586, 288 - 60, 0, 0, 48, 48, UnitPixel);
        }
        else {
            if (rowIndex == 0) {
                graphics.DrawImage(image, 173 + (columnIndex * 57), 162 - 60, 0, 0, 48, 48, UnitPixel);
            }
            else {
                graphics.DrawImage(image, 173 + (columnIndex * 57), 232 + ((rowIndex - 1) * 57) - 60, 0, 0, 48, 48, UnitPixel);
            }
        }

        HFONT hFont1 = CreateFont(
            25, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"m3x6"
        );
        SetTextColor(hDC, RGB(0, 0, 0));
        SetBkMode(hDC, TRANSPARENT);

        HFONT OldFont = (HFONT)SelectObject(hDC, hFont1);

        TCHAR szItemNum[64];
        _stprintf_s(szItemNum, _T("%d"), _pickUpItem.num);

        if (rowIndex == 0) {
            TextOut(hDC, 213 + (columnIndex * 57), 162 - 35, szItemNum, _tcslen(szItemNum));
        }
        else {
            TextOut(hDC, 213 + (columnIndex * 57), 229 - 35 + ((rowIndex - 1) * 56), szItemNum, _tcslen(szItemNum));
        }

        SelectObject(hDC, OldFont);
        DeleteObject(hFont1);
    }

    if (inventory[rowIndex][columnIndex]->Get_Item().itemId != ITEM_END) {
        image = Image::FromFile(L"../MoonlighterAssets/Ui/GUI_ItemNameLabel.png");
        graphics.DrawImage(image, WINCX / 2 - 155, 550, 0, 0, 310, 34, UnitPixel);
        RECT rect = { WINCX / 2 - 155 , 550,  WINCX / 2 - 155 + 310, 584 };

        TCHAR szItemName[64];
        _stprintf_s(szItemName, _T("%s"), inventory[rowIndex][columnIndex]->Get_Item().itemName);
        DrawText(hDC, szItemName, _tcslen(szItemName), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

    delete image;
}

void CInventoryShop::Release()
{
}

void CInventoryShop::KeyInput()
{
    if (CUiManager::GetInstance()->Get_UiType() == UI_INVENSHOP) {
        if (CKeyManager::Get_Instance()->Key_Down(KEY_INVEN, 'W')) {
            rowIndex--;

            if (rowIndex < 0) {
                rowIndex = 3;
            }
        }

        if (CKeyManager::Get_Instance()->Key_Down(KEY_INVEN, 'S')) {
            rowIndex++;

            if (rowIndex > 3) {
                rowIndex = 0;
            }
        }

        if (CKeyManager::Get_Instance()->Key_Down(KEY_INVEN, 'A')) {
            columnIndex--;
            if (columnIndex < 0) {
                columnIndex = 4;
            }
        }

        if (CKeyManager::Get_Instance()->Key_Down(KEY_INVEN, 'D')) {
            columnIndex++;
            if (columnIndex > 4) {
                columnIndex = 0;
            }
        }
    }
}

void CInventoryShop::Copy_Inven(vector<vector<CInvenSlot*>>& _inventory)
{
    inventory = _inventory;
}
