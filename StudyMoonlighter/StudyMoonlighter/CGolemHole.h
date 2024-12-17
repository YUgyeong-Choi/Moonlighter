#pragma once
#include "CObject.h"
class CGolemHole :public CObject
{
public:
	CGolemHole();
	virtual ~CGolemHole() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

	void Set_HoleType(HOLETYPE _type) { m_type = _type; }
	HOLETYPE Get_HoleType() { return m_type; }
private:
	HOLETYPE m_type;
	bool m_IsTop;
	bool m_FreeLeft;
	bool m_FreeRight;
};

