#pragma once
#include "CObject.h"
class CVisitor :public CObject
{
protected:
	enum Pattern { IDLE, WALK, END };
public:
	CVisitor();
	virtual ~CVisitor() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
protected:
	Pattern m_eCurPattern;
	Pattern m_ePrePattern;
};

