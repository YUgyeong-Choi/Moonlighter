#include "pch.h"
#include "CGolemSoldier.h"
#include "CObjectManager.h"
#include "CScrollManager.h"
#include "CBitManager.h"
#include "CCollisionManager.h"

CGolemSoldier::CGolemSoldier()
{

}

void CGolemSoldier::Initialize()
{
    m_eOBJID = OBJ_MONSTER;

    m_tInfo.fCX = 0.f;
    m_tInfo.fCY = 0.f;
    m_fSpeed = 0.5f;

    m_tFrame.iFrameStart = 0;
    m_tFrame.iFrameEnd = 2;
    m_tFrame.dwSpeed = 200;
    m_tFrame.dwTime = GetTickCount64();

    m_tRenderSizeX = 200.f;
    m_tRenderSizeY = 200.f;
    m_eRender = RENDER_GAMEOBJECT;

    m_targetObj = CObjectManager::Get_Instance()->Get_Player();
}

int CGolemSoldier::Update()
{
    if (m_IsAttack) {
        if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd) {
            m_IsAttack = false;
            m_tFrame.iFrameEnd = 2;
            m_tFrame.dwSpeed = 200;
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

        m_checkCircle = { (int)m_tInfo.fX - 50, (int)m_tInfo.fY - 50,(int)m_tInfo.fX + 50, (int)m_tInfo.fY + 50 };

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
    Check_PlayerIn();
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


    if (g_bDevmode) {
        Renderbox(hDC, m_tInfo.fX - 100, m_tInfo.fY - 100, m_tInfo.fX + 100, m_tInfo.fY + 100, iScrollX, iScrollY);
        Renderbox(hDC, m_HitBox, iScrollX, iScrollY);
        DetectCircle(hDC, m_checkCircle);
    }
}

void CGolemSoldier::Release()
{
}

void CGolemSoldier::Check_PlayerIn()
{
    CObject* _copyPlayer = CObjectManager::Get_Instance()->Get_Player();
    float fRadius = (200 + _copyPlayer->Get_Info().fCX) * 0.5f;

    float fWidth = abs(m_tInfo.fX - _copyPlayer->Get_Info().fX);
    float fHeight = abs(m_tInfo.fY - _copyPlayer->Get_Info().fY);

    float fDiagonal = powf((fWidth * fWidth + fHeight * fHeight), 0.5f);

    if (fRadius >= fDiagonal) {
        if (!m_IsAttack) {
            m_tFrame.iFrameEnd = 12;
            m_tFrame.dwSpeed = 100;
            m_IsAttack = true;
        }
    }
}
