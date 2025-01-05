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
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
	virtual void Change_Motion();
	virtual void OnCollision(CObject* _obj);
	void Set_Init(float _x, float _y, ITEM _item, int _price);
	void Move_Npc();

	void Render_Talk(HDC hDC);
protected:
	Pattern m_eCurPattern;
	Pattern m_ePrePattern;
	float m_fSellX;
	float m_fSellY;
	float m_fCounterX;
	float m_fCounterY;
	bool m_bSellXY;
	ITEM m_item;
	int price;
	int tick;
};

