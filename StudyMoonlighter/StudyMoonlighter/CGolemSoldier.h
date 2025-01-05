#pragma once
#include "CMonster.h"
class CGolemSoldier :public CMonster
{
public:
    CGolemSoldier();
    virtual ~CGolemSoldier() { Release(); }
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
    void OnCollision();
    void OnCollision(CObject* _obj);
private:
    void Check_PlayerIn();
private:
    RECT m_checkCircle;
    bool m_IsAttack;
};

