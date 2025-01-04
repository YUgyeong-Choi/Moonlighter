#pragma once
#include "CInvenSlot.h"
class CSellSlot :public CInvenSlot
{
public:
	CSellSlot();
	CSellSlot(int _row, int _column);
	virtual ~CSellSlot() { Release(); }
	void Render(HDC hDC);
};

