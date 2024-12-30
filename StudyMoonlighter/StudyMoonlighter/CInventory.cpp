#include "pch.h"
#include "CInventory.h"
#include "CKeyManager.h"
#include "CUiManager.h"

CInventory::CInventory():rowIndex(0),columnIndex(0)
{
    ZeroMemory(&_pickUpItem, sizeof(ITEM));
    _pickUpItem.itemId = ITEM_END;

    inventory.resize(4);
    for (int i = 0; i < 4; ++i) {
        inventory[i].resize(5);
        for (int j = 0; j < 5; ++j) {
            inventory[i][j] = new InventorySlot(i, j);
        }
    }
}

void CInventory::Initialize()
{
}

int CInventory::Update()
{
    KeyInput();
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 5; ++j) {
            inventory[i][j]->Update();
        }
    }
    return 0;
}

void CInventory::Late_Update()
{
}

void CInventory::Render(HDC hDC)
{
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 5; ++j) {
            inventory[i][j]->Render(hDC);
        }
    }

    Image* image(nullptr);
    image = Image::FromFile(L"../MoonlighterAssets/Ui/selector.png");
    Graphics graphics(hDC);
    if (rowIndex == 0) {
        graphics.DrawImage(image, 141 + (columnIndex * 71), 126 + (rowIndex * 72), 0, 0, 64, 64, UnitPixel);
    }
    else {
        graphics.DrawImage(image, 141 + (columnIndex * 71), 140 + (rowIndex * 72), 0, 0, 64, 64, UnitPixel);
    }
    delete image;
}

void CInventory::Release()
{
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 5; ++j) {
            Safe_Delete<InventorySlot*>(inventory[i][j]);
        }
    }
}

void CInventory::KeyInput()
{
    if (CUiManager::GetInstance()->Get_UiType() == UI_INVEN) {
        if (CKeyManager::Get_Instance()->Key_Down('W')) {
            rowIndex--;
            if (rowIndex < 0) {
                rowIndex = 0;
            }
        }

        if (CKeyManager::Get_Instance()->Key_Down('S')) {
            rowIndex++;
            if (rowIndex > 3) {
                rowIndex = 3;
            }
        }

        if (CKeyManager::Get_Instance()->Key_Down('A')) {
            columnIndex--;
            if (columnIndex < 0) {
                columnIndex = 0;
            }
        }

        if (CKeyManager::Get_Instance()->Key_Down('D')) {
            columnIndex++;
            if (columnIndex > 4) {
                columnIndex = 4;
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
                    ZeroMemory(&_pickUpItem, sizeof(ITEM));
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
