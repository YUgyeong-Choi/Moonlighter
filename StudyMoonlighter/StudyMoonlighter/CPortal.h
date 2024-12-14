#pragma once
#include "CObject.h"
class CPortal :public CObject
{
public:
	CPortal();
	virtual ~CPortal() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void Set_PortalType(PORTALTYPE _type) { m_ePortalType = _type; }
	void		OnCollisionEnter(CObject* _obj);
private:
	PORTALTYPE m_ePortalType;
};

