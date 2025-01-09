#pragma once
#include "CScene.h"
class CStartAniScene :public CScene
{
private:
	enum STARTANITYPE{HOUSE, TABLE, STARTEND};
public:
	CStartAniScene();
	virtual ~CStartAniScene() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC hDC) override;
	void Release() override;
	void Key_Input() override;
	void Create_MapObj() override;
	void Offset() override;
private:
	int index;
	STARTANITYPE	m_eCurType;
	STARTANITYPE	m_ePreType;
	FRAME m_tFrame;
	int alpha;
	bool m_paid;
};

