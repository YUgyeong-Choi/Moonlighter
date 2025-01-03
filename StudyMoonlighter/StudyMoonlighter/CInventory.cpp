#include "pch.h"
#include "CInventory.h"
#include "CKeyManager.h"
#include "CUiManager.h"
#include "CSpecialSlot.h"

CInventory::CInventory() :rowIndex(0), columnIndex(0)
{
    _pickUpItem.itemId = ITEM_END;

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
    
}

void CInventory::Initialize()
{
}

int CInventory::Update()
{
    KeyInput();
    return 0;
}

void CInventory::Late_Update()
{
}

void CInventory::Render(HDC hDC)
{
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 7; ++j) {
            if (inventory[i][j] != nullptr) {
                inventory[i][j]->Render(hDC);
            }
        }
    }

    Image* image(nullptr);
    Graphics graphics(hDC);
    image = Image::FromFile(L"../MoonlighterAssets/Ui/selector.png");
    if (rowIndex == 1 && columnIndex == 5) {
        graphics.DrawImage(image, 529 , 229, 0, 0, 64, 64, UnitPixel);
    }else if (rowIndex == 2 && columnIndex == 5) {
        graphics.DrawImage(image, 529, 288, 0, 0, 64, 64, UnitPixel);
    }else if (rowIndex == 3 && columnIndex == 5) {
        graphics.DrawImage(image, 529, 345, 0, 0, 64, 64, UnitPixel);
    }else if (rowIndex == 0 && columnIndex == 5) {
        graphics.DrawImage(image, 596 , 158, 0, 0, 64, 64, UnitPixel);
    }else if (rowIndex == 0 && columnIndex == 6) {
        graphics.DrawImage(image, 736 , 158, 0, 0, 64, 64, UnitPixel);
    }else if (rowIndex == 2 && columnIndex == 6) {
        graphics.DrawImage(image, 586 , 288, 0, 0, 64, 64, UnitPixel);
    }
    else {
        if (rowIndex == 0) {
            graphics.DrawImage(image, 173 + (columnIndex * 57), 162, 0, 0, 64, 64, UnitPixel);
        }
        else {
            graphics.DrawImage(image, 173 + (columnIndex * 57), 232 + ((rowIndex - 1) * 57), 0, 0, 64, 64, UnitPixel);
        }
    }

    if (_pickUpItem.itemId != ITEM_END) {
        image = Image::FromFile(L"../MoonlighterAssets/Ui/selector_circle.png");
        if (rowIndex == 1 && columnIndex == 5) {
            graphics.DrawImage(image, 524, 229- 65, 0, 0, 64, 64, UnitPixel);
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

        if (rowIndex == 1 && columnIndex == 5) {
            TextOut(hDC, 569 , 229 - 35, szItemNum, _tcslen(szItemNum));
        }
        else if (rowIndex == 2 && columnIndex == 5) {
            TextOut(hDC, 569 , 288 - 35, szItemNum, _tcslen(szItemNum));
        }
        else if (rowIndex == 3 && columnIndex == 5) {
            TextOut(hDC, 569 , 345 - 35, szItemNum, _tcslen(szItemNum));
        }
        else if (rowIndex == 0 && columnIndex == 5) {
            TextOut(hDC, 636 , 158 - 35, szItemNum, _tcslen(szItemNum));
        }
        else if (rowIndex == 0 && columnIndex == 6) {
            TextOut(hDC, 776 , 158 - 35, szItemNum, _tcslen(szItemNum));
        }
        else if (rowIndex == 2 && columnIndex == 6) {
            TextOut(hDC, 626 , 288 - 35, szItemNum, _tcslen(szItemNum));
        }
        else {
            if (rowIndex == 0) {
                TextOut(hDC, 213 + (columnIndex * 57), 162-35, szItemNum, _tcslen(szItemNum));
            }
            else {
                TextOut(hDC, 213 + (columnIndex * 57), 229 - 35 + ((rowIndex - 1) * 56), szItemNum, _tcslen(szItemNum));
            }
        }

        SelectObject(hDC, OldFont);
        DeleteObject(hFont1);
    }

    delete image;
}

void CInventory::Release()
{
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 7; ++j) {
            Safe_Delete<CInvenSlot*>(inventory[i][j]);
        }
    }
}

void CInventory::KeyInput()
{
    if (CUiManager::GetInstance()->Get_UiType() == UI_INVEN) {
        if (CKeyManager::Get_Instance()->Key_Down('W')) {
            rowIndex--;
            if (rowIndex == 1 && columnIndex == 6) {
                columnIndex--;;
            }

            if (rowIndex == -1 && columnIndex == 6) {
                columnIndex=5;
                rowIndex = 3;
            }
            if (rowIndex < 0) {
                rowIndex = 3;
            }
        }

        if (CKeyManager::Get_Instance()->Key_Down('S')) {
            rowIndex++;

            if (columnIndex == 6) {
                columnIndex--;
            }
            if (rowIndex > 3) {
                rowIndex = 0;
            }
        }

        if (CKeyManager::Get_Instance()->Key_Down('A')) {
            columnIndex--;
            if (rowIndex == 1 && columnIndex == -1) {
                columnIndex = 5;
            }
            if (rowIndex == 3 && columnIndex == -1) {
                columnIndex = 5;
            }
            if (columnIndex < 0) {
                columnIndex = 6;
            }
        }

        if (CKeyManager::Get_Instance()->Key_Down('D')) {
            columnIndex++;
            if (rowIndex == 3 && columnIndex == 6) {
                columnIndex = 0;
            }

            if (rowIndex == 1 && columnIndex == 6) {
                columnIndex = 0;
            }
            if (columnIndex > 6) {
                columnIndex = 0;
            }
        }

        if (CKeyManager::Get_Instance()->Key_Down('J')) {
            if (_pickUpItem.itemId == ITEM_END) {
                _pickUpItem = inventory[rowIndex][columnIndex]->Get_Item();
                _pickUpItem.num = 1;
                inventory[rowIndex][columnIndex]->Sub_ItemNum();

                if (inventory[rowIndex][columnIndex]->Get_Item().num == 0) {
                    inventory[rowIndex][columnIndex]->Item_Init();
                }
            }
            else if (_pickUpItem.itemId == inventory[rowIndex][columnIndex]->Get_Item().itemId) {
                if (_pickUpItem.num != _pickUpItem.maxNum) {
                    _pickUpItem.num++;
                    inventory[rowIndex][columnIndex]->Sub_ItemNum();

                    if (inventory[rowIndex][columnIndex]->Get_Item().num == 0) {
                        inventory[rowIndex][columnIndex]->Item_Init();
                    }
                }
            }
            else if (_pickUpItem.itemId != ITEM_END && inventory[rowIndex][columnIndex]->Get_Item().itemId == ITEM_END) {
                inventory[rowIndex][columnIndex]->Set_Item(_pickUpItem);
                _pickUpItem.itemId = ITEM_END;
            }
            else if (_pickUpItem.itemId != ITEM_END && inventory[rowIndex][columnIndex]->Get_Item().itemId != ITEM_END) {
                ITEM temp= _pickUpItem;
                _pickUpItem = inventory[rowIndex][columnIndex]->Get_Item();
                inventory[rowIndex][columnIndex]->Set_Item(temp);
            }
        }
    }
}

void CInventory::AddItem(ITEMTYPE _item)
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

bool CInventory::FindItem(ITEMTYPE _item)
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