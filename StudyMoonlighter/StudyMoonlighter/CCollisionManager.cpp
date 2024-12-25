#include "pch.h"
#include "CCollisionManager.h"
#include "CPlayer.h"
#include "CGolemBreakable.h"
#include "CGolemPunch.h"

void CCollisionManager::CollisionRect(list<CObject*> _Dst, list<CObject*> _Src)
{
    RECT rc{};

    for (auto& Dst : _Dst)
    {
        for (auto& Src : _Src)
        {
            if (IntersectRect(&rc, Dst->Get_Rect(), Src->Get_Rect()))
            {
                Dst->OnCollision(Src);
                Src->OnCollision(Dst);
            }
        }
    }
}

void CCollisionManager::CollisionRectWeapon(list<CObject*> _Dst, list<CObject*> _Src)
{
    RECT rc{};

    for (auto& Dst : _Dst)
    {
        for (auto& Src : _Src)
        {
            if (IntersectRect(&rc, Dst->Get_HitBox(), Src->Get_Rect()))
            {
                Src->OnCollision(Dst);
            }
        }
    }
}

void CCollisionManager::CollisionCircle(list<CObject*> _Dst, list<CObject*> _Src)
{
    RECT rc{};

    for (auto& Dst : _Dst)
    {
        for (auto& Src : _Src)
        {
            if (static_cast<CPlayer*>(Src)->Get_IsRolling()) {
                continue;
            }

            if (CheckCircle(Dst, Src))
            {
                Dst->OnCollision(Src);
                Src->OnCollision(Dst);
            }
        }
    }
}

bool CCollisionManager::CheckCircle(CObject* _Dst, CObject* _Src)
{
	float fRadius = (_Dst->Get_Info().fCX + _Src->Get_Info().fCX) * 0.5f;

	float fWidth = abs(_Dst->Get_Info().fX - _Src->Get_Info().fX);
	float fHeight = abs(_Dst->Get_Info().fY - _Src->Get_Info().fY);

	float fDiagonal = powf((fWidth * fWidth + fHeight * fHeight), 0.5f);

	return fRadius >= fDiagonal;
}

void CCollisionManager::CollisionRectEx(list<CObject*> _Dst, list<CObject*> _Src)
{
    float fX(0.f), fY(0.f);

    for (auto& Dst : _Dst)
    {
        for (auto& Src : _Src)
        {
            if (CheckRect(Dst, Src, &fX, &fY))
            {
                if (static_cast<CPlayer*>(Dst)->Get_IsRolling()) {
                    continue;
                }

                if (dynamic_cast<CGolemPunch*>(Src)) {
                    continue;
                }

                Dst->OnCollision(Src);

                if (fX > fY)
                {
                    if (Dst->Get_Info().fY < Src->Get_Info().fY)
                    {
                        Dst->Set_Pos(Dst->Get_Info().fX, Dst->Get_Info().fY - fY - Dst->Get_Speed());
                    }
                    else
                    {
                        Dst->Set_Pos(Dst->Get_Info().fX, Dst->Get_Info().fY + fY + Dst->Get_Speed());
                    }
                }
                else
                {
                    if (Dst->Get_Info().fX < Src->Get_Info().fX)
                    {
                        Dst->Set_Pos(Dst->Get_Info().fX - fX - Dst->Get_Speed(), Dst->Get_Info().fY);
                    }
                    else
                    {
                        Dst->Set_Pos(Dst->Get_Info().fX + fX + Dst->Get_Speed(), Dst->Get_Info().fY);
                    }
                }
            }
        }
    }
}

void CCollisionManager::CollisionRectExMapObj(list<CObject*> _Dst, list<CObject*> _Src)
{
    float fX(0.f), fY(0.f);

    for (auto& Dst : _Dst)
    {
        for (auto& Src : _Src)
        {
            if (CheckRect(Dst, Src, &fX, &fY))
            {
                if (dynamic_cast<CGolemBreakable*>(Src)) {
                    if (!dynamic_cast<CGolemBreakable*>(Src)->Get_Breakable()) {
                        return;
                    }
                }
                else {
                    Dst->OnCollision(Src);
                }


                if (fX > fY)
                {
                    if (Dst->Get_Info().fY < Src->Get_Info().fY)
                    {
                        Dst->Set_Pos(Dst->Get_Info().fX, Dst->Get_Info().fY - fY - Dst->Get_Speed());
                    }
                    else
                    {
                        Dst->Set_Pos(Dst->Get_Info().fX, Dst->Get_Info().fY + fY + Dst->Get_Speed());
                    }
                }
                else
                {
                    if (Dst->Get_Info().fX < Src->Get_Info().fX)
                    {
                        Dst->Set_Pos(Dst->Get_Info().fX - fX - Dst->Get_Speed(), Dst->Get_Info().fY);
                    }
                    else
                    {
                        Dst->Set_Pos(Dst->Get_Info().fX + fX + Dst->Get_Speed(), Dst->Get_Info().fY);
                    }
                }
            }
        }
    }
}

void CCollisionManager::CollisionFloor(list<CObject*> _Dst, list<CObject*> _Src)
{
    float fX(0.f), fY(0.f);

    for (auto& Dst : _Dst)
    {
        for (auto& Src : _Src)
        {
            if (CheckRect(Dst, Src, &fX, &fY))
            {
                //구르는 상태라면 continue
                if (static_cast<CPlayer*>(Dst)->Get_IsRolling()) {
                    continue;
                }

                //걸어서 충돌했을 때와 낭떨어지 가운데쯤에 있을 때 분리
                if (fX > fY)
                {
                    if (Dst->Get_Info().fY < Src->Get_Info().fY)
                    {
                        Dst->Set_Pos(Dst->Get_Info().fX, Dst->Get_Info().fY - fY - Dst->Get_Speed());
                    }
                    else
                    {
                        Dst->Set_Pos(Dst->Get_Info().fX, Dst->Get_Info().fY + fY + Dst->Get_Speed());
                    }
                }
                else
                {
                    //좌충돌
                    if (Dst->Get_Info().fX < Src->Get_Info().fX)
                    {
                        if (Dst->Get_Info().fX < Src->Get_Info().fX - (Src->Get_Info().fCX * 0.5f) + 3.f) {
                            Dst->Set_Pos(Dst->Get_Info().fX - fX - Dst->Get_Speed(), Dst->Get_Info().fY);
                        }
                        else {
                            Dst->OnCollision(Src);
                            Src->OnCollision(Dst);
                        }
                    }
                    else
                    {
                        if (Dst->Get_Info().fX > Src->Get_Info().fX + (Src->Get_Info().fCX * 0.5f) - 3.f) {
                            Dst->Set_Pos(Dst->Get_Info().fX + fX + Dst->Get_Speed(), Dst->Get_Info().fY);
                        }
                        else {
                            Dst->OnCollision(Src);
                            Src->OnCollision(Dst);
                        }
                    }
                }
            }
        }
    }
}

bool CCollisionManager::CheckRect(CObject* _Dst, CObject* _Src, float* pX, float* pY)
{
	float		fX = abs(_Dst->Get_Info().fX - _Src->Get_Info().fX);
	float		fY = abs(_Dst->Get_Info().fY - _Src->Get_Info().fY);

	float		fRadiusX = (_Dst->Get_Info().fCX + _Src->Get_Info().fCX) * 0.5f;
	float		fRadiusY = (_Dst->Get_Info().fCY + _Src->Get_Info().fCY) * 0.5f;

	if ((fRadiusX >= fX) && (fRadiusY >= fY))
	{
		*pX = fRadiusX - fX;
		*pY = fRadiusY - fY;
		return true;
	}

	return false;
}