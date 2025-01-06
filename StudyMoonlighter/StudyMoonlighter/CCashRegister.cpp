#include "pch.h"
#include "CCashRegister.h"
#include "CScrollManager.h"
#include "CObjectManager.h"

CCashRegister::CCashRegister():m_animActive(false)
{
}

void CCashRegister::Initialize()
{
	m_eOBJID = OBJ_MAPOBJ;

	m_tInfo.fCX = 0.f;
	m_tInfo.fCY = 0.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 6;
	m_tFrame.dwSpeed = 100;
	m_tFrame.dwTime = GetTickCount64();

	m_tRenderSizeX = 60.f;
	m_tRenderSizeY = 54.f;

	m_eRender = RENDER_GAMEOBJECT;
	m_HitBox = { 610, 600, 670, 630 };
}

int CCashRegister::Update()
{
	if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd) {
		m_animActive = false;
		m_tFrame.iFrameStart = 0;
	}
	__super::Update_Rect();
	return 0;
}

void CCashRegister::Late_Update()
{
	OnCollision();
	if (m_animActive) {
		__super::Move_Frame();
	}
}

void CCashRegister::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();
	Graphics graphics(hDC);
	Image* image(nullptr);
	image = Image::FromFile(L"../MoonlighterAssets/Map/Shop/cash_register_open.png");
	graphics.DrawImage(image, (int)m_tRenderRect.left + iScrollX , (int)m_tRenderRect.top + iScrollY-10, (int)m_tRenderSizeX*m_tFrame.iFrameStart, 0, (int)m_tRenderSizeX, (int)m_tRenderSizeY, UnitPixel);

	delete image;

	if (g_bDevmode) {
		Renderbox(hDC, m_tRenderRect, iScrollX, iScrollY);
		Hitbox(hDC, m_HitBox, iScrollX, iScrollY);
	}
}

void CCashRegister::Release()
{
}

void CCashRegister::OnCollision(CObject* _obj)
{
}

void CCashRegister::OnCollision()
{
	RECT rc;
	list<CObject*> _npclist = CObjectManager::Get_Instance()->Get_NpcList();
	for (auto& _npc : _npclist) {
		if (IntersectRect(&rc, &m_HitBox, _npc->Get_Rect()))
		{
			m_animActive = true;
		}
	}
}
