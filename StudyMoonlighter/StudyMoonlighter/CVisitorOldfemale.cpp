#include "pch.h"
#include "CVisitorOldfemale.h"
#include "CScrollManager.h"

CVisitorOldfemale::CVisitorOldfemale()
{
}

void CVisitorOldfemale::Render(HDC hDC)
{
	Image* image(nullptr);
	Graphics graphics(hDC);

	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();


	switch (m_eCurPattern)
	{
	case CVisitor::IDLE:
		switch (m_eDir)
		{
		case LEFT:
			image = Image::FromFile(L"../MoonlighterAssets/Visitor/Oldfemale/idle_left.png");
			break;
		case RIGHT:
			image = Image::FromFile(L"../MoonlighterAssets/Visitor/Oldfemale/idle_right.png");
			break;
		case UP:
			image = Image::FromFile(L"../MoonlighterAssets/Visitor/Oldfemale/idle_up.png");
			break;
		case DOWN:
			image = Image::FromFile(L"../MoonlighterAssets/Visitor/Oldfemale/idle_down.png");
			break;
		}
		break;
	case CVisitor::WALK:
		switch (m_eDir)
		{
		case LEFT:
			image = Image::FromFile(L"../MoonlighterAssets/Visitor/Oldfemale/walk_left.png");
			break;
		case RIGHT:
			image = Image::FromFile(L"../MoonlighterAssets/Visitor/Oldfemale/walk_right.png");
			break;
		case UP:
			image = Image::FromFile(L"../MoonlighterAssets/Visitor/Oldfemale/walk_up.png");
			break;
		case DOWN:
			image = Image::FromFile(L"../MoonlighterAssets/Visitor/Oldfemale/walk_down.png");
			break;
		}
		break;
	}
	graphics.DrawImage(image, (int)m_tRenderRect.left + iScrollX, (int)m_tRenderRect.top + iScrollY, (int)m_tRenderSizeX * m_tFrame.iFrameStart, 0, (int)m_tRenderSizeX, (int)m_tRenderSizeY, UnitPixel);

	delete image;

	if (m_bSellXY && !m_bBeforeTableXY) {
		Render_Talk(hDC);
	}

	if (g_bDevmode) {
		Renderbox(hDC, m_tRenderRect, iScrollX, iScrollY);
		Hitbox(hDC, m_tRect, iScrollX, iScrollY);
	}
}

void CVisitorOldfemale::Release()
{
}