#include "pch.h"
#include "CGolemScroll.h"
#include "CScrollManager.h"
#include "CGolemBossScene.h"
#include "CSceneManager.h"
CGolemScroll::CGolemScroll() :m_bIsOpen(false), m_bIsFinish(false)
{
}


void CGolemScroll::Initialize()
{
	m_eOBJID = OBJ_MAPOBJ;
	m_mapType = DUNGEON_SCROLL;

	m_tInfo.fCX = 0.f;
	m_tInfo.fCY = 0.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 5;
	m_tFrame.dwSpeed = 200;
	m_tFrame.dwTime = GetTickCount64();

	m_tRenderSizeX = 400.f;
	m_tRenderSizeY = 104.f;

	m_eRender = RENDER_GAMEOBJECT;

}

int CGolemScroll::Update()
{
	if (m_bIsFinish) {
		return OBJ_DEAD;
	}
	__super::Update_Rect();
	return 0;
}

void CGolemScroll::Late_Update()
{
	if (!m_bIsOpen) {
		if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount64())
		{
			++m_tFrame.iFrameStart;

			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd) {
				m_tFrame.iFrameStart = m_tFrame.iFrameEnd;
				m_bIsOpen = true;
			}

			m_tFrame.dwTime = GetTickCount64();
		}
	}
	else {
		if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount64())
		{
			--m_tFrame.iFrameStart;

			if (m_tFrame.iFrameStart < 0) {
				m_bIsFinish = true;
			}

			m_tFrame.dwTime = GetTickCount64();
		}
	}
}

void CGolemScroll::Render(HDC hDC)
{
	Image* image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/dungeon_scroll_sheet.png");
	Graphics graphics(hDC);

	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

	graphics.DrawImage(image, (int)m_tRenderRect.left + iScrollX, (int)m_tRenderRect.top + iScrollY, (int)m_tRenderSizeX * m_tFrame.iFrameStart, 0, (int)m_tRenderSizeX, (int)m_tRenderSizeY, UnitPixel);

	HFONT hFont1 = CreateFont(
		30, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"m3x6"
	);
	HFONT OldFont = (HFONT)SelectObject(hDC, hFont1);

	TextOut(hDC, 467, 550, text, wcslen(text));

	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkMode(hDC, TRANSPARENT);

	SelectObject(hDC, OldFont);
	DeleteObject(hFont1);

	if (g_bDevmode) {
		Hitbox(hDC, m_tRect, iScrollX, iScrollY);
		Renderbox(hDC, m_tRenderRect, iScrollX, iScrollY);
	}
	delete image;
}

void CGolemScroll::Release()
{
}
