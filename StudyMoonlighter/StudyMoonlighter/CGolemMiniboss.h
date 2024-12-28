#pragma once
#include "CMonster.h"
class CGolemMiniboss :public CMonster
{
public:
    CGolemMiniboss();
    virtual ~CGolemMiniboss() { Release(); }
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
};

