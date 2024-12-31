#pragma once
#include "CObject.h"
class CItem : public CObject
{
public:
	CItem();
	virtual ~CItem() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void OnCollision(CObject* _obj);
	void Set_ItemType(ITEMTYPE _type);
	ITEMTYPE Get_ItemType() { return m_type; }
private:
	void DetectedPlayer();
	void OnCollision();
private:
	ITEMTYPE m_type;
	const TCHAR* pImageUrl;
};

