#pragma once
#include "CMonster.h"
class CGolemBoss : public CMonster
{
private:
    enum GolemBossPattern { WAKEUP, IDLE, IDLENOARM, WAVE, SPAWNCIRCLE, PUNCHARM, RECOVERARM, SPAWNRANDOM, DEATH, NONE };
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
    void SpawnRockCircle(float radius, int numRocks);
    void SpawnRockRandom(int numRocks);
private:
    ULONGLONG m_fPatternCool;
    bool m_IsWake;
    GolemBossPattern m_eCurPattern;
    GolemBossPattern m_ePrePattern;
    vector<GolemBossPattern> m_Pattern = {
    IDLE,IDLE,SPAWNCIRCLE,IDLE, IDLE, SPAWNRANDOM, IDLE, IDLE
    };
    int m_PatternIndex;
    float m_preFrame;
};

