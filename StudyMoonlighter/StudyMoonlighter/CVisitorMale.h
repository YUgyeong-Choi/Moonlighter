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
private:
	void KeyInput();
private:
	int charIndex;
	int tick;
	TCHAR text[6][64];
	TCHAR name[64];
	
	int cnt;
	bool talkFinish;
	bool m_Appear;
	bool m_dead;
};

