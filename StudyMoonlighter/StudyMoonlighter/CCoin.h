#pragma once
#include "CObject.h"
class CCoin :public CObject
{
public:
	CCoin();
	virtual ~CCoin() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
private:
	void OnCollision();
private:
	bool m_sound;
};

