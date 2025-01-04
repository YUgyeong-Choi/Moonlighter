#pragma once
#include "CInvenSlot.h"
class CPriceSlot :public CInvenSlot
{
public:
	CPriceSlot();
	virtual ~CPriceSlot() { Release(); }
public:
	void Initialize();
	int Update();
	void Late_Update();
	void Render(HDC hDC) override;
	void Release();
};

