#include "pch.h"
#include "CSellSlot.h"

CSellSlot::CSellSlot()
{
	item.itemId = ITEM_END;
	item.num = 0;
	m_OutfitType = OUTFIT_ITEM;
}

CSellSlot::CSellSlot(int _row, int _column)
{
	rowIndex = _row, columnIndex = _column;
	item.itemId = ITEM_END;
	item.num = 0;
}

void CSellSlot::Render(HDC hDC)
{
	Image* image(nullptr);
	Graphics graphics(hDC);

	if (item.itemId != ITEM_END) {
		image = Image::FromFile(item.pImageUrl);
		graphics.DrawImage(image, 573 + (columnIndex * 200), 136 + (rowIndex * 100), 0, 0, 64, 64, UnitPixel);

		TCHAR szItemNum[64];
		_stprintf_s(szItemNum, _T("%d"), item.num);
		SetTextColor(hDC, RGB(255, 255, 255));
		SetBkMode(hDC, TRANSPARENT);

		HFONT hFont1 = CreateFont(
			25, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"m3x6"
		);


		HFONT OldFont = (HFONT)SelectObject(hDC, hFont1);


		TextOut(hDC, 613 + (columnIndex * 200), 155 + (rowIndex * 100), szItemNum, _tcslen(szItemNum));
		SelectObject(hDC, OldFont);
		DeleteObject(hFont1);
	}


	delete image;
}
