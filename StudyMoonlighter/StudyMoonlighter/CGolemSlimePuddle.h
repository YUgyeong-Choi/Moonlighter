#pragma once
#include "CMapObj.h"
class CGolemSlimePuddle :public CMapObj
{
public:
	CGolemSlimePuddle();
	virtual ~CGolemSlimePuddle() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
};

