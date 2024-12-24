#include "pch.h"
#include "CGolemBossRock.h"
#include "CScrollManager.h"
#include "CBitManager.h"

CGolemBossRock::CGolemBossRock():m_iRockY(0), m_bArrive(false)
{
}

void CGolemBossRock::Initialize()
{
    m_tInfo.fCX = 0.f;
    m_tInfo.fCY = 0.f;
    m_tRemoveTime = GetTickCount64();

    m_eOBJID = OBJ_MAPOBJ;
    m_eRender = RENDER_BACKGROUND;
    m_iRockY = m_tInfo.fY - 300;

    m_tRenderSizeX = 64;
    m_tRenderSizeY = 64;
}

int CGolemBossRock::Update()
{
    if (m_tRemoveTime + 15000 < GetTickCount64()) {
        return OBJ_DEAD;
    }

    if (m_tRemoveTime + 1000 < GetTickCount64()) {
        if ((int)m_tInfo.fY > m_iRockY) {
            m_iRockY += 10;
        }
        if ((int)m_tInfo.fY == m_iRockY) {
            m_bArrive = true;
            m_tInfo.fCX = 20.f;
            m_tInfo.fCY = 20.f;
        }
    }
    return 0;
}

void CGolemBossRock::Late_Update()
{
}

void CGolemBossRock::Render(HDC hDC)
{
    if (!m_bArrive) {
        HDC hMemDC = CBitManager::GetInstance()->FindImage(L"GolemBossRockShadow");
        int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
        int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

        GdiTransparentBlt(hDC, (int)m_tInfo.fX - 32 + iScrollX, (int)m_tInfo.fY - 32 + iScrollY, (int)m_tRenderSizeX, (int)m_tRenderSizeY, hMemDC, 0, 0, 64, 64, RGB(255, 255, 255));
        if (g_bDevmode) {
            Renderbox(hDC, m_tInfo.fX - 32, m_tInfo.fY - 32, m_tInfo.fX + 32, m_tInfo.fY + 32, iScrollX, iScrollY);
        }
    }

    if (m_tRemoveTime + 1000 < GetTickCount64()) {
        HDC hMemDC = CBitManager::GetInstance()->FindImage(L"GolemBossRock1");
        int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
        int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

        GdiTransparentBlt(hDC, (int)m_tInfo.fX - 64 + iScrollX, m_iRockY -64+ iScrollY, (int)128, (int)128, hMemDC, 0, 0, 128, 128, RGB(255, 255, 255));
        if (g_bDevmode) {
            Renderbox(hDC, m_tInfo.fX-64, m_iRockY-64, m_tInfo.fX + 64, m_iRockY + 64, iScrollX, iScrollY);
            Hitbox(hDC, m_tInfo.fX- 10, m_iRockY-10, m_tInfo.fX + 10, m_iRockY + 10, iScrollX, iScrollY);
        }
    }
}

void CGolemBossRock::Release()
{
}

void CGolemBossRock::OnCollision(CObject* _obj)
{
}
