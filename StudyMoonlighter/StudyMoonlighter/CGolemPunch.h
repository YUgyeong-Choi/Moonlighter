#pragma once
#include "CMonster.h"
class CGolemPunch : public CMonster
{
public:
    CGolemPunch();
    virtual ~CGolemPunch() { Release(); }
public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
    void OnCollision(CObject* _obj);
public:
    int Get_PunchTime() { return m_iPunchTime; }
private:
    int m_iSize;
    bool m_bCanDown;
    bool m_bCanUp;
    float m_PunchY;
    float m_OriginPunchY;
    int m_iPunchTime;
};

