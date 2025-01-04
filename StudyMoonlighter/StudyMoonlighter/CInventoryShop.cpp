#include "pch.h"
#include "CInventoryShop.h"
#include "CUiManager.h"
#include "CKeyManager.h"
#include "CPriceSlot.h"

CInventoryShop::CInventoryShop() :rowIndex(0), columnIndex(0), m_IsInven(true)
{
	_pickUpItem.itemId = ITEM_END;
}

void CInventoryShop::Initialize()
{
    sellSlots.resize(4);
    for (int i = 0; i < 4; ++i) {
        sellSlots[i].resize(2);
        for (int j = 0; j < 2; ++j) {
            sellSlots[i][j] = nullptr;
        }
    }
    sellSlots[0][0] = new CInvenSlot();
    sellSlots[0][1] = new CInvenSlot();
    sellSlots[2][0] = new CInvenSlot();
    sellSlots[2][1] = new CInvenSlot();
    sellSlots[1][0] = new CPriceSlot();
    sellSlots[1][1] = new CPriceSlot();
    sellSlots[3][0] = new CPriceSlot();
    sellSlots[3][1] = new CPriceSlot();
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            if (i == 0 || i == 2)
            {
                sellSlots[i][j] = new CInvenSlot();
            }
            else if (i == 1 || i == 3)
            {
                sellSlots[i][j] = new CPriceSlot();
            }
        }
    }
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

    if (m_IsInven) {
        image = Image::FromFile(L"../MoonlighterAssets/Ui/selector.png");

        if (rowIndex == 0) {
            graphics.DrawImage(image, 173 + (columnIndex * 57), 162, 0, 0, 64, 64, UnitPixel);
        }
        else {
            graphics.DrawImage(image, 173 + (columnIndex * 57), 232 + ((rowIndex - 1) * 57), 0, 0, 64, 64, UnitPixel);
        }

        if (_pickUpItem.itemId != ITEM_END) {
            image = Image::FromFile(L"../MoonlighterAssets/Ui/selector_circle.png");
            if (rowIndex == 0) {
                graphics.DrawImage(image, 168 + (columnIndex * 57), 162 - 65, 0, 0, 64, 64, UnitPixel);
            }
            else {
                graphics.DrawImage(image, 168 + (columnIndex * 57), 232 + ((rowIndex - 1) * 57) - 65, 0, 0, 64, 64, UnitPixel);
            }

            image = Image::FromFile(_pickUpItem.pImageUrl);
            if (rowIndex == 0) {
                graphics.DrawImage(image, 173 + (columnIndex * 57), 162 - 60, 0, 0, 48, 48, UnitPixel);
            }
            else {
                graphics.DrawImage(image, 173 + (columnIndex * 57), 232 + ((rowIndex - 1) * 57) - 60, 0, 0, 48, 48, UnitPixel);
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
    }
    else {

        if (rowIndex == 0 || rowIndex == 2) {
            image = Image::FromFile(L"../MoonlighterAssets/Ui/selector.png");
            graphics.DrawImage(image, 573 + (columnIndex * 200), 136 + (rowIndex * 100), 0, 0, 64, 64, UnitPixel);
        }
        else {
            image = Image::FromFile(L"../MoonlighterAssets/Ui/selector_price.png");
            graphics.DrawImage(image, 514 + (columnIndex * 200), 100 + (rowIndex * 100), 0, 0, 170, 80, UnitPixel);
        }

        //if (_pickUpItem.itemId != ITEM_END) {
        //    image = Image::FromFile(L"../MoonlighterAssets/Ui/selector_circle.png");
        //    if (rowIndex == 0) {
        //        graphics.DrawImage(image, 168 + (columnIndex * 57), 162 - 65, 0, 0, 64, 64, UnitPixel);
        //    }
        //    else {
        //        graphics.DrawImage(image, 168 + (columnIndex * 57), 232 + ((rowIndex - 1) * 57) - 65, 0, 0, 64, 64, UnitPixel);
        //    }

        //    image = Image::FromFile(_pickUpItem.pImageUrl);
        //    if (rowIndex == 0) {
        //        graphics.DrawImage(image, 173 + (columnIndex * 57), 162 - 60, 0, 0, 48, 48, UnitPixel);
        //    }
        //    else {
        //        graphics.DrawImage(image, 173 + (columnIndex * 57), 232 + ((rowIndex - 1) * 57) - 60, 0, 0, 48, 48, UnitPixel);
        //    }

        //    HFONT hFont1 = CreateFont(
        //        25, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        //        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        //        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"m3x6"
        //    );
        //    SetTextColor(hDC, RGB(0, 0, 0));
        //    SetBkMode(hDC, TRANSPARENT);

        //    HFONT OldFont = (HFONT)SelectObject(hDC, hFont1);

        //    TCHAR szItemNum[64];
        //    _stprintf_s(szItemNum, _T("%d"), _pickUpItem.num);

        //    if (rowIndex == 0) {
        //        TextOut(hDC, 213 + (columnIndex * 57), 162 - 35, szItemNum, _tcslen(szItemNum));
        //    }
        //    else {
        //        TextOut(hDC, 213 + (columnIndex * 57), 229 - 35 + ((rowIndex - 1) * 56), szItemNum, _tcslen(szItemNum));
        //    }

        //    SelectObject(hDC, OldFont);
        //    DeleteObject(hFont1);
        //}

        //if (inventory[rowIndex][columnIndex]->Get_Item().itemId != ITEM_END) {
        //    image = Image::FromFile(L"../MoonlighterAssets/Ui/GUI_ItemNameLabel.png");
        //    graphics.DrawImage(image, WINCX / 2 - 155, 550, 0, 0, 310, 34, UnitPixel);
        //    RECT rect = { WINCX / 2 - 155 , 550,  WINCX / 2 - 155 + 310, 584 };

        //    TCHAR szItemName[64];
        //    _stprintf_s(szItemName, _T("%s"), inventory[rowIndex][columnIndex]->Get_Item().itemName);
        //    DrawText(hDC, szItemName, _tcslen(szItemName), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        //}
    }


    delete image;
}

void CInventoryShop::Release()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            Safe_Delete<CInvenSlot*>(sellSlots[i][j]);
        }
    }
}

void CInventoryShop::KeyInput()
{
    if (CUiManager::GetInstance()->Get_UiType() == UI_INVENSHOP) {
        if (m_IsInven) {
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
                    m_IsInven = false;
                    columnIndex = 1;
                    if (dynamic_cast<CPriceSlot*>(sellSlots[rowIndex][columnIndex])) {
                        if (sellSlots[rowIndex - 1][columnIndex]->Get_Item().itemId == ITEM_END) {
                            rowIndex--;
                        }
                    }
                }
            }

            if (CKeyManager::Get_Instance()->Key_Down(KEY_INVEN, 'D')) {
                columnIndex++;
                if (columnIndex > 4) {
                    m_IsInven = false;
                    columnIndex = 0;
                    if (dynamic_cast<CPriceSlot*>(sellSlots[rowIndex][columnIndex])) {
                        if (sellSlots[rowIndex - 1][columnIndex]->Get_Item().itemId == ITEM_END) {
                            rowIndex--;
                        }
                    }
                }
            }
        }
        else {
            if (CKeyManager::Get_Instance()->Key_Down(KEY_INVEN, 'W')) {
                rowIndex--;

                if (rowIndex < 0) {
                    rowIndex = 3;
                }

                if (dynamic_cast<CPriceSlot*>(sellSlots[rowIndex][columnIndex])) {
                    if (sellSlots[rowIndex - 1][columnIndex]->Get_Item().itemId == ITEM_END) {
                        rowIndex--;
                    }
                }
            }

            if (CKeyManager::Get_Instance()->Key_Down(KEY_INVEN, 'S')) {
                rowIndex++;
                if (_pickUpItem.itemId != ITEM_END) {
                    rowIndex++;
                }

                if (rowIndex > 3) {
                    rowIndex = 0;
                }

                if (dynamic_cast<CPriceSlot*>(sellSlots[rowIndex][columnIndex])) {
                    if (rowIndex == 3) {
                        if (sellSlots[0][columnIndex]->Get_Item().itemId == ITEM_END) {
                            rowIndex = 0;
                        }
                    }
                    else {
                        if (sellSlots[rowIndex + 1][columnIndex]->Get_Item().itemId == ITEM_END) {
                            rowIndex++;
                        }
                    }

                }
            }

            if (CKeyManager::Get_Instance()->Key_Down(KEY_INVEN, 'A')) {
                columnIndex--;
                if (columnIndex < 0) {
                    m_IsInven = true;
                    columnIndex = 4;
                }
            }

            if (CKeyManager::Get_Instance()->Key_Down(KEY_INVEN, 'D')) {
                columnIndex++;
                if (columnIndex > 1) {
                    m_IsInven = true;
                    columnIndex = 0;
                }
            }
        }

    }
}

void CInventoryShop::Copy_Inven(vector<vector<CInvenSlot*>>& _inventory)
{
    inventory = _inventory;
}
