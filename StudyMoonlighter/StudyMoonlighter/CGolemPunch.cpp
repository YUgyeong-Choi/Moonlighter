#include "pch.h"
#include "CGolemPunch.h"
#include "CObjectManager.h"
#include "CBitManager.h"
#include "CScrollManager.h"
#include "CSoundManager.h"

CGolemPunch::CGolemPunch():m_iSize(0), m_bCanDown(false), m_PunchY(0), m_OriginPunchY(0), m_bCanUp(false), m_iPunchTime(3),m_sound(false), m_sound2(false)
{
}

void CGolemPunch::Initialize()
{
    m_eOBJID = OBJ_MONSTER;

    m_tInfo.fCX = 0.f;
    m_tInfo.fCY = 0.f;

    m_tFrame.iFrameStart = 0;
    m_tFrame.iFrameEnd = 8;
    m_tFrame.dwSpeed = 280;
    m_tFrame.dwTime = GetTickCount64();

    m_tRenderSizeX = 10.f;
    m_tRenderSizeY = 10.f;
    m_eRender = RENDER_GAMEOBJECT;

    m_iPunchTime = 3;
    m_iAttackDamage = 20;

    m_targetObj = CObjectManager::Get_Instance()->Get_Player(); 
}

int CGolemPunch::Update()
{
    if (m_iPunchTime < 0) {
        return OBJ_DEAD;
    }

    if (m_bCanUp) {
        if (m_iSize >= 20) {
            m_PunchY = (int)m_OriginPunchY + m_iSize;
            m_iSize -= 20;
            if (m_iPunchTime != 0) {
                if (!m_sound2) {
                    CSoundManager::Get_Instance()->StopSound(MONSTER_EFFECT);
                    CSoundManager::Get_Instance()->PlaySound(L"golem_dungeon_king_golem_handcrash_prepare.wav", MONSTER_EFFECT, g_fMonsterVolume, true);
                    m_sound2 = true;
                }
            }

        }
        else {
            m_bCanUp = false;
            m_bCanDown = false;
            m_sound = false;
            m_sound2 = false;
            m_tRenderSizeX = 10.f;
            m_tRenderSizeY = 10.f;
            m_iPunchTime--;
        }
    }

    if (m_bCanDown && !m_bCanUp) {
        if (m_iSize < 400) {
            m_PunchY = (int)m_OriginPunchY + m_iSize;
            m_iSize += 20;

            if (m_iPunchTime != 0 ) {
                if (!m_sound) {
                    CSoundManager::Get_Instance()->StopSound(MONSTER_EFFECT);
                    CSoundManager::Get_Instance()->PlaySound(L"golem_dungeon_king_golem_handcrash.wav", MONSTER_EFFECT, g_fMonsterVolume+0.2f, true);
                    m_sound = true;
                }
            }

        }
    }

    if (m_tRenderSizeX < 64) {
        m_iSize++;
        m_tInfo.fX = m_targetObj->Get_Info().fX;
        m_tInfo.fY = m_targetObj->Get_Info().fY + 64 - m_iSize;
        m_tRenderSizeX++;
        m_tRenderSizeY++;
    }
    else {
        if (!m_bCanDown && !m_bCanUp) {
            m_bCanDown = true;
            m_tRenderSizeX = 256.f;
            m_tRenderSizeY = 256.f;
            m_OriginPunchY = m_targetObj->Get_Info().fY - 400.f;
            m_iSize = 10;
        }
    }

    if (m_iPunchTime != 0) {
        if (2 <= m_tFrame.iFrameStart && m_tFrame.iFrameStart <= 3) {
            m_HitBox = { (int)m_tInfo.fX - 25 , (int)m_PunchY + 10 , (int)m_tInfo.fX + 25 , (int)m_PunchY + 60 };
        }
        else {
            m_HitBox = { 0,0,0,0 };
        }
    }

    if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd) {
        m_bCanUp = true;
    }

    return 0;
}

void CGolemPunch::Late_Update()
{
    if (m_bCanDown) {
        __super::Move_Frame();
    }

}

void CGolemPunch::Render(HDC hDC)
{
    int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
    int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

    if (m_iPunchTime != 0) {
        if (!m_bCanDown) {
            HDC hMemDC = CBitManager::GetInstance()->FindImage(L"GolemBossShadow");
            GdiTransparentBlt(hDC, (int)m_tInfo.fX - 32 + iScrollX, (int)m_tInfo.fY - 32 + iScrollY, (int)m_tRenderSizeX, (int)m_tRenderSizeY, hMemDC, 0, 0, 64, 64, RGB(255, 255, 255));
        }
        else {
            HDC hMemDC = CBitManager::GetInstance()->FindImage(L"GolemBossPunchDown");
            GdiTransparentBlt(hDC, (int)m_tInfo.fX - 128 + iScrollX, (int)m_PunchY - 128 + iScrollY, (int)m_tRenderSizeX, (int)m_tRenderSizeY, hMemDC, m_tFrame.iFrameStart * m_tRenderSizeX, 0, 256, 256, RGB(255, 255, 255));
        }
    }


    if (g_bDevmode) {
        //Renderbox(hDC, m_tInfo.fX - 32, m_tInfo.fY - 32, m_tInfo.fX + 32, m_tInfo.fY + 32, iScrollX, iScrollY);
        Hitbox(hDC, m_HitBox, iScrollX, iScrollY);
    }
}

void CGolemPunch::Release()
{
}

void CGolemPunch::OnCollision(CObject* _obj)
{
}
