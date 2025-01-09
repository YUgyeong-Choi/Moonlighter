#include "pch.h"
#include "CCoin.h"
#include "CBitManager.h"
#include "CScrollManager.h"
#include "CObjectManager.h"
#include "CSoundManager.h"
#include "CPlayer.h"


CCoin::CCoin():m_sound(false)
{
}

void CCoin::Initialize()
{
	m_eOBJID = OBJ_MAPOBJ;
	m_eRender = RENDER_GAMEOBJECT;
	m_tInfo.fCX = 20;
	m_tInfo.fCY = 20;
	m_tRenderSizeX = 48;
	m_tRenderSizeY = 48;
	m_iHp = 1;
}

int CCoin::Update()
{
	if (m_iHp <= 0) {
		return OBJ_DEAD;
	}
	__super::Update_Rect();
	return 0;
}

void CCoin::Late_Update()
{
	OnCollision();
}

void CCoin::Render(HDC hDC)
{
	HDC hMemDC = CBitManager::GetInstance()->FindImage(L"Coin");
	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

	GdiTransparentBlt(hDC, (int)m_tRenderRect.left + iScrollX, (int)m_tRenderRect.top + iScrollY, (int)m_tRenderSizeX, (int)m_tRenderSizeY, hMemDC,0, 0, (int)m_tRenderSizeX, (int)m_tRenderSizeY, RGB(255, 255, 255));

	if (g_bDevmode) {
		Renderbox(hDC, m_tRenderRect, iScrollX, iScrollY);
		Hitbox(hDC, m_tRect, iScrollX, iScrollY);
	}
}

void CCoin::Release()
{
}

void CCoin::OnCollision()
{
	CObject* _copyPlayer = CObjectManager::Get_Instance()->Get_Player();
	RECT rc;
	if (IntersectRect(&rc, _copyPlayer->Get_Rect(), &m_tRect)) {
		if (!m_sound) {
			m_iHp = 0;
			CObject* _player = CObjectManager::Get_Instance()->Get_Player();
			static_cast<CPlayer*>(_player)->Set_Money(-1000);
			m_sound = true;
		}

	}
}
