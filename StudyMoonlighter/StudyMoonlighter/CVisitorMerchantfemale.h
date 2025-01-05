#pragma once
#include "CVisitor.h"
class CVisitorMerchantfemale :public CVisitor
{
public:
	CVisitorMerchantfemale();
	virtual ~CVisitorMerchantfemale() { Release(); }
public:
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
};

