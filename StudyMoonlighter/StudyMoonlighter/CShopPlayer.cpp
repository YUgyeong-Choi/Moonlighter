#include "pch.h"
#include "CShopPlayer.h"
#include "CObjectManager.h"

CShopPlayer::CShopPlayer():m_iMoney(0)
{
}

void CShopPlayer::Initialize()
{
    m_eOBJID = OBJ_PLAYER;
    m_eRender = RENDER_GAMEOBJECT;
}

int CShopPlayer::Update()
{
    return 0;
}

void CShopPlayer::Late_Update()
{
}

void CShopPlayer::Render(HDC hDC)
{
}

void CShopPlayer::Release()
{
}

void CShopPlayer::Set_State(int hp, int money, int maxHp)
{
    m_iHp = hp;
    m_iMaxHp = maxHp;
    m_iMoney = money;
}
