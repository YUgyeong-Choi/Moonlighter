#pragma once
#include "CInvenSlot.h"
class CPriceSlot :public CInvenSlot
{
public:
	CPriceSlot();
	CPriceSlot(int _row, int _column, int num);
	virtual ~CPriceSlot() { Release(); }
public:
	void Initialize();
	int Update();
	void Late_Update();
	void Render(HDC hDC) override;
	void Release();
	void Init_Index() { m_iPriceIndex = 5; m_bActive = true; }
	void Init_Price();
	int Get_Price() { return m_iPrice; }
private:
	void Calc_Price();
	void Key_Input();
private:
	int m_iPriceIndex;
	int m_OnePrice[6];
	int m_iPrice;
	bool m_bActive;
	KEYTYPE key_type;
};

