#pragma once
#include "CMapObj.h"
class CGolemHole:public CMapObj
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
public:
	HOLETYPE Get_HoleType() { return m_type; }
	void Set_HoleType(HOLETYPE _type) { m_type = _type; }
private:
	HOLETYPE m_type;
};

