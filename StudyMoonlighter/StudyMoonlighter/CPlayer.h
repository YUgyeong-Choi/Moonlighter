#pragma once
#include "CObject.h"
class CPlayer :public CObject
{
public:
	enum STATE { IDLE, WALK, ROLL, FALL, ATTACK, HIT, DEAD, STATE_END };
public:
	CPlayer();
	virtual ~CPlayer() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void OnCollision(CObject* _obj);
public:
	bool Get_IsRolling() { return m_bIsRoll; }
	bool Get_CanHit() { return !m_bIsRoll && m_bCanHit; }
private:
	void Key_Input();
	void Rolling();
	void Change_Motion();
	void SoundEffet();
	void Attack();
private:
	bool m_bIsRoll;
	bool m_bFalling;
	float m_fRollTime;

	bool mbIsAttack;
	float m_fComboTime;

	DIRECTION	m_eCurDir;
	DIRECTION	m_ePreDir;

	STATE               m_eCurState;
	STATE               m_ePreState;
	
	float alpha;

	float m_fTimeSinceLastStep = 0.0f;
};

