#pragma once
#include "CMapObj.h"
class CCollisionBox :public CMapObj
{
public:
	CCollisionBox();
	virtual ~CCollisionBox() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
};

