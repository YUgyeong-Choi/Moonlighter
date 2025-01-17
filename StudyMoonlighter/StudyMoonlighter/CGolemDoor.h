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
	void OnCollision(CObject* _obj) override;

public:
	void Set_IsOpened(bool _b) { m_bAlreadyOpen = _b; }
	bool Get_IsOpened() { return m_bAlreadyOpen; }
	bool Get_Load_Next() { return m_bLoad_Next; }
private:

	bool m_bIsOpen;
	bool m_bAlreadyOpen;
	bool m_bLoad_Next;
};

