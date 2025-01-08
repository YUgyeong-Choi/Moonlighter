#include "pch.h"
#include "CPotionShop.h"
#include "CUiManager.h"
#include "CKeyManager.h"
#include "CSoundManager.h"
#include "CObjectManager.h"
#include "CPlayer.h"
#include "CBitManager.h"

CPotionShop::CPotionShop():rowIndex(0), columnIndex(0), m_craft(false), up(0)
{
}

void CPotionShop::Initialize()
{
    ADD_BMP(L"../MoonlighterAssets/PotionNpc/craftpotion.bmp", L"CraftPotion");

    m_tFrame.iFrameStart = 0;
    m_tFrame.iFrameEnd = 9;
    m_tFrame.dwSpeed = 100;
    m_tFrame.dwTime = GetTickCount64();


}

int CPotionShop::Update()
{
    if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd) {
        m_craft = false;
        up = 0;
    }

    if (m_craft) {
        up +=0.5f;
    }
    KeyInput();
	return 0;
}

void CPotionShop::Late_Update()
{
    if (m_craft) {
        if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount64())
        {
            ++m_tFrame.iFrameStart;

            if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
                m_tFrame.iFrameStart = 0;

            m_tFrame.dwTime = GetTickCount64();
        }
    }

}

void CPotionShop::Render(HDC hDC)
{
    Image* image(nullptr);
    Graphics graphics(hDC);
    image = Image::FromFile(L"../MoonlighterAssets/PotionNpc/selector.png");
    graphics.DrawImage(image, 416 + (columnIndex * 120), 177, 0, 0, 76, 76, UnitPixel);

    image = Image::FromFile(L"../MoonlighterAssets/PotionNpc/Potion1.png");
    graphics.DrawImage(image, 416 + 15 , 187, 0, 0, 50, 50, UnitPixel);
    image = Image::FromFile(L"../MoonlighterAssets/PotionNpc/Potion2.png");
    graphics.DrawImage(image, 536 + 15 , 187, 0, 0, 50, 50, UnitPixel);
    image = Image::FromFile(L"../MoonlighterAssets/PotionNpc/Potion3.png");
    graphics.DrawImage(image, 656 + 15, 187 , 0, 0, 50, 50, UnitPixel);

    SetTextColor(hDC, RGB(0, 0, 0));
    SetBkMode(hDC, TRANSPARENT);

    HFONT hFont2 = CreateFont(
        25, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"m3x6"
    );

    HFONT OldFont2 = (HFONT)SelectObject(hDC, hFont2);

    TCHAR szTitle[64];
    _stprintf_s(szTitle, _T("Hp 15"));
    RECT rect1 = { 416-15 , 260, 416+100, 280 };
    DrawText(hDC, szTitle, _tcslen(szTitle), &rect1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

    _stprintf_s(szTitle, _T("Hp 30"));
    RECT rect2 = { 536 - 15 , 260, 536 + 100, 280 };
    DrawText(hDC, szTitle, _tcslen(szTitle), &rect2, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

    _stprintf_s(szTitle, _T("Hp 45"));
    RECT rect3 = { 656 - 15 , 260, 656 + 100, 280 };
    DrawText(hDC, szTitle, _tcslen(szTitle), &rect3, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

    SelectObject(hDC, OldFont2);
    DeleteObject(hFont2);

    if(m_craft){
        HDC hMemDC = CBitManager::GetInstance()->FindImage(L"CraftPotion");
        GdiTransparentBlt(hDC, 570 - 100, 320, 200, 260, hMemDC, m_tFrame.iFrameStart * 200, 0, 200, 260, RGB(255, 255, 255));

        image = Image::FromFile(L"../MoonlighterAssets/PotionNpc/Potion2.png");
        graphics.DrawImage(image, 540, (int)(440 - up), 0, 0, 50, 50, UnitPixel);
    }


    delete image;
}

void CPotionShop::Release()
{
}

void CPotionShop::KeyInput()
{
    if (CUiManager::GetInstance()->Get_UiType() == UI_POTIONSHOP) {
        if (CKeyManager::Get_Instance()->Key_Down(KEY_SHOP, 'A')) {
            columnIndex--;
            if (columnIndex < 0) {
                columnIndex = 2; 
            }
            CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
            CSoundManager::Get_Instance()->PlaySound(L"gui_selector_movement.wav", SOUND_EFFECT, g_fEffectVolume, true);
        }

        if (CKeyManager::Get_Instance()->Key_Down(KEY_SHOP, 'D')) {
            columnIndex++;
            if (columnIndex > 2) {
                columnIndex = 0;
            }
            CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
            CSoundManager::Get_Instance()->PlaySound(L"gui_selector_movement.wav", SOUND_EFFECT, g_fEffectVolume, true);
        }

        if (CKeyManager::Get_Instance()->Key_Down(KEY_SHOP, 'K')) {
            if (!m_craft) {
                switch (columnIndex)
                {
                case 0:
                    break;
                case 1:
                    if (static_cast<CPlayer*>(CObjectManager::Get_Instance()->Get_Player())->Get_Money() >= 100) {
                        CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
                        CSoundManager::Get_Instance()->PlaySound(L"shop_item_sold.wav", SOUND_EFFECT, g_fEffectVolume, true);
                        static_cast<CPlayer*>(CObjectManager::Get_Instance()->Get_Player())->Set_Money(100);
                        CUiManager::GetInstance()->AddItem(POTION);
                        m_craft = true;
                    }
                    break;
                case 2:
                    break;
                default:
                    break;
                }
            }

        }

    }
}
