#pragma once
#include "CObject.h"
class CHouse : public CObject
{
public:
	CHouse();
	virtual ~CHouse() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
};

