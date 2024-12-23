#pragma once
#include "CMonster.h"
class CGolemBoss : public CMonster
{
private:
    enum GolemBossPattern { WAKEUP, IDLE, IDLENOARM, SPAWN, DEATH, NONE };
public:
    CGolemBoss();
    virtual ~CGolemBoss() { Release(); }
public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
    void OnCollision(CObject* _obj);
private:
    void Change_Frame();
private:
    float m_fPatternCool;
    bool m_IsWake;
    GolemBossPattern m_eCurPattern;
    GolemBossPattern m_ePrePattern;
};

