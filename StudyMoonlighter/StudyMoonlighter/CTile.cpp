#include "pch.h"
#include "CTile.h"
#include "CScrollManager.h"
#include "CBitManager.h"

CTile::CTile():m_bIsWalk(false)
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
	if (m_bIsWalk) {
		HDC		hMemDC = CBitManager::GetInstance()->FindImage(L"CanWalk");
		int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
		int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();
		GdiTransparentBlt(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, 32, 32, hMemDC, 0, 0, 32, 32, RGB(255, 255, 255));
	}
	else {
		HDC		hMemDC = CBitManager::GetInstance()->FindImage(L"CantWalk");
		int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
		int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();
		GdiTransparentBlt(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, 32, 32, hMemDC, 0, 0, 32, 32, RGB(255, 255, 255));
	}
}

void CTile::Release()
{
}
