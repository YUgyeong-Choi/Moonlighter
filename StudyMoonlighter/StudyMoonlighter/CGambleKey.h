#pragma once
#include "CObject.h"
class CGambleKey :public CObject
{
private:
	enum FRUITTYPE{CHERRY, LEMON, STRAW,GRAPE,WATER, BANANA, ORANGE };
public:
	CGambleKey();
	virtual ~CGambleKey() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void Set_GambleStart(bool _b) { m_bActive = _b; }
	void InitGame();
	void KeyInput();
	void CheckWin();
	Image* LoadImageForFruit(FRUITTYPE fruitType);
private:
	bool m_bActive;
	int y;
	vector<FRUITTYPE> m_firstFruit;
	vector<FRUITTYPE> m_secondFruit;
	vector<FRUITTYPE> m_thridFruit;
	int index;
	bool m_first;
	bool m_second;
	bool m_third;
	int m_firstIndex;
	int m_secondIndex;
	int m_thirdIndex;
	bool m_finish;
	bool m_Win;
	int m_goldDown;
};

