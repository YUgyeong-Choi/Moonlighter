#include "pch.h"
#include "CMonsterDead.h"
#include "CScrollManager.h"
#include "CAbstractFactory.h"
#include "CItem.h"
#include "CObjectManager.h"
#include "CSoundManager.h"

CMonsterDead::CMonsterDead():m_IsSpawn(false)
{
}

void CMonsterDead::Initialize()
{
    m_eOBJID = OBJ_NOCOL;
    m_eRender = RENDER_BACKGROUND;
    m_tFrame.iFrameStart = 0;
    m_tFrame.iFrameEnd = 11;
    m_tFrame.dwSpeed = 80;
    m_tFrame.dwTime = GetTickCount64();
    m_tRenderSizeX = 64;
    m_tRenderSizeY = 64;
}

int CMonsterDead::Update()
{

    if (m_tFrame.iFrameStart == 6) {
        if (!m_IsSpawn) {
            CObjectManager::Get_Instance()->Add_Object(OBJ_ITEM, CAbstractFactory<CItem>::Create(m_tInfo.fX, m_tInfo.fY));
            static_cast<CItem*>(CObjectManager::Get_Instance()->Get_LastItem())->Set_ItemType(m_eType);
            CSoundManager::Get_Instance()->StopSound(MONSTER_EFFECT);
            CSoundManager::Get_Instance()->PlaySound(L"enemy_death.wav", MONSTER_EFFECT, g_fMonsterVolume+0.1f, true);
            m_IsSpawn = true;
        }
    }  
    
    if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd) {
        return OBJ_DEAD;
    }
    __super::Update_Rect();
	return 0;
}

void CMonsterDead::Late_Update()
{
    __super::Move_Frame();
}

void CMonsterDead::Render(HDC hDC)
{
    Graphics graphics(hDC);
    Image* image(nullptr);
    image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/monster_dead.png");
    int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
    int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

    graphics.DrawImage(image, (int)m_tRenderRect.left + iScrollX, (int)m_tRenderRect.top + iScrollY, (int)m_tRenderSizeX*m_tFrame.iFrameStart, 0, (int)m_tRenderSizeX, (int)m_tRenderSizeY, UnitPixel);

    if (g_bDevmode) {
        Renderbox(hDC, m_tRenderRect, iScrollX, iScrollY);
    }
}

void CMonsterDead::Release()
{
}
