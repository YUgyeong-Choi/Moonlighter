#include "pch.h"
#include "CTile.h"
#include "CScrollManager.h"
#include "CBitManager.h"

CTile::CTile():m_bIsWalk(true)
{
}

void CTile::Initialize()
{
	m_tInfo.fCX = TILECX;
	m_tInfo.fCY = TILECY;
}

int CTile::Update()
{
	__super::Update_Rect();
	return 0;
}

void CTile::Late_Update()
{
}

void CTile::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();
	if (m_bIsWalk) {
		HDC		hMemDC = CBitManager::GetInstance()->FindImage(L"CanWalk");
		GdiTransparentBlt(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, 48, 48, hMemDC, 0, 0, 48, 48, RGB(255, 255, 255));
	}
	else {
		HDC		hMemDC = CBitManager::GetInstance()->FindImage(L"CantWalk");
		GdiTransparentBlt(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, 48, 48, hMemDC, 0, 0, 48, 48, RGB(255, 255, 255));
	}
	Renderbox(hDC, m_tRect, iScrollX, iScrollY);
}

void CTile::Release()
{
}
