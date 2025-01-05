#pragma once
#include "CVisitor.h"
class CVisitorOldfemale :public CVisitor
{
public:
	CVisitorOldfemale();
	virtual ~CVisitorOldfemale() { Release(); }
public:
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
};

