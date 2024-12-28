#include "pch.h"
#include "CGolemMiniboss.h"
#include "CObjectManager.h"
#include "CScrollManager.h"
#include "CBitManager.h"
#include "CCollisionManager.h"

CGolemMiniboss::CGolemMiniboss()
{

}

void CGolemMiniboss::Initialize()
{
    m_eOBJID = OBJ_MONSTER;

    m_tInfo.fCX = 0.f;
    m_tInfo.fCY = 0.f;
    m_fSpeed = 0.5f;

    m_tFrame.iFrameStart = 0;
    m_tFrame.iFrameEnd = 6;
    m_tFrame.dwSpeed = 200;
    m_tFrame.dwTime = GetTickCount64();

    m_tRenderSizeX = 300.f;
    m_tRenderSizeY = 300.f;
    m_eRender = RENDER_GAMEOBJECT;

    m_targetObj = CObjectManager::Get_Instance()->Get_Player();
}

int CGolemMiniboss::Update()
{
    float x = m_targetObj->Get_Info().fX - m_tInfo.fX;
    float y = m_targetObj->Get_Info().fY - m_tInfo.fY;

    if (fabsf(x) < fabsf(y)) {
        if (y < 0) {
            m_eDir = UP;


        }
        else {
            m_eDir = DOWN;

        }
    }
    else {
        if (x < 0) {
            m_eDir = LEFT;
        }
        else {
            m_eDir = RIGHT;

        }
    }
    __super::Update_Rect();
	return 0;
}

void CGolemMiniboss::Late_Update()
{
    __super::Move_Frame();
}

void CGolemMiniboss::Render(HDC hDC)
{
    Image* image(nullptr);
    switch (m_eDir)
    {
    case LEFT:
        image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/miniboss/miniboss_idle_left.png");
        break;
    case RIGHT:
        image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/miniboss/miniboss_idle_right.png");
        break;
    case UP:
        image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/miniboss/miniboss_idle_up.png");
        break;
    case DOWN:
        image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/miniboss/miniboss_idle_down.png");
        break;
    }
    Graphics graphics(hDC);

    int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
    int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

    graphics.DrawImage(image, (int)m_tRenderRect.left + iScrollX, (int)m_tRenderRect.top + iScrollY, (int)m_tRenderSizeX * m_tFrame.iFrameStart, 0, (int)m_tRenderSizeX, (int)m_tRenderSizeY, UnitPixel);


    if (g_bDevmode) {
        Renderbox(hDC, m_tInfo.fX - 150, m_tInfo.fY - 150, m_tInfo.fX + 150, m_tInfo.fY + 150, iScrollX, iScrollY);
        //Renderbox(hDC, m_HitBox, iScrollX, iScrollY);
    }
    delete image;
}

void CGolemMiniboss::Release()
{
}
