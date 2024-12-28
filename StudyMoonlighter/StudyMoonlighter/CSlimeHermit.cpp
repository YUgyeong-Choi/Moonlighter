#include "pch.h"
#include "CSlimeHermit.h"
#include "CObjectManager.h"
#include "CScrollManager.h"
#include "CBitManager.h"
#include "CCollisionManager.h"
#include "CSoundManager.h"

CSlimeHermit::CSlimeHermit():m_IsAttack(false)
{
    ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/slimehermit/slimehermit_down.bmp", L"SlimeHermitDown");
    ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/slimehermit/slimehermit_left.bmp", L"SlimeHermitLeft");
    ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/slimehermit/slimehermit_right.bmp", L"SlimeHermitRight");
    ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/slimehermit/slimehermit_up.bmp", L"SlimeHermitUp");
}

void CSlimeHermit::Initialize()
{
    m_eOBJID = OBJ_MONSTER;

    m_tInfo.fCX = 0.f;
    m_tInfo.fCY = 0.f;

    m_tFrame.iFrameStart = 0;
    m_tFrame.iFrameEnd = 2;
    m_tFrame.dwSpeed = 200;
    m_tFrame.dwTime = GetTickCount64();

    m_tRenderSizeX = 400.f;
    m_tRenderSizeY = 400.f;
    m_eRender = RENDER_GAMEOBJECT;

    m_targetObj = CObjectManager::Get_Instance()->Get_Player();
    m_checkCircle = { (int)m_tInfo.fX - 100, (int)m_tInfo.fY - 100,(int)m_tInfo.fX + 100, (int)m_tInfo.fY + 100 };
}

int CSlimeHermit::Update()
{
    if (m_IsAttack) {
        if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd) {
            m_IsAttack = false;
            m_tFrame.iFrameEnd = 2;
            m_tFrame.dwSpeed = 200;
        }
    }

    if (!m_IsAttack) {
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
    }

	return 0;
}

void CSlimeHermit::Late_Update()
{
    OnCollision();
    Hit();
    Check_PlayerIn();
    __super::Move_Frame();
}

void CSlimeHermit::Render(HDC hDC)
{
    int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
    int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();
    HDC hMemDC = CBitManager::GetInstance()->FindImage(L"SlimeHermitLeft");
    switch (m_eDir)
    {
    case LEFT:
        hMemDC = CBitManager::GetInstance()->FindImage(L"SlimeHermitLeft");
        GdiTransparentBlt(hDC, (int)m_tInfo.fX - 200 + iScrollX, (int)m_tInfo.fY - 200 + iScrollY, (int)m_tRenderSizeX, (int)m_tRenderSizeY, hMemDC, m_tFrame.iFrameStart * m_tRenderSizeX, 0, m_tRenderSizeX, m_tRenderSizeY, RGB(255, 255, 255));
        break;
    case RIGHT:
        hMemDC = CBitManager::GetInstance()->FindImage(L"SlimeHermitRight");
        GdiTransparentBlt(hDC, (int)m_tInfo.fX - 200 + iScrollX, (int)m_tInfo.fY - 200 + iScrollY, (int)m_tRenderSizeX, (int)m_tRenderSizeY, hMemDC, m_tFrame.iFrameStart * m_tRenderSizeX, 0, m_tRenderSizeX, m_tRenderSizeY, RGB(255, 255, 255));
        break;
    case UP:
        hMemDC = CBitManager::GetInstance()->FindImage(L"SlimeHermitUp");
        GdiTransparentBlt(hDC, (int)m_tInfo.fX - 200 + iScrollX, (int)m_tInfo.fY - 200 + iScrollY, (int)m_tRenderSizeX, (int)m_tRenderSizeY, hMemDC, m_tFrame.iFrameStart * m_tRenderSizeX, 0, m_tRenderSizeX, m_tRenderSizeY, RGB(255, 255, 255));
        break;
    case DOWN:
        hMemDC = CBitManager::GetInstance()->FindImage(L"SlimeHermitDown");
        GdiTransparentBlt(hDC, (int)m_tInfo.fX - 200 + iScrollX, (int)m_tInfo.fY - 200 + iScrollY, (int)m_tRenderSizeX, (int)m_tRenderSizeY, hMemDC, m_tFrame.iFrameStart * m_tRenderSizeX, 0, m_tRenderSizeX, m_tRenderSizeY, RGB(255, 255, 255));
        break;
    }

    if (g_bDevmode) {
        Renderbox(hDC, m_tInfo.fX - 200, m_tInfo.fY - 200, m_tInfo.fX + 200, m_tInfo.fY + 200, iScrollX, iScrollY);
        Renderbox(hDC, m_HitBox, iScrollX, iScrollY);
        HitCircle(hDC, m_checkCircle);
    }
}

void CSlimeHermit::Release()
{
}

void CSlimeHermit::OnCollision()
{
}

void CSlimeHermit::Check_PlayerIn()
{
    CObject* _copyPlayer = CObjectManager::Get_Instance()->Get_Player();
    float fRadius = (200 + _copyPlayer->Get_Info().fCX) * 0.5f;

    float fWidth = abs(m_tInfo.fX - _copyPlayer->Get_Info().fX);
    float fHeight = abs(m_tInfo.fY - _copyPlayer->Get_Info().fY);

    float fDiagonal = powf((fWidth * fWidth + fHeight * fHeight), 0.5f);

    if (fRadius >= fDiagonal) {
        if (!m_IsAttack) {
            m_tFrame.iFrameEnd = 51;
            m_tFrame.dwSpeed = 100;
            m_IsAttack = true;
            CSoundManager::Get_Instance()->StopSound(MONSTER_EFFECT);
            CSoundManager::Get_Instance()->PlaySound(L"golem_dungeon_golem_crash.wav", MONSTER_EFFECT, g_fMonsterVolume, true);
        }
    }
}
