#pragma once
#include "CVisitor.h"
class CVisitorMale :public CVisitor
{
public:
	CVisitorMale();
	virtual ~CVisitorMale() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void TarGetPosition();
private:
	void Change_Motion();
	bool m_bYArrive;
	bool m_bXArrive;
	int charIndex;
	int tick;
	TCHAR text[64];
	TCHAR name[64];
	
};

