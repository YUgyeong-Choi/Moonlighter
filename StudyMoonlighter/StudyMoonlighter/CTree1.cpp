#include "pch.h"
#include "CTree1.h"
#include "CBitManager.h"
#include "CScrollManager.h"

CTree1::CTree1()
{
}

void CTree1::Initialize()
{
	m_eOBJID = OBJ_MAPOBJ;
	
	m_tInfo.fCX = 30.f;
	m_tInfo.fCY = 30.f;

	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/Map/Object/Tree/Village_Tree_01.bmp", L"Tree1");
	m_tRenderSizeX = 140.f;
	m_tRenderSizeY = 170.f;

	m_eRender = RENDER_GAMEOBJECT;
}

int CTree1::Update()
{
	__super::Update_Rect();
	return 0;
}

void CTree1::Late_Update()
{
}

void CTree1::Render(HDC hDC)
{
	HDC hMemDC = CBitManager::GetInstance()->FindImage(L"Tree1");
	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();
	GdiTransparentBlt(hDC, m_tRenderRect.left+ iScrollX, m_tRenderRect.top+ iScrollY, m_tRenderSizeX, m_tRenderSizeY, hMemDC,  0, 0, m_tRenderSizeX, m_tRenderSizeY, RGB(255, 255, 255));
	if (g_bDevmode) {
		Hitbox(hDC, m_tRect, iScrollX, iScrollY);
		Renderbox(hDC, m_tRenderRect, iScrollX, iScrollY);
	}
}

void CTree1::Release()
{
}
