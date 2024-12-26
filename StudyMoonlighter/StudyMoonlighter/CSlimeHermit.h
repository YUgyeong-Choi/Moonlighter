#pragma once
#include "CMonster.h"
class CSlimeHermit :public CMonster
{
public:
    CSlimeHermit();
    virtual ~CSlimeHermit() { Release(); }
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
private:
    void Check_PlayerIn();
private:
    RECT m_checkCircle;
    bool m_IsAttack;
};

