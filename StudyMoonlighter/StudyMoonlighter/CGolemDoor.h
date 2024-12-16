#pragma once
#include "CMapObj.h"
class CGolemDoor :public CMapObj
{
public:
	CGolemDoor();
	virtual ~CGolemDoor() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void Set_IsOpened(bool _b) { m_bAlreadyOpen = _b; }
	void Set_DIR(DIRECTION _dir) { m_eDir = _dir; }
	DIRECTION Get_DIR() { return m_eDir; }
private:
	bool m_bIsOpen;
	bool m_bAlreadyOpen;
	DIRECTION m_eDir;
};

