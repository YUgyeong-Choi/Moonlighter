#pragma once
#include "CObject.h"
class CPlayer :public CObject
{
public:
	enum STATE { IDLE, WALK, ATTACK, HIT, DEAD, END };
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
	void Offset();
	void Change_Motion();
private:
	bool m_bIsRoll;
	STATE               m_eCurState;
	STATE               m_ePreState;
};

