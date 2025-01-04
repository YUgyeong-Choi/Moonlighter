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
	void		OnCollision(CObject* _obj);
private:
	void KeyInput();
private:
	PORTALTYPE m_ePortalType;
	bool m_bEnter;
	bool m_bCollision;
	int alpha;
};

