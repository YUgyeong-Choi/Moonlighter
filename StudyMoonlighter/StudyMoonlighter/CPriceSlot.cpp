#include "pch.h"
#include "CPriceSlot.h"

CPriceSlot::CPriceSlot()
{
	item.itemId = ITEM_END;
	item.num = 0;
	m_OutfitType = OUTFIT_ITEM;
}

CPriceSlot::CPriceSlot(int _row, int _column)
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
	return 0;
}

void CPriceSlot::Late_Update()
{
}

void CPriceSlot::Render(HDC hDC)
{
}

void CPriceSlot::Release()
{
}
