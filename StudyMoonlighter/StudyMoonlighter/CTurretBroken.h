#pragma once
#include "CMonster.h"
class CTurretBroken :public CMonster
{
public:
    CTurretBroken();
    virtual ~CTurretBroken() { Release(); }
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

private:
    void Shoot();
};

