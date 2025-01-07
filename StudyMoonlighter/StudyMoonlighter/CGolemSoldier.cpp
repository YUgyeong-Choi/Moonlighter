#include "pch.h"
#include "CGolemSoldier.h"
#include "CObjectManager.h"
#include "CScrollManager.h"
#include "CBitManager.h"
#include "CCollisionManager.h"
#include "CSoundManager.h"
#include "CCollisionManager.h"
#include "CItem.h"
#include "CAbstractFactory.h"

CGolemSoldier::CGolemSoldier()
{

}

void CGolemSoldier::Initialize()
{
    m_eOBJID = OBJ_MONSTER;
    m_MonsterType = GOLEM_SOLIDER;
    m_tInfo.fCX = 30.f;
    m_tInfo.fCY = 50.f;
    m_fSpeed = 0.5f;

    m_tFrame.iFrameStart = 0;
    m_tFrame.iFrameEnd = 2;
    m_tFrame.dwSpeed = 200;
    m_tFrame.dwTime = GetTickCount64();

    m_tRenderSizeX = 200.f;
    m_tRenderSizeY = 200.f;
    m_eRender = RENDER_GAMEOBJECT;

    m_targetObj = CObjectManager::Get_Instance()->Get_Player();

    m_iHp = 100;
    m_iMaxHp = m_iHp;
    m_iAttackDamage = 17;
    InitHitFrame();
}

int CGolemSoldier::Update()
{
    if (m_iHp <= 0) {
        CObjectManager::Get_Instance()->Add_Object(OBJ_ITEM, CAbstractFactory<CItem>::Create(m_tInfo.fX, m_tInfo.fY));
        static_cast<CItem*>(CObjectManager::Get_Instance()->Get_LastItem())->Set_ItemType(BROKENSWORD);
        return OBJ_DEAD;
    }
    if (m_IsAttack) {
        switch (m_eDir)
        {
        case LEFT:
            if (m_tFrame.iFrameStart == 7) {
                m_HitBox = { (long)m_tInfo.fX - 80,(long)m_tInfo.fY - 25,(long)m_tInfo.fX,(long)m_tInfo.fY + 25 };
            }
            break;
        case RIGHT:
            if (m_tFrame.iFrameStart == 7) {
                m_HitBox = { (long)m_tInfo.fX - 20,(long)m_tInfo.fY - 25,(long)m_tInfo.fX + 80,(long)m_tInfo.fY + 25 };
            }
            break;
        case UP:
            if (m_tFrame.iFrameStart == 7) {
                m_HitBox = { (long)m_tInfo.fX - 25,(long)m_tInfo.fY - 80,(long)m_tInfo.fX + 25 ,(long)m_tInfo.fY };
            }
            break;
        case DOWN:
            if (m_tFrame.iFrameStart == 7) {
                m_HitBox = { (long)m_tInfo.fX - 25,(long)m_tInfo.fY,(long)m_tInfo.fX + 25 ,(long)m_tInfo.fY + 80 };
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
        float   fWidth(0.f), fHeight(0.f), fDiagonal(0.f), fRadian(0.f), m_fAngle(0.f);

        fWidth = m_targetObj->Get_Info().fX - m_tInfo.fX;
        fHeight = m_targetObj->Get_Info().fY - m_tInfo.fY;

        fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

        fRadian = acosf(fWidth / fDiagonal);

        m_fAngle = fRadian * (180.f / PI);

        if (m_targetObj->Get_Info().fY > m_tInfo.fY)
            m_fAngle *= -1.f;

        // degree to radian
        m_tInfo.fX += m_fSpeed * cosf(m_fAngle * (PI / 180.f));
        m_tInfo.fY -= m_fSpeed * sinf(m_fAngle * (PI / 180.f));


        float x = m_targetObj->Get_Info().fX - m_tInfo.fX;
        float y = m_targetObj->Get_Info().fY - m_tInfo.fY;

        m_checkCircle = { (int)m_tInfo.fX - 80, (int)m_tInfo.fY - 80,(int)m_tInfo.fX + 80, (int)m_tInfo.fY + 80 };

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

void CGolemSoldier::Late_Update()
{
    OnCollision();
    Hit();
    Check_PlayerIn();
    if (m_bHit) {
        Move_Frame_Hit();
    }
    __super::Move_Frame();
}

void CGolemSoldier::Render(HDC hDC)
{
    Image* image(nullptr);
    switch (m_eDir)
    {
    case LEFT:
        image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/golem_soldier/golem_soldier_left.png");
        break;
    case RIGHT:
        image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/golem_soldier/golem_soldier_right.png");
        break;
    case UP:
        image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/golem_soldier/golem_soldier_up.png");
        break;
    case DOWN:
        image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/golem_soldier/golem_soldier_down.png");
        break;
    }
    Graphics graphics(hDC);

    int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
    int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

    graphics.DrawImage(image, (int)m_tRenderRect.left + iScrollX, (int)m_tRenderRect.top + iScrollY, (int)m_tRenderSizeX * m_tFrame.iFrameStart, 0, (int)m_tRenderSizeX, (int)m_tRenderSizeY, UnitPixel);

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

void CGolemSoldier::Release()
{
}

void CGolemSoldier::OnCollision()
{
    CObject* _copyPlayer = CObjectManager::Get_Instance()->Get_Player();

    if (CCollisionManager::CollisionRectWeapon(_copyPlayer, this)) {
        if (m_bCanHit) {
            if (m_fAttacktedTime + 500 < GetTickCount64()) {
                m_iAttackedDamage = _copyPlayer->Get_AttackDamage();
                m_bCanHit = false;
                m_fAttacktedTime = GetTickCount64();
                CSoundManager::Get_Instance()->StopSound(MONSTER_EFFECT);
                CSoundManager::Get_Instance()->PlaySound(L"golem_dungeon_golem_hit.wav", MONSTER_EFFECT, g_fMonsterVolume + 0.5f, true);
                m_bHit = true;
            }
        }
    }
}

void CGolemSoldier::OnCollision(CObject* _obj)
{
    if (_obj->Get_OBJID() == OBJ_PLAYER_ARROW) {
        if (m_bCanHit) {
            if (m_fAttacktedTime + 500 < GetTickCount64()) {
                m_iAttackedDamage = _obj->Get_AttackDamage();
                m_bCanHit = false;
                m_fAttacktedTime = GetTickCount64();
            }
        }
    }
}

void CGolemSoldier::Check_PlayerIn()
{
    CObject* _copyPlayer = CObjectManager::Get_Instance()->Get_Player();
    float fRadius = (160 + _copyPlayer->Get_Info().fCX) * 0.5f;

    float fWidth = abs(m_tInfo.fX - _copyPlayer->Get_Info().fX);
    float fHeight = abs(m_tInfo.fY - _copyPlayer->Get_Info().fY);

    float fDiagonal = powf((fWidth * fWidth + fHeight * fHeight), 0.5f);

    if (fRadius >= fDiagonal) {
        if (!m_IsAttack) {
            m_tFrame.iFrameEnd = 12;
            m_tFrame.dwSpeed = 100;
            m_IsAttack = true;
            CSoundManager::Get_Instance()->StopSound(MONSTER_EFFECT);
            CSoundManager::Get_Instance()->PlaySound(L"golem_dungeon_golem_crash.wav", MONSTER_EFFECT, g_fMonsterVolume, true);
        }
    }
}
