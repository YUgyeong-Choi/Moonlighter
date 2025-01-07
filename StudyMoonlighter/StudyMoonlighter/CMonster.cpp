#include "pch.h"
#include "CMonster.h"
#include "CScrollManager.h"

CMonster::CMonster():m_MonsterType(MONSTER_END), m_bHit(false)
{
}

void CMonster::Initialize()
{
}

int CMonster::Update()
{
	return 0;
}

void CMonster::Late_Update()
{
}

void CMonster::Render(HDC hDC)
{
}

void CMonster::Release()
{
}

void CMonster::RenderHpUi(HDC _hDC)
{
	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

	int height = m_tInfo.fY - m_tInfo.fCY * 0.5f;
	int hpWidth = (50 * m_iHp) / m_iMaxHp;

	HPEN hPen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));
	HPEN hOldPen = (HPEN)SelectObject(_hDC, hPen);

	Rectangle(_hDC, (int)m_tInfo.fX - 25+ iScrollX, height- 20+ iScrollY, (int)m_tInfo.fX + 25+ iScrollX, height - 15+ iScrollY);

	COLORREF color = RGB(237, 52, 52);
	HBRUSH hBrush = CreateSolidBrush(color);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(_hDC, hBrush);
	Rectangle(_hDC, (int)m_tInfo.fX - 25+ iScrollX, height- 20+ iScrollY, (int)m_tInfo.fX + -25+ iScrollX + hpWidth, height - 15+ iScrollY);

	SelectObject(_hDC, hOldBrush);
	SelectObject(_hDC, hOldPen);
	DeleteObject(hBrush);
	DeleteObject(hPen);
}

void CMonster::Move_Frame_Hit()
{
	if (m_HitFrame.dwTime + m_HitFrame.dwSpeed < GetTickCount64())
	{
		++m_HitFrame.iFrameStart;

		if (m_HitFrame.iFrameStart > m_HitFrame.iFrameEnd)
			m_HitFrame.iFrameStart = 0;

		m_HitFrame.dwTime = GetTickCount64();
	}
}

void CMonster::InitHitFrame()
{
	m_HitFrame.iFrameStart = 0;
	m_HitFrame.iFrameEnd = 2;
	m_HitFrame.dwSpeed = 100;
	m_HitFrame.dwTime = GetTickCount64();
}

void CMonster::HitEffect(HDC _hDC)
{
	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();
	Graphics graphics(_hDC);
	Image* image(nullptr);
	image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/hit_effect.png");
	graphics.DrawImage(image, (int)m_tInfo.fX - 32+ iScrollX, (int)m_tInfo.fY - 32+ iScrollY, m_HitFrame.iFrameStart*64, 0, 64, 64, UnitPixel);

	delete image;
}
