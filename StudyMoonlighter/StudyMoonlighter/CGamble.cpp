#include "pch.h"
#include "CGamble.h"
#include "CScrollManager.h"
#include "CKeyManager.h"
#include "CObjectManager.h"
#include "CUiManager.h"
#include "CSoundManager.h"
#include "CShopPlayer.h"

CGamble::CGamble():m_bActive(false), m_ShowUi(false)
{
}

void CGamble::Initialize()
{
    m_eOBJID = OBJ_MAPOBJ;
    m_eRender = RENDER_GAMEOBJECT;
    m_tInfo.fCX = 50;
    m_tInfo.fCY = 80;
    m_tRenderSizeX = 80;
    m_tRenderSizeY = 113;
    m_HitBox = { (int)m_tInfo.fX - 20, (int)m_tInfo.fY + 30, (int)m_tInfo.fX + 20, (int)m_tInfo.fY + 60 };
}

int CGamble::Update()
{
    Key_Input();
    __super::Update_Rect();
    return 0;
}

void CGamble::Late_Update()
{
    m_bActive = false;
    OnCollision();
}

void CGamble::Render(HDC hDC)
{
    int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
    int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();
    Graphics graphics(hDC);
    Image* image(nullptr);
    image = Image::FromFile(L"../MoonlighterAssets/Gamble/slotmachine.png");

    graphics.DrawImage(image, (int)m_tRenderRect.left + (int)iScrollX, m_tRenderRect.top + iScrollY, 0, 0, (int)m_tRenderSizeX, (int)m_tRenderSizeY, UnitPixel);

    delete image;

    if (m_bActive) {
        COLORREF color = RGB(234, 221, 187);

        HBRUSH hBrush = CreateSolidBrush(color);
        HPEN hPen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
        HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
        RoundRect(hDC, m_tInfo.fX + iScrollX + 20 - 80, m_tInfo.fY - 70 + iScrollY, m_tInfo.fX + 180 + iScrollX - 80, m_tInfo.fY - 20 + iScrollY, 30, 30);

        SelectObject(hDC, hOldBrush);
        SelectObject(hDC, hOldPen);
        DeleteObject(hBrush);
        DeleteObject(hPen);

        Image* image(nullptr);
        Graphics graphics(hDC);
        image = Image::FromFile(L"../MoonlighterAssets/Ui/button_J.png");
        graphics.DrawImage(image, (int)m_tInfo.fX + iScrollX + 15 - 80, (int)m_tInfo.fY - 75 + iScrollY, 0, 0, 64, 64, UnitPixel);

        SetTextColor(hDC, RGB(0, 0, 0));
        TCHAR szEnter[64];
        _stprintf_s(szEnter, _T("도박 (올인)"));
        TextOut(hDC, (int)m_tInfo.fX + iScrollX + 80 - 80, (int)m_tInfo.fY - 55 + iScrollY, szEnter, _tcslen(szEnter));
    }

    if (g_bDevmode) {
        Hitbox(hDC, m_tRect, iScrollX, iScrollY);
        Renderbox(hDC, m_tRenderRect, iScrollX, iScrollY);
        DetectRect(hDC, m_HitBox, iScrollX, iScrollY);
    }
}

void CGamble::Release()
{
}

void CGamble::OnCollision()
{
    RECT rc;
    CObject* _player = CObjectManager::Get_Instance()->Get_Player();
    if (IntersectRect(&rc, _player->Get_Rect(), &m_HitBox)) {
        m_bActive = true;
    }
}

void CGamble::Key_Input()
{
    if (CKeyManager::Get_Instance()->Key_Down(KEY_GAMBLE, 'J') && m_bActive) {
        m_ShowUi = !m_ShowUi;
        if (m_ShowUi) {
            CSoundManager::Get_Instance()->StopSound(SOUND_BGM);
            CUiManager::GetInstance()->Set_UiType(UI_GAMBLE);
            CUiManager::GetInstance()->Get_Gamble()->Set_GambleStart(true);
            CUiManager::GetInstance()->Get_Gamble()->InitGame();
            static_cast<CShopPlayer*>(CObjectManager::Get_Instance()->Get_Player())->Set_Inven(true);

        }
        else {
            CUiManager::GetInstance()->Set_UiType(UI_END);
            CSoundManager::Get_Instance()->StopAll();
            CSoundManager::Get_Instance()->PlayBGM(L"shop_day_open.wav", g_fBackgroundVolume, true);
            CUiManager::GetInstance()->Get_Gamble()->Set_GambleStart(false);
            static_cast<CShopPlayer*>(CObjectManager::Get_Instance()->Get_Player())->Set_Inven(false);
        }
    }
}
