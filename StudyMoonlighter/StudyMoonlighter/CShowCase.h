#pragma once
#include "CObject.h"
class CShowCase :public CObject
{
public:
	CShowCase();
	virtual ~CShowCase() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
private:
	void OnCollision();
	void KeyInput();
private:
	bool m_bActive;
	ITEM m_sellItem;
};

