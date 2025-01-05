#pragma once
#include "CObject.h"
class CVisitor :public CObject
{
protected:
	enum Pattern { IDLE, WALK, END };
	enum ITEMSTATE {CHEAP, GOOD, BAD, ITEMSTATE_END};
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
	void Set_Init(int index, float _outx, float _outy, float _x, float _y, ITEM _item, int _price);
	void Move_Npc();

	void Render_Talk(HDC hDC);
protected:
	int m_index;
	Pattern m_eCurPattern;
	Pattern m_ePrePattern;
	float m_fOutX;
	float m_fOutY;
	float m_fSellX;
	float m_fSellY;
	float m_fCounterX;
	float m_fCounterY;
	float m_fBeforeCounterX;
	float m_fBeforeCounterY;
	bool m_bSellXY;
	bool m_itemTalk;
	bool m_bTableXY;
	bool m_bBeforeTableXY;
	bool m_SellFinish;
	ITEM m_item;
	int price;
	int tick;
	ITEMSTATE m_itemState;
	FRAME m_itemFrame;
	bool m_AddMoney;
	bool m_sound;
};

