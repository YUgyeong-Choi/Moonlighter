#pragma once
#include "CBullet.h"
class CTurretDownBullet :public CBullet
{
public:
	CTurretDownBullet();
	virtual ~CTurretDownBullet() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void OnCollision(CObject* _obj);
};

