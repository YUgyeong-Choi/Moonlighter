#pragma once
#include "CMapObj.h"
class CScrollWasd : public CMapObj
{
public:
	CScrollWasd();
	virtual ~CScrollWasd() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
private:
	bool m_bIsOpen;
};

