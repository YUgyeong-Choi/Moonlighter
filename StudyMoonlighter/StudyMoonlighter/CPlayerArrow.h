#pragma once
#include "CBullet.h"
class CPlayerArrow :public CBullet
{
public:
	CPlayerArrow();
	virtual ~CPlayerArrow() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void OnCollision(CObject* _obj);
};

