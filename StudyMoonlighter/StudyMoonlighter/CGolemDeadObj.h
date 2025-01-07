#pragma once
#include "CObject.h"
class CGolemDeadObj :public CObject
{
public:
	CGolemDeadObj();
	virtual ~CGolemDeadObj() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void Set_DeadType(DEADOBJ _type) { m_eType = _type; }
private:
	DEADOBJ m_eType;
};

