#include "pch.h"
#include "CGolemMiniboss.h"
#include "CObjectManager.h"
#include "CScrollManager.h"
#include "CBitManager.h"
#include "CCollisionManager.h"
#include "CSoundManager.h"
#include "CGolemScroll.h"
#include "CAbstractFactory.h"
#include "CItem.h"

CGolemMiniboss::CGolemMiniboss(): m_eCurPatten(GolemMiniPattern::END), m_ePrePatten(GolemMiniPattern::END), m_iAttackTick(0), m_iAttackMaze(0), m_IsAttack(false), m_radius(0)
{
    ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/miniboss/miniboss_idle_down.bmp", L"MiniBossIdleDown");
    ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/miniboss/miniboss_idle_left.bmp", L"MiniBossIdleLeft");
    ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/miniboss/miniboss_idle_right.bmp", L"MiniBossIdleRight");
    ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/miniboss/miniboss_idle_up.bmp", L"MiniBossIdleUp");
    ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/miniboss/miniboss_sword_down.bmp", L"MiniBossSwordDown");
    ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/miniboss/miniboss_sword_left.bmp", L"MiniBossSwordLeft");
    ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/miniboss/miniboss_sword_right.bmp", L"MiniBossSwordRight");
    ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/miniboss/miniboss_sword_up.bmp", L"MiniBossSwordUp");
    ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/miniboss/miniboss_maze_down.bmp", L"MiniBossMazeDown");
    ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/miniboss/miniboss_maze_left.bmp", L"MiniBossMazeLeft");
    ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/miniboss/miniboss_maze_right.bmp", L"MiniBossMazeRight");
    ADD_BMP(L"../MoonlighterAssets/Map/Dungeon1/miniboss/miniboss_maze_up.bmp", L"MiniBossMazeUp");
}

void CGolemMiniboss::Initialize()
{
    m_eOBJID = OBJ_MONSTER;
    m_MonsterType = MINIBOSS;

    m_tInfo.fCX = 100.f;
    m_tInfo.fCY = 100.f;
    m_fSpeed = 0.5f;

    m_tFrame.iFrameStart = 0;
    m_tFrame.iFrameEnd = 6;
    m_tFrame.dwSpeed = 200;
    m_tFrame.dwTime = GetTickCount64();

    m_tRenderSizeX = 300.f;
    m_tRenderSizeY = 300.f;
    m_eRender = RENDER_GAMEOBJECT;

    m_targetObj = CObjectManager::Get_Instance()->Get_Player();
    m_eDir = LEFT;
    m_eCurPatten = IDLE;
    m_iAttackMaze = 3;

    m_iHp = 300;
    m_iMaxHp= m_iHp; 
    m_iAttackDamage = 23;
    InitHitFrame();
}

int CGolemMiniboss::Update()
{
    if (m_iHp <= 0) {
        CObjectManager::Get_Instance()->Add_Object(OBJ_ITEM, CAbstractFactory<CItem>::Create(m_tInfo.fX-10, m_tInfo.fY));
        static_cast<CItem*>(CObjectManager::Get_Instance()->Get_LastItem())->Set_ItemType(WATERSPHERE);
        CObjectManager::Get_Instance()->Add_Object(OBJ_ITEM, CAbstractFactory<CItem>::Create(m_tInfo.fX+10, m_tInfo.fY));
        static_cast<CItem*>(CObjectManager::Get_Instance()->Get_LastItem())->Set_ItemType(HARDENEDSTEEL);
        return OBJ_DEAD;
    }
    m_iAttackTick++;

    if (m_iAttackTick > 300 && !m_IsAttack) {
        m_eCurPatten = SWORD;
        m_iAttackTick = 0;
        m_iAttackMaze--;
    }

    float   fWidth(0.f), fHeight(0.f), fDiagonal(0.f), fRadian(0.f), m_fAngle(0.f);

    float x = m_targetObj->Get_Info().fX - m_tInfo.fX;
    float y = m_targetObj->Get_Info().fY - m_tInfo.fY;

    switch (m_eCurPatten)
    {
    case CGolemMiniboss::IDLE:

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

        fWidth = m_targetObj->Get_Info().fX - m_tInfo.fX;
        fHeight = m_targetObj->Get_Info().fY - m_tInfo.fY;

        fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

        fRadian = acosf(fWidth / fDiagonal);

        m_fAngle = fRadian * (180.f / PI);

        if (m_targetObj->Get_Info().fY > m_tInfo.fY)
            m_fAngle *= -1.f;

        m_tInfo.fX += m_fSpeed * cosf(m_fAngle * (PI / 180.f));
        m_tInfo.fY -= m_fSpeed * sinf(m_fAngle * (PI / 180.f));

        break;
    case CGolemMiniboss::MAZE:
        m_IsAttack = true;
        if (m_tFrame.iFrameStart == 7) {
            MazeAttack();
        }
        m_radius += 5;
        if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd) {
            m_eCurPatten = IDLE;
            m_HitBox = { 0,0,0,0 };
            m_IsAttack = false;
            m_radius = 100;
        }
        break;
    case CGolemMiniboss::SWORD:
        if (m_iAttackMaze == 0) {
            m_eCurPatten = MAZE;
            m_HitBox = { 0,0,0,0 };
            m_iAttackMaze = 3;
            break;
        }
        m_IsAttack = true;
        SwordAttack();
        if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd) {
            m_eCurPatten = IDLE;
            m_HitBox = { 0,0,0,0 };
            m_IsAttack = false;
        }
        break;
    case CGolemMiniboss::END:
        break;
    default:
        break;
    }


    Change_Motion();
    __super::Update_Rect();
	return 0;
}

void CGolemMiniboss::Late_Update()
{
    OnCollision();
    Hit();
    if (m_bHit) {
        Move_Frame_Hit();
    }
    __super::Move_Frame();
}

void CGolemMiniboss::Render(HDC hDC)
{
    int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
    int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();
    if (m_eCurPatten == MAZE) {
        COLORREF color = RGB(9, 51, 48);

        HBRUSH hBrush = CreateSolidBrush(color);
        HPEN hPen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
        HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
        Ellipse(hDC, m_HitBox.left+ iScrollX, m_HitBox.top+ iScrollY, m_HitBox.right+ iScrollX, m_HitBox.bottom+ iScrollY);

        SelectObject(hDC, hOldBrush);
        SelectObject(hDC, hOldPen);
        DeleteObject(hBrush);
        DeleteObject(hPen);
    }

    HDC hMemDC = CBitManager::GetInstance()->FindImage(L"MiniBossSwordLeft");
    switch (m_eCurPatten)
    {
    case CGolemMiniboss::IDLE:
        switch (m_eDir)
        {
        case LEFT:
            hMemDC = CBitManager::GetInstance()->FindImage(L"MiniBossIdleLeft");
            GdiTransparentBlt(hDC, m_tRenderRect.left + iScrollX, m_tRenderRect.top + iScrollY, (int)m_tRenderSizeX, (int)m_tRenderSizeY, hMemDC, m_tFrame.iFrameStart * m_tRenderSizeX, 0, m_tRenderSizeX, m_tRenderSizeY, RGB(255, 255, 255));
            break;
        case RIGHT:
            hMemDC = CBitManager::GetInstance()->FindImage(L"MiniBossIdleRight");
            GdiTransparentBlt(hDC, m_tRenderRect.left + iScrollX, m_tRenderRect.top + iScrollY, (int)m_tRenderSizeX, (int)m_tRenderSizeY, hMemDC, m_tFrame.iFrameStart * m_tRenderSizeX, 0, m_tRenderSizeX, m_tRenderSizeY, RGB(255, 255, 255));
            break;
        case UP:
            hMemDC = CBitManager::GetInstance()->FindImage(L"MiniBossIdleUp");
            GdiTransparentBlt(hDC, m_tRenderRect.left + iScrollX, m_tRenderRect.top + iScrollY, (int)m_tRenderSizeX, (int)m_tRenderSizeY, hMemDC, m_tFrame.iFrameStart * m_tRenderSizeX, 0, m_tRenderSizeX, m_tRenderSizeY, RGB(255, 255, 255));
            break;
        case DOWN:
            hMemDC = CBitManager::GetInstance()->FindImage(L"MiniBossIdleDown");
            GdiTransparentBlt(hDC, m_tRenderRect.left + iScrollX, m_tRenderRect.top + iScrollY, (int)m_tRenderSizeX, (int)m_tRenderSizeY, hMemDC, m_tFrame.iFrameStart * m_tRenderSizeX, 0, m_tRenderSizeX, m_tRenderSizeY, RGB(255, 255, 255));
            break;
        }
        break;
    case CGolemMiniboss::MAZE:
        switch (m_eDir)
        {
        case LEFT:
            hMemDC = CBitManager::GetInstance()->FindImage(L"MiniBossMazeLeft");
            GdiTransparentBlt(hDC, m_tRenderRect.left + iScrollX, m_tRenderRect.top + iScrollY, (int)m_tRenderSizeX, (int)m_tRenderSizeY, hMemDC, m_tFrame.iFrameStart * m_tRenderSizeX, 0, m_tRenderSizeX, m_tRenderSizeY, RGB(255, 255, 255));
            break;
        case RIGHT:
            hMemDC = CBitManager::GetInstance()->FindImage(L"MiniBossMazeRight");
            GdiTransparentBlt(hDC, m_tRenderRect.left + iScrollX, m_tRenderRect.top + iScrollY, (int)m_tRenderSizeX, (int)m_tRenderSizeY, hMemDC, m_tFrame.iFrameStart * m_tRenderSizeX, 0, m_tRenderSizeX, m_tRenderSizeY, RGB(255, 255, 255));
            break;
        case UP:
            hMemDC = CBitManager::GetInstance()->FindImage(L"MiniBossMazeUp");
            GdiTransparentBlt(hDC, m_tRenderRect.left + iScrollX, m_tRenderRect.top + iScrollY, (int)m_tRenderSizeX, (int)m_tRenderSizeY, hMemDC, m_tFrame.iFrameStart * m_tRenderSizeX, 0, m_tRenderSizeX, m_tRenderSizeY, RGB(255, 255, 255));
            break;
        case DOWN:
            hMemDC = CBitManager::GetInstance()->FindImage(L"MiniBossMazeDown");
            GdiTransparentBlt(hDC, m_tRenderRect.left + iScrollX, m_tRenderRect.top + iScrollY, (int)m_tRenderSizeX, (int)m_tRenderSizeY, hMemDC, m_tFrame.iFrameStart * m_tRenderSizeX, 0, m_tRenderSizeX, m_tRenderSizeY, RGB(255, 255, 255));
            break;
        }
        break;
    case CGolemMiniboss::SWORD:
        switch (m_eDir)
        {
        case LEFT:
            hMemDC = CBitManager::GetInstance()->FindImage(L"MiniBossSwordLeft");
            GdiTransparentBlt(hDC, m_tRenderRect.left + iScrollX, m_tRenderRect.top + iScrollY, (int)m_tRenderSizeX, (int)m_tRenderSizeY, hMemDC, m_tFrame.iFrameStart * m_tRenderSizeX, 0, m_tRenderSizeX, m_tRenderSizeY, RGB(255, 255, 255));
            break;
        case RIGHT:
            hMemDC = CBitManager::GetInstance()->FindImage(L"MiniBossSwordRight");
            GdiTransparentBlt(hDC, m_tRenderRect.left + iScrollX, m_tRenderRect.top + iScrollY, (int)m_tRenderSizeX, (int)m_tRenderSizeY, hMemDC, m_tFrame.iFrameStart * m_tRenderSizeX, 0, m_tRenderSizeX, m_tRenderSizeY, RGB(255, 255, 255));
            break;
        case UP:
            hMemDC = CBitManager::GetInstance()->FindImage(L"MiniBossSwordUp");
            GdiTransparentBlt(hDC, m_tRenderRect.left + iScrollX, m_tRenderRect.top + iScrollY, (int)m_tRenderSizeX, (int)m_tRenderSizeY, hMemDC, m_tFrame.iFrameStart * m_tRenderSizeX, 0, m_tRenderSizeX, m_tRenderSizeY, RGB(255, 255, 255));
            break;
        case DOWN:
            hMemDC = CBitManager::GetInstance()->FindImage(L"MiniBossSwordDown");
            GdiTransparentBlt(hDC, m_tRenderRect.left + iScrollX, m_tRenderRect.top + iScrollY, (int)m_tRenderSizeX, (int)m_tRenderSizeY, hMemDC, m_tFrame.iFrameStart * m_tRenderSizeX, 0, m_tRenderSizeX, m_tRenderSizeY, RGB(255, 255, 255));
            break;
        }
        break;
    case CGolemMiniboss::END:
        break;
    default:
        break;
    }

    RenderHpUi(hDC);
    if (!m_bCanHit) {
        HitEffect(hDC);
    }

    if (g_bDevmode) {
        Renderbox(hDC, m_tRenderRect, iScrollX, iScrollY);
        Hitbox(hDC, m_tRect, iScrollX, iScrollY);
        Attackbox(hDC, m_HitBox, iScrollX, iScrollY);
    }
}

void CGolemMiniboss::Release()
{
}

void CGolemMiniboss::RenderHpUi(HDC _hDC)
{
    int height = m_tInfo.fY - m_tInfo.fCY * 0.5f;
    int hpWidth = (800 * m_iHp) / m_iMaxHp;
    int start = WINCX / 2 - 400;

    HPEN hPen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(_hDC, hPen);

    COLORREF color = RGB(237, 52, 52);
    HBRUSH hBrush = CreateSolidBrush(color);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(_hDC, hBrush);
    RoundRect(_hDC, start,650 , start + hpWidth, 665,10,10);

    SelectObject(_hDC, hOldBrush);
    SelectObject(_hDC, hOldPen);
    DeleteObject(hBrush);
    DeleteObject(hPen);
}

void CGolemMiniboss::OnCollision()
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
void CGolemMiniboss::OnCollision(CObject* _obj)
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

void CGolemMiniboss::Change_Motion()
{
    if (m_ePrePatten != m_eCurPatten)
    {
        switch (m_eCurPatten)
        {
        case CGolemMiniboss::IDLE:
            CSoundManager::Get_Instance()->StopSound(MONSTER_EFFECT);
            CSoundManager::Get_Instance()->PlaySound(L"Miniboss_golemwarrior_idle_corrupted.wav", MONSTER_EFFECT, g_fMonsterVolume, true);
            m_tRenderSizeX = 300.f;
            m_tRenderSizeY = 300.f;
            m_tFrame.iFrameStart = 0;
            m_tFrame.iFrameEnd = 6;
            m_tFrame.dwSpeed = 200;
            m_tFrame.dwTime = GetTickCount64();
            break;
        case CGolemMiniboss::MAZE:
            CSoundManager::Get_Instance()->StopSound(MONSTER_EFFECT);
            CSoundManager::Get_Instance()->PlaySound(L"Miniboss_golemwarrior_maze.wav", MONSTER_EFFECT, g_fMonsterVolume, true);
            m_tRenderSizeX = 600.f;
            m_tRenderSizeY = 600.f;
            m_tFrame.iFrameStart = 0;
            m_tFrame.iFrameEnd = 14;
            m_tFrame.dwSpeed = 100;
            m_tFrame.dwTime = GetTickCount64();
            break;
        case CGolemMiniboss::SWORD:
            CSoundManager::Get_Instance()->StopSound(MONSTER_EFFECT);
            CSoundManager::Get_Instance()->PlaySound(L"Miniboss_golemwarrior_sword.wav", MONSTER_EFFECT, g_fMonsterVolume, true);
            m_tRenderSizeX = 600.f;
            m_tRenderSizeY = 600.f;
            m_tFrame.iFrameStart = 0;
            m_tFrame.iFrameEnd = 12;
            m_tFrame.dwSpeed = 100;
            m_tFrame.dwTime = GetTickCount64();
            break;
        case CGolemMiniboss::END:
            break;
        default:
            break;
        }
        m_ePrePatten = m_eCurPatten;
    }
}

void CGolemMiniboss::SwordAttack()
{

    switch (m_eDir)
    {
    case LEFT:
        m_tInfo.fX -= 1.f;
        m_HitBox = { (int)m_tInfo.fX - 200, (int)m_tInfo.fY -100,(int)m_tInfo.fX, (int)m_tInfo.fY + 100 };
        break;
    case RIGHT:
        m_tInfo.fX += 1.f;
        m_HitBox = { (int)m_tInfo.fX + 200, (int)m_tInfo.fY - 100,(int)m_tInfo.fX, (int)m_tInfo.fY + 100 };
        break;
    case UP:
        m_tInfo.fY -= 1.f;
        m_HitBox = { (int)m_tInfo.fX - 100, (int)m_tInfo.fY - 200,(int)m_tInfo.fX +100, (int)m_tInfo.fY};
        break;
    case DOWN:
        m_tInfo.fY += 1.f;
        m_HitBox = { (int)m_tInfo.fX - 100, (int)m_tInfo.fY + 200,(int)m_tInfo.fX + 100, (int)m_tInfo.fY };
        break;
    }
}

void CGolemMiniboss::MazeAttack()
{
    m_HitBox = { (int)m_tInfo.fX - m_radius, (int)m_tInfo.fY - m_radius,(int)m_tInfo.fX + m_radius, (int)m_tInfo.fY + m_radius };
}
