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
private:
    void Attack();
};

