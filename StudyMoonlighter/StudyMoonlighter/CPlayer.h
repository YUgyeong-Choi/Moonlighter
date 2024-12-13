#pragma once
#include "CObject.h"
class CPlayer :public CObject
{
public:
	enum STATE { IDLE, WALK, ROLL, ATTACK, HIT, DEAD, STATE_END };
public:
	CPlayer();
	virtual ~CPlayer() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
private:
	void Key_Input();
	void Rolling();
	void Offset();
	void Load_Assets();
	void Change_Motion();
	
private:
	bool m_bIsRoll;
	float m_fRollTime;
	DIRECTION	m_eCurDir;
	DIRECTION	m_ePreDir;
	STATE               m_eCurState;
	STATE               m_ePreState;
};

