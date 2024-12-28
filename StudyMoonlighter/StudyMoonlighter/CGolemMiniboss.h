#pragma once
#include "CMonster.h"
class CGolemMiniboss :public CMonster
{
private:
    enum GolemMiniPattern{IDLE, MAZE, SWORD, END};
public:
    CGolemMiniboss();
    virtual ~CGolemMiniboss() { Release(); }
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
    void RenderHpUi(HDC _hDC);
    void OnCollision();
private:
    void Change_Motion();
    void SwordAttack();
    void MazeAttack();
private:
    GolemMiniPattern m_eCurPatten;
    GolemMiniPattern m_ePrePatten;
    int m_iAttackTick;
    int m_iAttackMaze;
    bool m_IsAttack;
    int m_radius;
};

