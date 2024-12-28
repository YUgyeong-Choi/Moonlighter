#pragma once
#include "CMonster.h"
class CBabySlime :public CMonster
{
public:
    CBabySlime();
    virtual ~CBabySlime() { Release(); }
public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
    void OnCollision();
    
};

