#pragma once
#include "CMonster.h"
class CGolemHead :public CMonster
{
public:
    CGolemHead();
    virtual ~CGolemHead() { Release(); }
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
    void OnCollision(CObject* _obj);
    void OnCollision();
private:
    float m_fAngle;
    int m_AttackCount;
    bool m_IsAttack;
};

