#pragma once
#include "CInvenSlot.h"
class CPriceSlot :public CInvenSlot
{
public:
	CPriceSlot();
	CPriceSlot(int _row, int _column);
	virtual ~CPriceSlot() { Release(); }
public:
	void Initialize();
	int Update();
	void Late_Update();
	void Render(HDC hDC) override;
	void Release();
private:
	void Calc_Price();
private:
	int one;
	int two;
	int three;
	int four;
	int five;
	int six;
	int m_iPrice;
};

