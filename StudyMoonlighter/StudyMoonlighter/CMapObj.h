#pragma once
#include "CObject.h"
class CMapObj :public CObject
{
public:
	CMapObj();
	virtual ~CMapObj() { Release(); }
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
};

