#include "pch.h"
#include "CGolemBossRock.h"
#include "CScrollManager.h"

CGolemBossRock::CGolemBossRock()
{
}

void CGolemBossRock::Initialize()
{
    m_tInfo.fCX = 10.f;
    m_tInfo.fCY = 10.f;
    m_tRemoveTime = GetTickCount64();

    m_eOBJID = OBJ_MAPOBJ;
    m_eRender = RENDER_GAMEOBJECT;
}

int CGolemBossRock::Update()
{
    if (m_tRemoveTime + 10000 < GetTickCount64()) {
        return OBJ_DEAD;
    }
    return 0;
}

void CGolemBossRock::Late_Update()
{
}

void CGolemBossRock::Render(HDC hDC)
{
    int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
    int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

    Ellipse(hDC, m_tInfo.fX - 10+ iScrollX, m_tInfo.fY - 10+ iScrollY, m_tInfo.fX + 10+ iScrollX, m_tInfo.fY + 10+ iScrollY);
}

void CGolemBossRock::Release()
{
}

void CGolemBossRock::OnCollision(CObject* _obj)
{
}
