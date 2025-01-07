#include "pch.h"
#include "CGolemBreakable2.h"
#include "CScrollManager.h"
#include "CObjectManager.h"
#include "CSoundManager.h"

CGolemBreakable2::CGolemBreakable2():m_bIsBreak(true)
{
}

void CGolemBreakable2::Initialize()
{
	m_eOBJID = OBJ_MAPOBJ;
	m_mapType = GOLEM_BROKEN2;
	m_eRender = RENDER_GAMEOBJECT;

	m_tInfo.fCX = 30.f;
	m_tInfo.fCY = 30.f;

	m_tRenderSizeX = 50.f;
	m_tRenderSizeY = 50.f;

	type = rand() % 3;
}

int CGolemBreakable2::Update()
{
	__super::Update_Rect();
	return 0;
}

void CGolemBreakable2::Late_Update()
{
	OnCollision();
}

void CGolemBreakable2::Render(HDC hDC)
{
	Image* image(nullptr);

	if (!m_bIsBreak) {
		switch (type)
		{
		case 0:
			image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/breakable/alreadybreak_1.png");
			break;
		case 1:
			image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/breakable/alreadybreak_2.png");
			break;
		case 2:
			image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/breakable/alreadybreak_3.png");
			break;
		default:
			break;
		}
	}
	else {
		switch (type)
		{
		case 0:
			image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/breakable/breakable_1.png");
			break;
		case 1:
			image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/breakable/breakable_2.png");
			break;
		case 2:
			image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/breakable/breakable_3.png");
			break;
		default:
			break;
		}
	}


	Graphics graphics(hDC);

	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

	graphics.DrawImage(image, (int)m_tRenderRect.left + iScrollX, (int)m_tRenderRect.top + iScrollY, 0, 0, (int)m_tRenderSizeX, (int)m_tRenderSizeY, UnitPixel);

	if (g_bDevmode) {
		Hitbox(hDC, m_tRect, iScrollX, iScrollY);
		Renderbox(hDC, m_tRenderRect, iScrollX, iScrollY);
	}
	delete image;
}

void CGolemBreakable2::Release()
{
}

void CGolemBreakable2::OnCollision(CObject* _obj)
{
}

void CGolemBreakable2::OnCollision()
{
	RECT rc;
	CObject* _copyPlayer = CObjectManager::Get_Instance()->Get_Player();
	if (IntersectRect(&rc,& m_tRect, _copyPlayer->Get_Rect()))
	{
		if (m_bIsBreak) {
			m_tInfo.fCX = 0;
			m_tInfo.fCY = 0;
			m_eRender = RENDER_BACKGROUND;
			m_bIsBreak = false;
			CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
			CSoundManager::Get_Instance()->PlaySound(L"breakable_break.wav", SOUND_EFFECT, g_fEffectVolume, true);
		}

	}
}
