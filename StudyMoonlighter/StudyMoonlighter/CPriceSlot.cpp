#include "pch.h"
#include "CPriceSlot.h"

CPriceSlot::CPriceSlot():one(0),two(0),three(0),four(0),five(0), six(0), m_iPrice(0)
{
	item.itemId = ITEM_END;
	item.num = 0;
	m_OutfitType = OUTFIT_ITEM;
}

CPriceSlot::CPriceSlot(int _row, int _column) :one(0), two(0), three(0), four(0), five(0),six(0), m_iPrice(0)
{
	rowIndex = _row, columnIndex = _column;
	item.itemId = ITEM_END;
	item.num = 0;
}

void CPriceSlot::Initialize()
{
}

int CPriceSlot::Update()
{
	Calc_Price();
	return 0;
}

void CPriceSlot::Late_Update()
{
}

void CPriceSlot::Render(HDC hDC)
{
	SetTextColor(hDC, RGB(186, 175, 154));
	SetBkMode(hDC, TRANSPARENT);

	HFONT hFont1 = CreateFont(
		22, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"m5x7"
	);

	HFONT OldFont = (HFONT)SelectObject(hDC, hFont1);

	TCHAR szPrice[64];
	_stprintf_s(szPrice, _T("%d%d%d%d%d%d"), one, two, three, four,five,six);

	RECT rect = { 530 + (columnIndex * 200) , 195 + ((rowIndex-1) * 100), 680 + (columnIndex  * 200), 255 + ((rowIndex - 1) * 100) };
	DrawText(hDC, szPrice, _tcslen(szPrice), &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	SetTextColor(hDC, RGB(20, 157, 114));
	TCHAR szCount[64];
	_stprintf_s(szCount, _T("x%d"), item.num);

	RECT rect2 = { 640 + (columnIndex * 200) , 230 + ((rowIndex - 1) * 100), 680 + (columnIndex * 200), 250 + ((rowIndex - 1) * 100) };
	DrawText(hDC, szCount, _tcslen(szCount), &rect2, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

	SetTextColor(hDC, RGB(236, 221, 192));
	TCHAR szPriceText[64];
	_stprintf_s(szPriceText, _T("รัพื:"));
	RECT rect3 = { 530 + (columnIndex * 200) , 250 + ((rowIndex - 1) * 100), 580 + (columnIndex * 200), 270 + ((rowIndex - 1) * 100) };
	DrawText(hDC, szPriceText, _tcslen(szPriceText), &rect3, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

	TCHAR szTotalPrice[64];
	_stprintf_s(szTotalPrice, _T("%d"), item.num * m_iPrice);
	RECT rect4 = { 600 + (columnIndex * 200) , 250 + ((rowIndex - 1) * 100), 670 + (columnIndex * 200), 270 + ((rowIndex - 1) * 100) };
	DrawText(hDC, szTotalPrice, _tcslen(szTotalPrice), &rect4, DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

	SelectObject(hDC, OldFont);
	DeleteObject(hFont1);

	if (g_bDevmode) {
		DetectRect(hDC, rect,0,0);
		DetectRect(hDC, rect2,0,0);
		DetectRect(hDC, rect3,0,0);
		DetectRect(hDC, rect4,0,0);
	}
}

void CPriceSlot::Release()
{
}

void CPriceSlot::Calc_Price()
{
	m_iPrice = 0;
	m_iPrice += one * 100000;
	m_iPrice += two * 10000;
	m_iPrice += three * 1000;
	m_iPrice += four * 100;
	m_iPrice += five * 10;
	m_iPrice += six * 1;
}
