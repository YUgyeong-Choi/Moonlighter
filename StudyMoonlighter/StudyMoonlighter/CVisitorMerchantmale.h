#pragma once
#include "CVisitor.h"
class CVisitorMerchantmale :public CVisitor
{
public:
	CVisitorMerchantmale();
	virtual ~CVisitorMerchantmale() { Release(); }
public:
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
};

