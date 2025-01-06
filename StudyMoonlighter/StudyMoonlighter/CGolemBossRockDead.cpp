#include "pch.h"
#include "CGolemBossRockDead.h"
#include "CBitManager.h"
#include "CScrollManager.h"

CGolemBossRockDead::CGolemBossRockDead()
{
}

void CGolemBossRockDead::Initialize()
{
    m_eOBJID = OBJ_NOCOL;
    m_eRender = RENDER_BACKGROUND;
    m_tFrame.iFrameStart = 0;
    m_tFrame.iFrameEnd = 7;
    m_tFrame.dwSpeed = 100;
    m_tFrame.dwTime = GetTickCount64();
    m_tRenderSizeX = 80;
    m_tRenderSizeY = 80;
}

int CGolemBossRockDead::Update()
{
    if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd) {
        return OBJ_DEAD;
    }
    __super::Update_Rect();
    return 0;
}

void CGolemBossRockDead::Late_Update()
{
    __super::Move_Frame();
}

void CGolemBossRockDead::Render(HDC hDC)
{
    HDC hMemDC = CBitManager::GetInstance()->FindImage(L"GolemBossRockDeath");
    int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
    int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

    GdiTransparentBlt(hDC, m_tRenderRect.left+ iScrollX, m_tRenderRect.top + iScrollY, (int)m_tRenderSizeX, (int)m_tRenderSizeY, hMemDC, m_tFrame.iFrameStart * m_tRenderSizeX, 0, m_tRenderSizeX, m_tRenderSizeY, RGB(255, 255, 255));

    if (g_bDevmode) {
        Renderbox(hDC, m_tRenderRect, iScrollX, iScrollY);
    }
}

void CGolemBossRockDead::Release()
{
}
