#pragma once
#include "CObject.h"
class CMonster : public CObject
{
public:
	CMonster();
	virtual ~CMonster() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

	MONSTERTYPE Get_MonsterType() { return m_MonsterType; }
protected:
	MONSTERTYPE m_MonsterType;
};

