#include "pch.h"
#include "CMonster.h"
#include "CScrollManager.h"

CMonster::CMonster():m_MonsterType(MONSTER_END), m_iMaxHp(0)
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
