#pragma once
#include "CObject.h"
class CGamble :public CObject
{
public:
	CGamble();
	virtual ~CGamble() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
private:
	void OnCollision();
	void Key_Input();
private:
	bool m_bActive;
	bool m_ShowUi;
};

