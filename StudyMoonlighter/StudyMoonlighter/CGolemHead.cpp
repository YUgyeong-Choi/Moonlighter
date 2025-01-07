#include "pch.h"
#include "CGolemHead.h"
#include "CObjectManager.h"
#include "CScrollManager.h"
#include "CBitManager.h"
#include "CCollisionManager.h"
#include "CSoundManager.h"
#include "CItem.h"
#include "CAbstractFactory.h"

CGolemHead::CGolemHead():m_AttackCount(0), m_IsAttack(false), m_fAngle(0)
{
}

void CGolemHead::Initialize()
{
    m_eOBJID = OBJ_MONSTER;
    m_MonsterType = GOLEM_HEAD;

    m_tInfo.fCX = 50.f;
    m_tInfo.fCY = 50.f;
    m_fSpeed = 8.f;

    m_tFrame.iFrameStart = 0;
    m_tFrame.iFrameEnd = 11;
    m_tFrame.dwSpeed = 100;
    m_tFrame.dwTime = GetTickCount64();

    m_tRenderSizeX = 80.f;
    m_tRenderSizeY = 80.f;
    m_eRender = RENDER_GAMEOBJECT;

    m_targetObj = CObjectManager::Get_Instance()->Get_Player();

    m_AttackCount = 2;

    m_iHp = 150;
    m_iMaxHp = m_iHp;
    m_iAttackDamage = 5;
    InitHitFrame();
}

int CGolemHead::Update()
{
    if (m_iHp <= 0) {
        CObjectManager::Get_Instance()->Add_Object(OBJ_ITEM, CAbstractFactory<CItem>::Create(m_tInfo.fX, m_tInfo.fY));
        static_cast<CItem*>(CObjectManager::Get_Instance()->Get_LastItem())->Set_ItemType(TEETHSTONE);
        return OBJ_DEAD;
    }
    if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd && !m_IsAttack) {
        m_AttackCount--;
        if (m_AttackCount == 0) {
            float   fWidth(0.f), fHeight(0.f), fDiagonal(0.f), fRadian(0.f);

            fWidth = m_targetObj->Get_Info().fX - m_tInfo.fX;
            fHeight = m_targetObj->Get_Info().fY - m_tInfo.fY;

            fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

            fRadian = acosf(fWidth / fDiagonal);

            m_fAngle = fRadian * (180.f / PI);

            if (m_targetObj->Get_Info().fY > m_tInfo.fY)
                m_fAngle *= -1.f;

            m_IsAttack = true;
            m_tFrame.iFrameEnd = 4;
            CSoundManager::Get_Instance()->StopSound(MONSTER_EFFECT);
            CSoundManager::Get_Instance()->PlaySound(L"golem_dungeon_golem_roll.wav", MONSTER_EFFECT, g_fMonsterVolume+0.1f, true);
        }
    }

    if (m_IsAttack) {
        m_tInfo.fX += m_fSpeed * cosf(m_fAngle * (PI / 180.f));
        m_tInfo.fY -= m_fSpeed * sinf(m_fAngle * (PI / 180.f));
    }
    else {
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

void CGolemHead::Late_Update()
{
    Hit();
    OnCollision();
    if (m_bHit) {
        Move_Frame_Hit();
    }
    __super::Move_Frame();
}

void CGolemHead::Render(HDC hDC)
{
    Image* image(nullptr);
    if (m_IsAttack) {
        switch (m_eDir)
        {
        case LEFT:
            image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/golem_head/golem_head_rollleft.png");
            break;
        case RIGHT:
            image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/golem_head/golem_head_rollright.png");
            break;
        case UP:
            image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/golem_head/golem_head_rollup.png");
            break;
        case DOWN:
            image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/golem_head/golem_head_rolldown.png");
            break;
        }
    }
    else {
        switch (m_eDir)
        {
        case LEFT:
            image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/golem_head/golem_head_preleft.png");
            break;
        case RIGHT:
            image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/golem_head/golem_head_preright.png");
            break;
        case UP:
            image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/golem_head/golem_head_preup.png");
            break;
        case DOWN:
            image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/golem_head/golem_head_predown.png");
            break;
        }
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
        //HitCircle(hDC, m_checkCircle);
    }
    delete image;
}

void CGolemHead::Release()
{
}

void CGolemHead::OnCollision(CObject* _obj)
{
    if (_obj->Get_OBJID() == OBJ_MAPOBJ) {
        if (m_IsAttack) {
            m_IsAttack = false;
            m_AttackCount = 2;
            m_tFrame.iFrameEnd = 11;
        }
    }
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

void CGolemHead::OnCollision()
{
    CObject* _copyPlayer = CObjectManager::Get_Instance()->Get_Player();

    if (CCollisionManager::CollisionRectWeapon(_copyPlayer, this)) {
        if (m_bCanHit) {
            if (m_fAttacktedTime + 500 < GetTickCount64()) {
                m_iAttackedDamage = _copyPlayer->Get_AttackDamage();
                m_bCanHit = false;
                m_fAttacktedTime = GetTickCount64();
                CSoundManager::Get_Instance()->StopSound(MONSTER_EFFECT);
                CSoundManager::Get_Instance()->PlaySound(L"golem_dungeon_babyslime_hit.wav", MONSTER_EFFECT, g_fMonsterVolume + 0.5f, true);
                m_bHit = true;
            }
        }
    }
}
