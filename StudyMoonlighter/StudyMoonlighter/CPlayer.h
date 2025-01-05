#pragma once
#include "CObject.h"
class CPlayer :public CObject
{
private:
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
	int Get_Money() { return m_iMoney; }
	void Set_State(int hp, int money, int maxHp);
	void Set_Inven() { m_bInvenOpen = !m_bInvenOpen; }
	bool Get_Inven() { return m_bInvenOpen; }
private:
	void Key_Input();
	void Rolling();
	void Change_Motion();
	void SoundEffet();
	void Attack();
	
private:
	int m_iMoney;

	bool m_bIsRoll;
	bool m_bFalling;
	bool m_bUsePendant;
	float m_fRollTime;

	bool mbIsAttack;
	bool m_bOnslime;
	float m_fComboTime;

	DIRECTION	m_eCurDir;
	DIRECTION	m_ePreDir;

	STATE               m_eCurState;
	STATE               m_ePreState;
	
	float alpha;

	bool m_bInvenOpen;
	bool m_NoRenderPlayer;
	FRAME m_PandentFrame;

	bool m_SelectFirstWeapon;
};

