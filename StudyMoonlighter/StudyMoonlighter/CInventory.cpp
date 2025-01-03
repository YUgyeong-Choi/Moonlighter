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
    image = Image::FromFile(L"../MoonlighterAssets/Ui/selector.png");
    Graphics graphics(hDC);
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
                inventory[rowIndex][columnIndex]->Item_Init();
            }
            else {
                if (inventory[rowIndex][columnIndex]->Get_Item().itemId == ITEM_END) {
                    inventory[rowIndex][columnIndex]->Set_Item(_pickUpItem);
                    _pickUpItem.itemId = ITEM_END;
                }
                else {
                    ITEM temp;
                    temp = _pickUpItem;
                    _pickUpItem = inventory[rowIndex][columnIndex]->Get_Item();
                    inventory[rowIndex][columnIndex]->Set_Item(temp);
                }
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
                    inventory[i][j]->Set_ItemNum();
                    return true;
                }
            }
        }
    }
    return false;
}