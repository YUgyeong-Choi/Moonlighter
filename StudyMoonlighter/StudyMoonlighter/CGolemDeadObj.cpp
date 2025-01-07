#include "pch.h"
#include "CGolemDeadObj.h"
#include "CScrollManager.h"

CGolemDeadObj::CGolemDeadObj():m_eType(DEAD_END)
{
}

void CGolemDeadObj::Initialize()
{
    m_eOBJID = OBJ_NOCOL;
    m_eRender = RENDER_BACKGROUND;
    m_tRenderSizeX = 60;
    m_tRenderSizeY = 60;
}

int CGolemDeadObj::Update()
{
    __super::Update_Rect();
	return 0;
}

void CGolemDeadObj::Late_Update()
{
}

void CGolemDeadObj::Render(HDC hDC)
{
    Graphics graphics(hDC);
    Image* image(nullptr);
    switch (m_eType)
    {
    case SLIME:
        image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/deadenemy_slimes.png");
        break;
    case HERMIT:
        image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/deadenemy_hermit.png");
        break;
    case SOLDIER:
        image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/deadenemy_soldier.png");
        break;
    case DEAD_END:
        break;
    default:
        break;
    }
    int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
    int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

    graphics.DrawImage(image, (int)m_tRenderRect.left + iScrollX, (int)m_tRenderRect.top + iScrollY, 0, 0, (int)m_tRenderSizeX, (int)m_tRenderSizeY, UnitPixel);

    if (g_bDevmode) {
        Renderbox(hDC, m_tRenderRect, iScrollX, iScrollY);
    }

    delete image;
}

void CGolemDeadObj::Release()
{
}
