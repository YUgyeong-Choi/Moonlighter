#include "pch.h"
#include "CCollisionManager.h"
#include "CPlayer.h"

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
void CCollisionManager::CollisionCircle(list<CObject*> _Dst, list<CObject*> _Src)
{
    RECT rc{};

    for (auto& Dst : _Dst)
    {
        for (auto& Src : _Src)
        {
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
                Dst->OnCollision(Src);
                Src->OnCollision(Dst);
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
                if (static_cast<CPlayer*>(Dst)->Get_IsRolling()) {
                    continue;
                }
                Dst->OnCollision(Src);
                Src->OnCollision(Dst);
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