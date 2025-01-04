#pragma once
#include "Define.h"
#include "CObject.h"
#include "CPlayer.h"

class CShopPlayer :public CObject
{
private:
	enum STATE { IDLE, WALK, ROLL, STATE_END };
public:
	CShopPlayer();
	virtual ~CShopPlayer() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void Set_State(int hp, int money, int maxHp);
	int Get_Money() { return m_iMoney; }
	void Set_Inven() { m_bInvenOpen = !m_bInvenOpen; }
	bool Get_Inven() { return m_bInvenOpen; }
private:
	void Key_Input();
	void Rolling();
	void Change_Motion();
	void SoundEffet();
private:
	float m_fRollTime;
	int m_iMoney;

	DIRECTION	m_eCurDir;
	DIRECTION	m_ePreDir;

	STATE               m_eCurState;
	STATE               m_ePreState;

	bool m_bIsRoll;
	bool m_bInvenOpen;
};

