#include "pch.h"
#include "CInventory.h"
#include "CKeyManager.h"
#include "CUiManager.h"
#include "CSpecialSlot.h"
#include "CSoundManager.h"

CInventory::CInventory() :rowIndex(0), columnIndex(0)
{
    _pickUpItem.itemId = ITEM_END;
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

    SetTextColor(hDC, RGB(0, 0, 0));
    if (inventory[rowIndex][columnIndex]->Get_Item().itemId != ITEM_END) {

        image = Image::FromFile(L"../MoonlighterAssets/Ui/GUI_ItemNameLabel.png");
        graphics.DrawImage(image, WINCX/2-155, 550, 0, 0, 310, 34, UnitPixel);
        RECT rect = { WINCX / 2 - 155 , 550,  WINCX / 2 - 155 + 310, 584 };

        TCHAR szItemName[64];
        _stprintf_s(szItemName, _T("%s"), inventory[rowIndex][columnIndex]->Get_Item().itemName);
        DrawText(hDC, szItemName, _tcslen(szItemName), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

    delete image;
}

void CInventory::Release()
{

}

void CInventory::KeyInput()
{
    if (CUiManager::GetInstance()->Get_UiType() == UI_INVEN) {
        if (CKeyManager::Get_Instance()->Key_Down(KEY_INVEN,'W')) {
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
            CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
            CSoundManager::Get_Instance()->PlaySound(L"gui_selector_movement.wav", SOUND_EFFECT, g_fEffectVolume, true);
        }

        if (CKeyManager::Get_Instance()->Key_Down(KEY_INVEN,'S')) {
            rowIndex++;

            if (columnIndex == 6) {
                columnIndex--;
            }
            if (rowIndex > 3) {
                rowIndex = 0;
            }
            CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
            CSoundManager::Get_Instance()->PlaySound(L"gui_selector_movement.wav", SOUND_EFFECT, g_fEffectVolume, true);
        }

        if (CKeyManager::Get_Instance()->Key_Down(KEY_INVEN,'A')) {
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
            CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
            CSoundManager::Get_Instance()->PlaySound(L"gui_selector_movement.wav", SOUND_EFFECT, g_fEffectVolume, true);
        }

        if (CKeyManager::Get_Instance()->Key_Down(KEY_INVEN,'D')) {
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
            CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
            CSoundManager::Get_Instance()->PlaySound(L"gui_selector_movement.wav", SOUND_EFFECT, g_fEffectVolume, true);
        }

        if (CKeyManager::Get_Instance()->Key_Down(KEY_INVEN,'J')) {
            if (_pickUpItem.itemId == ITEM_END) {
                _pickUpItem = inventory[rowIndex][columnIndex]->Get_Item();
                _pickUpItem.num = 1;
                inventory[rowIndex][columnIndex]->Sub_ItemNum();

                if (inventory[rowIndex][columnIndex]->Get_Item().num == 0) {
                    inventory[rowIndex][columnIndex]->Item_Init();
                }
                CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
                CSoundManager::Get_Instance()->PlaySound(L"gui_selector_pick.wav", SOUND_EFFECT, g_fEffectVolume, true);
            }
            else if (_pickUpItem.itemId == inventory[rowIndex][columnIndex]->Get_Item().itemId) {
                if (_pickUpItem.num != _pickUpItem.maxNum) {
                    _pickUpItem.num++;
                    inventory[rowIndex][columnIndex]->Sub_ItemNum();

                    if (inventory[rowIndex][columnIndex]->Get_Item().num == 0) {
                        inventory[rowIndex][columnIndex]->Item_Init();
                    }
                    CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
                    CSoundManager::Get_Instance()->PlaySound(L"gui_selector_pick.wav", SOUND_EFFECT, g_fEffectVolume, true);
                }
            }
            else if (_pickUpItem.itemId != ITEM_END && inventory[rowIndex][columnIndex]->Get_Item().itemId == ITEM_END) {
                inventory[rowIndex][columnIndex]->Set_Item(_pickUpItem);
                _pickUpItem.itemId = ITEM_END;
                CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
                CSoundManager::Get_Instance()->PlaySound(L"gui_selector_drop.wav", SOUND_EFFECT, g_fEffectVolume, true);
            }
            else if (_pickUpItem.itemId != ITEM_END && inventory[rowIndex][columnIndex]->Get_Item().itemId != ITEM_END) {
                ITEM temp= _pickUpItem;
                _pickUpItem = inventory[rowIndex][columnIndex]->Get_Item();
                inventory[rowIndex][columnIndex]->Set_Item(temp);
                CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
                CSoundManager::Get_Instance()->PlaySound(L"gui_selector_pick.wav", SOUND_EFFECT, g_fEffectVolume, true);
            }
        }
    }
}

void CInventory::Copy_Inven(vector<vector<CInvenSlot*>>& _inventory)
{
    inventory = _inventory;
}

