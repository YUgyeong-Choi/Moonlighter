#include "pch.h"
#include "CGolemBossRock.h"
#include "CScrollManager.h"
#include "CBitManager.h"
#include "CObjectManager.h"
#include "CAbstractFactory.h"
#include "CGolemBossRockDead.h"
#include "CSoundManager.h"

CGolemBossRock::CGolemBossRock():m_iRockY(0), m_bArrive(false), m_bIsDead(false), m_iRandom(0)
{
}

void CGolemBossRock::Initialize()
{
    m_tInfo.fCX = 0.f;
    m_tInfo.fCY = 0.f;
    m_tRemoveTime = GetTickCount64();

    m_eOBJID = OBJ_MAPOBJ;
    m_eRender = RENDER_GAMEOBJECT;
    m_iRockY = m_tInfo.fY;
    m_tInfo.fY = m_tInfo.fY - 300;

    m_tRenderSizeX = 128;
    m_tRenderSizeY = 128;

    m_iHp = 20;
    m_iRandom = rand()%6;
}

int CGolemBossRock::Update()
{
    if (m_iHp <= 0) {
        m_bIsDead = true;
    }

    if (m_tRemoveTime + 10000 < GetTickCount64()) {
        m_bIsDead = true;
    }
    
    if (m_bIsDead) {
        CObjectManager::Get_Instance()->Add_Object(OBJ_NOCOL, CAbstractFactory<CGolemBossRockDead>::Create(m_tInfo.fX, m_tInfo.fY));
        return OBJ_DEAD;
    }

    if (m_tRemoveTime + 1000 < GetTickCount64()) {
        if ((int)m_tInfo.fY < m_iRockY) {
            m_tInfo.fY += 10;
        }
        if ((int)m_tInfo.fY == m_iRockY) {
            m_bArrive = true;
            m_tInfo.fCX = 40.f;
            m_tInfo.fCY = 40.f;
        }
    }
    __super::Update_Rect();
    Hit();
    return 0;
}

void CGolemBossRock::Late_Update()
{
}

void CGolemBossRock::Render(HDC hDC)
{
    if (!m_bArrive) {
        HDC hMemDC = CBitManager::GetInstance()->FindImage(L"GolemBossShadow");
        int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
        int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

        GdiTransparentBlt(hDC, (int)m_tInfo.fX - 32 + iScrollX, (int)m_iRockY - 32 + iScrollY, (int)64, (int)64, hMemDC, 0, 0, 64, 64, RGB(255, 255, 255));
        if (g_bDevmode) {
            Renderbox(hDC, m_tInfo.fX - 32, m_iRockY - 32, m_tInfo.fX + 32, m_iRockY + 32, iScrollX, iScrollY);
        }
    }

    if (m_tRemoveTime + 1000 < GetTickCount64() && !m_bIsDead) {
        HDC hMemDC = CBitManager::GetInstance()->FindImage(L"GolemBossRock1");
        switch (m_iRandom)
        {
        case 0:
            hMemDC = CBitManager::GetInstance()->FindImage(L"GolemBossRock1");
            break;
        case 1:
            hMemDC = CBitManager::GetInstance()->FindImage(L"GolemBossRock2");
            break;
        case 2:
            hMemDC = CBitManager::GetInstance()->FindImage(L"GolemBossRock3");
            break;
        case 3:
            hMemDC = CBitManager::GetInstance()->FindImage(L"GolemBossRock4");
            break;
        case 4:
            hMemDC = CBitManager::GetInstance()->FindImage(L"GolemBossRock5");
            break;
        case 5:
            hMemDC = CBitManager::GetInstance()->FindImage(L"GolemBossRock6");
            break;
        default:
            break;
        }
        
        int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
        int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

        GdiTransparentBlt(hDC, (int)m_tRenderRect.left + iScrollX, m_tRenderRect.top + iScrollY, (int)m_tRenderSizeX, (int)m_tRenderSizeY, hMemDC, 0, 0, m_tRenderSizeX, m_tRenderSizeY, RGB(255, 255, 255));
        if (g_bDevmode) {
            Renderbox(hDC, m_tRenderRect, iScrollX, iScrollY);
            Hitbox(hDC, m_tRect, iScrollX, iScrollY);
        }
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
