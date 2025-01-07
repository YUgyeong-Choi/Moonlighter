#pragma once
#include "CObject.h"
class CPotionNpc :public CObject
{
public:
	CPotionNpc();
	virtual ~CPotionNpc() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void OnCollision();
	void Key_Input();
private:
	bool m_bOpen;
	bool m_bCollision;
};

