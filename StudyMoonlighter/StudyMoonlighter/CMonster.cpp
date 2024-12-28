#include "pch.h"
#include "CMonster.h"

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
	int height = m_tInfo.fY - m_tInfo.fCY * 0.5f;
	int hpWidth = (50 * m_iHp) / m_iMaxHp;

	HPEN hPen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));
	HPEN hOldPen = (HPEN)SelectObject(_hDC, hPen);

	Rectangle(_hDC, (int)m_tInfo.fX - 25, height- 20, (int)m_tInfo.fX + 25, height - 15);

	COLORREF color = RGB(237, 52, 52);
	HBRUSH hBrush = CreateSolidBrush(color);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(_hDC, hBrush);
	Rectangle(_hDC, (int)m_tInfo.fX - 25, height- 20, (int)m_tInfo.fX + -25 + hpWidth, height - 15);

	SelectObject(_hDC, hOldBrush);
	SelectObject(_hDC, hOldPen);
	DeleteObject(hBrush);
	DeleteObject(hPen);
}
