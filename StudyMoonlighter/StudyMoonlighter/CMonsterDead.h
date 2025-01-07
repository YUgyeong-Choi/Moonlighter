#pragma once
#include "CObject.h"
class CMonsterDead :public CObject
{
public:
	CMonsterDead();
	virtual ~CMonsterDead() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void Set_ItemType(ITEMTYPE _type) { m_eType = _type; }
private:
	ITEMTYPE m_eType;
	bool m_IsSpawn;
};

