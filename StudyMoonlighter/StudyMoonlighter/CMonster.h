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

	void RenderHpUi(HDC _hDC);
	void Move_Frame_Hit();
	void InitHitFrame();
	void HitEffect(HDC _hDC);
	MONSTERTYPE Get_MonsterType() { return m_MonsterType; }
protected:
	MONSTERTYPE m_MonsterType;
	FRAME m_HitFrame;
	bool m_bHit;
};

