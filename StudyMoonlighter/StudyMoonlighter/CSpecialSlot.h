#pragma once
#include "CInvenSlot.h"
class CSpecialSlot :public CInvenSlot
{
public:
	CSpecialSlot();
	CSpecialSlot(OUTFITTYPE _type) { m_OutfitType = _type; }
	virtual ~CSpecialSlot() { Release(); }
public:
	void Initialize();
	int Update();
	void Late_Update();
	void Render(HDC hDC) override;
	void Release();
};

