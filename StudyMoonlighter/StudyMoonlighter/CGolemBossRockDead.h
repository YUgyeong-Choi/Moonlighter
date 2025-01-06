#pragma once
#include "CObject.h"
class CGolemBossRockDead :public CObject
{
public:
	CGolemBossRockDead();
	virtual ~CGolemBossRockDead() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
};

