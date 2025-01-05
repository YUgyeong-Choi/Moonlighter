#include "pch.h"
#include "CInventoryShop.h"
#include "CUiManager.h"
#include "CKeyManager.h"
#include "CPriceSlot.h"
#include "CSellSlot.h"
#include "CObjectManager.h"
#include "CShowCase.h"

CInventoryShop::CInventoryShop() :rowIndex(0), columnIndex(0), m_IsInven(true), m_IsPriceTime(false)
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

    int num = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            if (i == 0 || i == 2)
            {
                sellSlots[i][j] = new CSellSlot(i,j);
            }
            else if (i == 1 || i == 3)
            {
                sellSlots[i][j] = new CPriceSlot(i, j, num);
                num++;
            }
        }
    }

    m_ShowCase.resize(4);
    for (int i = 0; i <4; ++i) {
        m_ShowCase[i]= nullptr;
    }
}

int CInventoryShop::Update()
{
    KeyInput();
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 2; ++j) {
            if (sellSlots[i][j] != nullptr) {
                sellSlots[i][j]->Update();
            }
        }
    }
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

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 2; ++j) {
            if (sellSlots[i][j] != nullptr) {
                sellSlots[i][j]->Render(hDC);
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

        if ((rowIndex == 0 || rowIndex == 2) && _pickUpItem.itemId != ITEM_END) {
            image = Image::FromFile(L"../MoonlighterAssets/Ui/selector_circle.png");
            graphics.DrawImage(image, 567 + (columnIndex * 200), 70 + (rowIndex * 100), 0, 0, 64, 64, UnitPixel);

            image = Image::FromFile(_pickUpItem.pImageUrl);
            graphics.DrawImage(image, 573 + (columnIndex * 200), 75 + (rowIndex * 100), 0, 0, 48, 48, UnitPixel);

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

            TextOut(hDC, 613 + (columnIndex * 200), 90 + (rowIndex * 100), szItemNum, _tcslen(szItemNum));

            SelectObject(hDC, OldFont);
            DeleteObject(hFont1);
        }

        //if (sell[rowIndex][columnIndex]->Get_Item().itemId != ITEM_END) {
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
        if (m_IsInven && !m_IsPriceTime) {
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

            if (CKeyManager::Get_Instance()->Key_Down(KEY_INVEN, 'J')) {
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
                    ITEM temp = _pickUpItem;
                    _pickUpItem = inventory[rowIndex][columnIndex]->Get_Item();
                    inventory[rowIndex][columnIndex]->Set_Item(temp);
                }
            }
        }
        else if(!m_IsInven && !m_IsPriceTime){
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

                if (rowIndex > 3) {
                    rowIndex = 0;
                }

                if (dynamic_cast<CPriceSlot*>(sellSlots[rowIndex][columnIndex])) {
                    if (sellSlots[rowIndex -1][columnIndex]->Get_Item().itemId == ITEM_END) {
                        rowIndex++;
                        if (rowIndex == 4) rowIndex = 0;
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

            if (CKeyManager::Get_Instance()->Key_Down(KEY_INVEN, 'J')) {
                if (dynamic_cast<CSellSlot*>(sellSlots[rowIndex][columnIndex])) {
                    if (_pickUpItem.itemId == ITEM_END) {
                        if (sellSlots[rowIndex][columnIndex]->Get_Item().num == 0) {
                            return;
                        }
                        _pickUpItem = sellSlots[rowIndex][columnIndex]->Get_Item();
                        _pickUpItem.num = 1;
                        sellSlots[rowIndex][columnIndex]->Sub_ItemNum();
                        sellSlots[rowIndex+1][columnIndex]->Sub_ItemNum();

                        if (sellSlots[rowIndex][columnIndex]->Get_Item().num == 0) {
                            sellSlots[rowIndex][columnIndex]->Item_Init();
                            sellSlots[rowIndex + 1][columnIndex]->Item_Init();
                            static_cast<CPriceSlot*>(sellSlots[rowIndex + 1][columnIndex])->Init_Price();
                            if (rowIndex == 0 && columnIndex == 0) {
                                static_cast<CShowCase*>(m_ShowCase[0])->Set_InitItem();
                            }
                            else if (rowIndex == 0 && columnIndex == 1) {
                                static_cast<CShowCase*>(m_ShowCase[1])->Set_InitItem();
                            }
                            else if (rowIndex == 2 && columnIndex == 0) {
                                static_cast<CShowCase*>(m_ShowCase[2])->Set_InitItem();
                            }
                            else if (rowIndex == 2 && columnIndex == 1) {
                                static_cast<CShowCase*>(m_ShowCase[3])->Set_InitItem();
                            }
                        }
                    }
                    else if (_pickUpItem.itemId == sellSlots[rowIndex][columnIndex]->Get_Item().itemId) {
                        if (_pickUpItem.num != _pickUpItem.maxNum) {
                            _pickUpItem.num++;
                            sellSlots[rowIndex][columnIndex]->Sub_ItemNum();
                            sellSlots[rowIndex + 1][columnIndex]->Sub_ItemNum();

                            if (sellSlots[rowIndex][columnIndex]->Get_Item().num == 0) {
                                sellSlots[rowIndex][columnIndex]->Item_Init();
                                sellSlots[rowIndex + 1][columnIndex]->Item_Init();
                                static_cast<CPriceSlot*>(sellSlots[rowIndex + 1][columnIndex])->Init_Price();
                                if (rowIndex == 0 && columnIndex == 0) {
                                    static_cast<CShowCase*>(m_ShowCase[0])->Set_InitItem();
                                }
                                else if (rowIndex == 0 && columnIndex == 1) {
                                    static_cast<CShowCase*>(m_ShowCase[1])->Set_InitItem();
                                }
                                else if (rowIndex == 2 && columnIndex == 0) {
                                    static_cast<CShowCase*>(m_ShowCase[2])->Set_InitItem();
                                }
                                else if (rowIndex == 2 && columnIndex == 1) {
                                    static_cast<CShowCase*>(m_ShowCase[3])->Set_InitItem();
                                }
                            }
                        }
                    }
                    else if (_pickUpItem.itemId != ITEM_END && sellSlots[rowIndex][columnIndex]->Get_Item().itemId == ITEM_END) {
                        sellSlots[rowIndex][columnIndex]->Set_Item(_pickUpItem);
                        sellSlots[rowIndex + 1][columnIndex]->Set_Item(_pickUpItem);
                        static_cast<CPriceSlot*>(sellSlots[rowIndex + 1][columnIndex])->Init_Price();
                        if (rowIndex == 0 && columnIndex == 0) {
                            static_cast<CShowCase*>(m_ShowCase[0])->Set_Item(_pickUpItem);
                        }
                        else if (rowIndex == 0 && columnIndex == 1) {
                            static_cast<CShowCase*>(m_ShowCase[1])->Set_Item(_pickUpItem);
                        }
                        else if (rowIndex == 2 && columnIndex == 0) {
                            static_cast<CShowCase*>(m_ShowCase[2])->Set_Item(_pickUpItem);
                        }
                        else if (rowIndex == 2 && columnIndex == 1) {
                            static_cast<CShowCase*>(m_ShowCase[3])->Set_Item(_pickUpItem);
                        }
                        _pickUpItem.itemId = ITEM_END;
                    }
                    else if (_pickUpItem.itemId != ITEM_END && sellSlots[rowIndex][columnIndex]->Get_Item().itemId != ITEM_END) {
                        ITEM temp = _pickUpItem;
                        _pickUpItem = sellSlots[rowIndex][columnIndex]->Get_Item();
                        sellSlots[rowIndex][columnIndex]->Set_Item(temp);
                        sellSlots[rowIndex + 1][columnIndex]->Set_Item(temp);
                        static_cast<CPriceSlot*>(sellSlots[rowIndex + 1][columnIndex])->Init_Price();
                        if (rowIndex == 0 && columnIndex == 0) {
                            static_cast<CShowCase*>(m_ShowCase[0])->Set_Item(temp);
                        }
                        else if (rowIndex == 0 && columnIndex == 1) {
                            static_cast<CShowCase*>(m_ShowCase[1])->Set_Item(temp);
                        }
                        else if (rowIndex == 2 && columnIndex == 0) {
                            static_cast<CShowCase*>(m_ShowCase[2])->Set_Item(temp);
                        }
                        else if (rowIndex == 2 && columnIndex == 1) {
                            static_cast<CShowCase*>(m_ShowCase[3])->Set_Item(temp);
                        }
                    }
                }
                else {
                    m_IsPriceTime = true;
                    static_cast<CPriceSlot*>(sellSlots[rowIndex][columnIndex])->Init_Index();
                }
            }
        }

    }
}

void CInventoryShop::Copy_Inven(vector<vector<CInvenSlot*>>& _inventory)
{
    inventory = _inventory;
}

void CInventoryShop::Set_Showcase()
{
    list<CObject*> mapObjList = CObjectManager::Get_Instance()->Get_MapObjList();
    int i = 0;
    for (auto& obj : mapObjList) {
        if (dynamic_cast<CShowCase*>(obj)) {
            m_ShowCase[i] = obj;
            i++;
        }
    }
}

void CInventoryShop::Set_InitSlot(int _index)
{
    switch (_index)
    {
    case 0:
        sellSlots[0][0]->Item_Init();
        sellSlots[1][0]->Item_Init();
        static_cast<CPriceSlot*>(sellSlots[1][0])->Init_Price();
        static_cast<CShowCase*>(m_ShowCase[0])->Set_InitItem();
        break;
    case 1:
        sellSlots[0][1]->Item_Init();
        sellSlots[1][1]->Item_Init();
        static_cast<CPriceSlot*>(sellSlots[1][1])->Init_Price();
        static_cast<CShowCase*>(m_ShowCase[1])->Set_InitItem();
        break;
    case 2:
        sellSlots[2][0]->Item_Init();
        sellSlots[3][0]->Item_Init();
        static_cast<CPriceSlot*>(sellSlots[3][0])->Init_Price();
        static_cast<CShowCase*>(m_ShowCase[2])->Set_InitItem();
        break;
    case 3:
        sellSlots[2][1]->Item_Init();
        sellSlots[3][1]->Item_Init();
        static_cast<CPriceSlot*>(sellSlots[3][1])->Init_Price();
        static_cast<CShowCase*>(m_ShowCase[3])->Set_InitItem();
        break;
    default:
        break;
    }
}

vector<ITEM> CInventoryShop::Get_PriceSlotITEM()
{
    vector<ITEM> _vecITem;
    _vecITem.resize(4);
    int num = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            if (i == 1 || i == 3)
            {
                _vecITem[num] = sellSlots[i][j]->Get_Item();
                num++;
            }
        }
    }
    return _vecITem;
}

vector<int> CInventoryShop::Get_PriceSlotPrice()
{
    vector<int> _vecITem;
    _vecITem.resize(4);
    int num = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            if (i == 1 || i == 3)
            {
                _vecITem[num] = static_cast<CPriceSlot*>(sellSlots[i][j])->Get_Price();
                num++;
            }
        }
    }
    return _vecITem;
}
