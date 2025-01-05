#include "pch.h"
#include "CGolemBreakable.h"
#include "CScrollManager.h"
#include "CSoundManager.h"

CGolemBreakable::CGolemBreakable():type(0), m_bIsBreak(true)
{
}

void CGolemBreakable::Initialize()
{
	m_eOBJID = OBJ_MAPOBJ;
	m_mapType = GOLEM_BROKEN;
	m_eRender = RENDER_GAMEOBJECT;

	m_tInfo.fCX = 30.f;
	m_tInfo.fCY = 30.f;

	m_tRenderSizeX = 70.f;
	m_tRenderSizeY = 70.f;

	type = rand() % 2;
}

int CGolemBreakable::Update()
{
	__super::Update_Rect();
	return 0;
}

void CGolemBreakable::Late_Update()
{
}

void CGolemBreakable::Render(HDC hDC)
{
	Image* image(nullptr);

	if (!m_bIsBreak) {
		if (type == 0) {
			image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/breakable/Golem_Camp_BreakVase.png");
		}
		else {
			image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/breakable/Dungeon1_Breakable_Rest.png");
		}
	}
	else {
		if (type == 0) {
			image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/breakable/Dungeon1_Breakable_1.png");
		}
		else {
			image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/breakable/Dungeon1_Breakable_2.png");
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

void CGolemBreakable::Release()
{
}

void CGolemBreakable::OnCollision(CObject* _obj)
{
	if (_obj->Get_OBJID() == OBJ_PLAYER) {
		m_eRender = RENDER_BACKGROUND;
		m_tInfo.fCX = 0.f;
		m_tInfo.fCY = 0.f;
		m_bIsBreak = false;
		CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
		CSoundManager::Get_Instance()->PlaySound(L"breakable_break.wav", SOUND_EFFECT, g_fMonsterVolume, true);
	}
}
