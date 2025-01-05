#pragma once
#include "CVisitor.h"
class CVisitorIrish :public CVisitor
{
public:
	CVisitorIrish();
	virtual ~CVisitorIrish() { Release(); }
public:
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
};

