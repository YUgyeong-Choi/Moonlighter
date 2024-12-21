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
	bool Get_IsRolling() { return m_bIsRoll; }
	void		OnCollision(CObject* _obj);
private:
	void Key_Input();
	void Rolling();
	void Hit();
	void Change_Motion();
	//void Move_Lock();
	void SoundEffet();
	void Attack();
private:
	bool m_bIsRoll;
	bool m_bFalling;
	bool mbIsAttack;
	float m_fComboTime;
	float m_fRollTime;

	bool m_bCanHit;
	int m_iAttackedDamage;

	DIRECTION	m_eCurDir;
	DIRECTION	m_ePreDir;

	STATE               m_eCurState;
	STATE               m_ePreState;
	
	float alpha;

	float m_fTimeSinceLastStep = 0.0f;
};

