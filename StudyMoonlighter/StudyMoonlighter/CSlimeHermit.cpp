#include "pch.h"
#include "CSlimeHermit.h"
#include "CObjectManager.h"
#include "CScrollManager.h"
#include "CBitManager.h"
#include "CCollisionManager.h"
#include "CSoundManager.h"
#include "CCollisionManager.h"
#include "CItem.h"
#include "CAbstractFactory.h"

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
    m_MonsterType = SLIME_HERMIT;
    m_tInfo.fCX = 50.f;
    m_tInfo.fCY = 50.f;

    m_tFrame.iFrameStart = 0;
    m_tFrame.iFrameEnd = 2;
    m_tFrame.dwSpeed = 200;
    m_tFrame.dwTime = GetTickCount64();

    m_tRenderSizeX = 400.f;
    m_tRenderSizeY = 400.f;
    m_eRender = RENDER_GAMEOBJECT;

    m_targetObj = CObjectManager::Get_Instance()->Get_Player();
    m_checkCircle = { (int)m_tInfo.fX - 100, (int)m_tInfo.fY - 100,(int)m_tInfo.fX + 100, (int)m_tInfo.fY + 100 };

    m_iHp = 100;
    m_iMaxHp = m_iHp;
    m_iAttackDamage = 17;
    InitHitFrame();
}

int CSlimeHermit::Update()
{
    if (m_iHp <= 0) {
        CObjectManager::Get_Instance()->Add_Object(OBJ_NOCOL, CAbstractFactory< CGolemDeadObj>::Create(m_tInfo.fX, m_tInfo.fY, HERMIT));
        CObjectManager::Get_Instance()->Add_Object(OBJ_ITEM, CAbstractFactory<CItem>::Create(m_tInfo.fX, m_tInfo.fY));
        static_cast<CItem*>(CObjectManager::Get_Instance()->Get_LastItem())->Set_ItemType(WHETSTONE);
        CSoundManager::Get_Instance()->StopSound(MONSTER_EFFECT);
        CSoundManager::Get_Instance()->PlaySound(L"enemy_death.wav", MONSTER_EFFECT, g_fMonsterVolume, true);
        return OBJ_DEAD;
    }
    if (m_IsAttack) {
        switch (m_eDir)
        {
        case LEFT:
            if (m_tFrame.iFrameStart == 8) {
                m_HitBox = { (long)m_tInfo.fX - 150,(long)m_tInfo.fY - 25,(long)m_tInfo.fX,(long)m_tInfo.fY + 25 };
            }

            if (m_tFrame.iFrameStart == 23) {
                m_HitBox = { (long)m_tInfo.fX - 100,(long)m_tInfo.fY - 25,(long)m_tInfo.fX,(long)m_tInfo.fY + 25 };
            }

            if (m_tFrame.iFrameStart == 40) {
                m_HitBox = { (long)m_tInfo.fX - 50,(long)m_tInfo.fY - 25,(long)m_tInfo.fX,(long)m_tInfo.fY + 25 };
            }
            break;
        case RIGHT:
            if (m_tFrame.iFrameStart == 8) {
                m_HitBox = { (long)m_tInfo.fX-20,(long)m_tInfo.fY - 25,(long)m_tInfo.fX+130,(long)m_tInfo.fY + 25 };
            }

            if (m_tFrame.iFrameStart == 23) {
                m_HitBox = { (long)m_tInfo.fX-20,(long)m_tInfo.fY - 25,(long)m_tInfo.fX+80,(long)m_tInfo.fY + 25 };
            }

            if (m_tFrame.iFrameStart == 40) {
                m_HitBox = { (long)m_tInfo.fX-20,(long)m_tInfo.fY - 25,(long)m_tInfo.fX+30,(long)m_tInfo.fY + 25 };
            }
            break;
        case UP:
            if (m_tFrame.iFrameStart == 8) {
                m_HitBox = { (long)m_tInfo.fX - 40,(long)m_tInfo.fY - 150,(long)m_tInfo.fX + 10 ,(long)m_tInfo.fY};
            }

            if (m_tFrame.iFrameStart == 23) {
                m_HitBox = { (long)m_tInfo.fX - 40,(long)m_tInfo.fY - 100,(long)m_tInfo.fX + 10 ,(long)m_tInfo.fY };
            }

            if (m_tFrame.iFrameStart == 40) {
                m_HitBox = { (long)m_tInfo.fX - 40,(long)m_tInfo.fY - 50,(long)m_tInfo.fX + 10,(long)m_tInfo.fY };
            }
            break;
        case DOWN:
            if (m_tFrame.iFrameStart == 8) {
                m_HitBox = { (long)m_tInfo.fX - 40,(long)m_tInfo.fY,(long)m_tInfo.fX+10 ,(long)m_tInfo.fY+150 };
            }

            if (m_tFrame.iFrameStart == 23) {
                m_HitBox = { (long)m_tInfo.fX - 40,(long)m_tInfo.fY,(long)m_tInfo.fX + 10 ,(long)m_tInfo.fY + 100 };
            }

            if (m_tFrame.iFrameStart == 40) {
                m_HitBox = { (long)m_tInfo.fX - 40,(long)m_tInfo.fY,(long)m_tInfo.fX + 10,(long)m_tInfo.fY+50};
            }
            break;
        }
        if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd) {
            m_IsAttack = false;
            m_tFrame.iFrameEnd = 2;
            m_tFrame.dwSpeed = 200;
            m_HitBox = { 0,0,0,0 };
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

    __super::Update_Rect();
	return 0;
}

void CSlimeHermit::Late_Update()
{
    OnCollision();
    Hit();
    Check_PlayerIn();
    if (m_bHit) {
        Move_Frame_Hit();
    }
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
        GdiTransparentBlt(hDC, m_tRenderRect.left + iScrollX, m_tRenderRect.top + iScrollY, (int)m_tRenderSizeX, (int)m_tRenderSizeY, hMemDC, m_tFrame.iFrameStart * m_tRenderSizeX, 0, m_tRenderSizeX, m_tRenderSizeY, RGB(255, 255, 255));
        break;
    case RIGHT:
        hMemDC = CBitManager::GetInstance()->FindImage(L"SlimeHermitRight");
        GdiTransparentBlt(hDC, m_tRenderRect.left + iScrollX, m_tRenderRect.top + iScrollY, (int)m_tRenderSizeX, (int)m_tRenderSizeY, hMemDC, m_tFrame.iFrameStart * m_tRenderSizeX, 0, m_tRenderSizeX, m_tRenderSizeY, RGB(255, 255, 255));
        break;
    case UP:
        hMemDC = CBitManager::GetInstance()->FindImage(L"SlimeHermitUp");
        GdiTransparentBlt(hDC, m_tRenderRect.left + iScrollX, m_tRenderRect.top + iScrollY, (int)m_tRenderSizeX, (int)m_tRenderSizeY, hMemDC, m_tFrame.iFrameStart * m_tRenderSizeX, 0, m_tRenderSizeX, m_tRenderSizeY, RGB(255, 255, 255));
        break;
    case DOWN:
        hMemDC = CBitManager::GetInstance()->FindImage(L"SlimeHermitDown");
        GdiTransparentBlt(hDC, m_tRenderRect.left + iScrollX, m_tRenderRect.top + iScrollY, (int)m_tRenderSizeX, (int)m_tRenderSizeY, hMemDC, m_tFrame.iFrameStart * m_tRenderSizeX, 0, m_tRenderSizeX, m_tRenderSizeY, RGB(255, 255, 255));
        break;
    }

    if (!m_bCanHit) {
        RenderHpUi(hDC);
        HitEffect(hDC);
    }

    if (g_bDevmode) {
        Renderbox(hDC, m_tRenderRect, iScrollX, iScrollY);
        Hitbox(hDC, m_tRect, iScrollX, iScrollY);
        Attackbox(hDC, m_HitBox, iScrollX, iScrollY);
        DetectCircle(hDC, m_checkCircle);
    }
}

void CSlimeHermit::Release()
{
}

void CSlimeHermit::OnCollision()
{
    CObject* _copyPlayer = CObjectManager::Get_Instance()->Get_Player();

    if (CCollisionManager::CollisionRectWeapon(_copyPlayer, this)) {
        if (m_bCanHit) {
            if (m_fAttacktedTime + 500 < GetTickCount64()) {
                m_iAttackedDamage = _copyPlayer->Get_AttackDamage();
                m_bCanHit = false;
                m_fAttacktedTime = GetTickCount64();
                CSoundManager::Get_Instance()->StopSound(MONSTER_EFFECT);
                CSoundManager::Get_Instance()->PlaySound(L"bigslime_hit.wav", MONSTER_EFFECT, g_fMonsterVolume + 0.5f, true);
                m_bHit = true;
            }
        }
    }
}

void CSlimeHermit::OnCollision(CObject* _obj)
{
    if (_obj->Get_OBJID() == OBJ_PLAYER_ARROW) {
        if (m_bCanHit) {
            if (m_fAttacktedTime + 500 < GetTickCount64()) {
                m_iAttackedDamage = _obj->Get_AttackDamage();
                m_bCanHit = false;
                CSoundManager::Get_Instance()->StopSound(MONSTER_EFFECT);
                CSoundManager::Get_Instance()->PlaySound(L"bigslime_hit.wav", MONSTER_EFFECT, g_fMonsterVolume + 0.5f, true);
                m_fAttacktedTime = GetTickCount64();
            }
        }
    }
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
            m_tFrame.dwSpeed = 80;
            m_IsAttack = true;
            CSoundManager::Get_Instance()->StopSound(MONSTER_EFFECT);
            CSoundManager::Get_Instance()->PlaySound(L"golem_dungeon_golem_crash.wav", MONSTER_EFFECT, g_fMonsterVolume, true);
        }
    }
}
