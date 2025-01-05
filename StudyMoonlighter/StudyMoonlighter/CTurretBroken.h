#pragma once
#include "CMonster.h"
class CTurretBroken :public CMonster
{
public:
    CTurretBroken();
    virtual ~CTurretBroken() { Release(); }
public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
    void OnCollision();
    void OnCollision(CObject* _obj);
private:
    void Shoot();
};

