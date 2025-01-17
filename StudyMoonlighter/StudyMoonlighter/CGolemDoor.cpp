#include "pch.h"
#include "CGolemDoor.h"
#include "CObjectManager.h"
#include "CScrollManager.h"

CGolemDoor::CGolemDoor() : m_bIsOpen(false), m_bAlreadyOpen(false), m_bLoad_Next(false)
{
}

void CGolemDoor::Initialize()
{
	m_eOBJID = OBJ_PORTAL;
	m_mapType = GOLEM_DOOR;

	m_tInfo.fCX = 0.f;
	m_tInfo.fCY = 0.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 4;
	m_tFrame.dwSpeed = 200;
	m_tFrame.dwTime = GetTickCount64();

	m_tRenderSizeX = 120.f;
	m_tRenderSizeY = 164.f;

	m_eRender = RENDER_BACKGROUND;
}

int CGolemDoor::Update()
{
	m_bLoad_Next = false;
	if (0 == CObjectManager::Get_Instance()->Get_MonsterSize()) {
		m_bIsOpen = true;
	}

	if (m_bAlreadyOpen) {
		m_tFrame.iFrameStart = m_tFrame.iFrameEnd;
	}
	__super::Update_Rect();
	return 0;
}

void CGolemDoor::Late_Update()
{
	if (m_bIsOpen && (!m_bAlreadyOpen)) {
		if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount64())
		{
			++m_tFrame.iFrameStart;

			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd) {
				m_tFrame.iFrameStart = m_tFrame.iFrameEnd;
				m_bAlreadyOpen = true;
			}

			m_tFrame.dwTime = GetTickCount64();
		}
	}
}

void CGolemDoor::Render(HDC hDC)
{
	Image* image(nullptr);
	switch (m_eDir)
	{
	case LEFT:
		image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/door/golem_left_door.png");
		m_tInfo.fCX = 70.f;
		m_tInfo.fCY = 50.f;
		m_tRenderSizeX = 120.f;
		m_tRenderSizeY = 164.f;
		break;
	case RIGHT:
		image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/door/golem_right_door.png");
		m_tInfo.fCX = 70.f;
		m_tInfo.fCY = 50.f;
		m_tRenderSizeX = 120.f;
		m_tRenderSizeY = 164.f;
		break;
	case UP:
		image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/door/golem_up_door.png");
		m_tInfo.fCX = 50.f;
		m_tInfo.fCY = 70.f;
		m_tRenderSizeX = 164.f;
		m_tRenderSizeY = 120.f;
		break;
	case DOWN:
		image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/door/golem_down_door.png");
		m_tInfo.fCX = 50.f;
		m_tInfo.fCY = 70.f;
		m_tRenderSizeX = 164.f;
		m_tRenderSizeY = 120.f;
		break;
	}
	
	Graphics graphics(hDC);

	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

	graphics.DrawImage(image, (int)m_tRenderRect.left + iScrollX, (int)m_tRenderRect.top + iScrollY, (int)m_tRenderSizeX * m_tFrame.iFrameStart, 0, (int)m_tRenderSizeX, (int)m_tRenderSizeY, UnitPixel);

	if (g_bDevmode) {
		Hitbox(hDC, m_tRect, iScrollX, iScrollY);
		Renderbox(hDC, m_tRenderRect, iScrollX, iScrollY);
	}
	delete image;
}

void CGolemDoor::Release()
{
}

void CGolemDoor::OnCollision(CObject* _obj)
{
	if (_obj->Get_OBJID() == OBJ_PLAYER) {
		if (m_bIsOpen) {
			m_bLoad_Next = true;
		}
		
	}
}
