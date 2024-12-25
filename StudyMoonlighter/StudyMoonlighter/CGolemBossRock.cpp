#include "pch.h"
#include "CGolemBossRock.h"
#include "CScrollManager.h"
#include "CBitManager.h"

CGolemBossRock::CGolemBossRock():m_iRockY(0), m_bArrive(false), m_bIsDead(false)
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

    m_tFrame.iFrameStart = 0;
    m_tFrame.iFrameEnd = 7;
    m_tFrame.dwSpeed = 100;
    m_tFrame.dwTime = GetTickCount64();

    m_tRenderSizeX = 64;
    m_tRenderSizeY = 64;

    m_iHp = 20;
}

int CGolemBossRock::Update()
{
    if (m_iHp <= 0) {
        m_bIsDead = true;
    }

    if (m_tRemoveTime + 10000 < GetTickCount64()) {
        m_bIsDead = true;
    }
    
    if (m_bIsDead && (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)) {
        return OBJ_DEAD;
    }

    if (m_tRemoveTime + 1000 < GetTickCount64()) {
        if ((int)m_tInfo.fY > m_iRockY) {
              m_iRockY += 10;
        }
        if ((int)m_tInfo.fY == m_iRockY) {
            m_bArrive = true;
            m_eRender = RENDER_GAMEOBJECT;
            m_tInfo.fCX = 30.f;
            m_tInfo.fCY = 30.f;
        }
    }
    __super::Update_Rect();
    Hit();
    return 0;
}

void CGolemBossRock::Late_Update()
{
    if (m_bIsDead) {
        __super::Move_Frame();
    }
}

void CGolemBossRock::Render(HDC hDC)
{
    if (!m_bArrive) {
        HDC hMemDC = CBitManager::GetInstance()->FindImage(L"GolemBossShadow");
        int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
        int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

        GdiTransparentBlt(hDC, (int)m_tInfo.fX - 32 + iScrollX, (int)m_tInfo.fY - 32 + iScrollY, (int)m_tRenderSizeX, (int)m_tRenderSizeY, hMemDC, 0, 0, 64, 64, RGB(255, 255, 255));
        if (g_bDevmode) {
            Renderbox(hDC, m_tInfo.fX - 32, m_tInfo.fY - 32, m_tInfo.fX + 32, m_tInfo.fY + 32, iScrollX, iScrollY);
        }
    }

    if (m_tRemoveTime + 1000 < GetTickCount64() && !m_bIsDead) {
        HDC hMemDC = CBitManager::GetInstance()->FindImage(L"GolemBossRock1");
        int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
        int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

        GdiTransparentBlt(hDC, (int)m_tInfo.fX - 64 + iScrollX, m_iRockY -64+ iScrollY, (int)128, (int)128, hMemDC, 0, 0, 128, 128, RGB(255, 255, 255));
        if (g_bDevmode) {
            Renderbox(hDC, m_tInfo.fX-64, m_iRockY-64, m_tInfo.fX + 64, m_iRockY + 64, iScrollX, iScrollY);
            Hitbox(hDC, m_tInfo.fX- 10, m_iRockY-10, m_tInfo.fX + 10, m_iRockY + 10, iScrollX, iScrollY);
        }
    }

    if (m_bIsDead) {
        HDC hMemDC = CBitManager::GetInstance()->FindImage(L"GolemBossRockDeath"); 
        int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
        int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

        GdiTransparentBlt(hDC, (int)m_tInfo.fX - 40 + iScrollX, m_iRockY - 40 + iScrollY, (int)80, (int)80, hMemDC, m_tFrame.iFrameStart * 80, 0, 80, 80, RGB(255, 255, 255));
    }
}

void CGolemBossRock::Release()
{
}

void CGolemBossRock::OnCollision(CObject* _obj)
{
    if (_obj->Get_OBJID() == OBJ_PLAYER) {
        if (m_bCanHit) {
            if (m_fAttacktedTime + 500 < GetTickCount64()) {
                m_iAttackedDamage = _obj->Get_AttackDamage();
                m_bCanHit = false;
                m_fAttacktedTime = GetTickCount64();
            }
        }
    }
}
