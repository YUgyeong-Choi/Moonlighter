#pragma once
#include "CMonster.h"
class CGolemBoss : public CMonster
{
private:
    enum GolemBossPattern { WAKEUP, IDLE, IDLENOARM, SHOOTPRE, SHOOTIDLE, SHOOTEND, SPAWNCIRCLE, PUNCHARM, RECOVERARM, SPAWNRANDOM, DEATH, NONE };
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
    void SpawnMoney(float radius, int numRocks);
    void Shoot();
    void OnCollision();
private:
    bool m_IsWake;
    GolemBossPattern m_eCurPattern;
    GolemBossPattern m_ePrePattern;
    vector<GolemBossPattern> m_Pattern = {
    IDLE,IDLE,SPAWNCIRCLE,IDLE, IDLE, PUNCHARM, IDLE, IDLE, SPAWNRANDOM, IDLE, SHOOTPRE,IDLE,IDLE,IDLE,IDLE,IDLE
    };
    int m_PatternIndex;
    float m_preFrame;
    bool m_Shoot;
    int tick;
    float m_fAngle;
    MONSTERTYPE m_monster;

    int m_hitBoxX;
    int m_hitBoxY;
    int removeTick;
    RECT m_MonsterHitox;
    int count;
    bool m_bDead;
    bool m_bSpawn;
};

