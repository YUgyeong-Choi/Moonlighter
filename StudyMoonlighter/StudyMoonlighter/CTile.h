#pragma once
#include "CObject.h"
class CTile: public CObject
{
public:
	CTile();
	virtual ~CTile() { Release(); }
public:
	bool			Get_DrawID() { return m_bIsWalk; }
	void			Set_DrawID(bool _b) { m_bIsWalk = _b; }

	void		Set_DrawIDPick() {m_bIsWalk = !m_bIsWalk; }

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

private:
	bool		m_bIsWalk; // 0�̸� �ɾ �Ǵ� �� 1�̸� ������ �ȵǴ� ��
};

