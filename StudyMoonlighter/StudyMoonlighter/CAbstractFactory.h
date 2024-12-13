#pragma once
#include "CObject.h"

template<typename T>
class CAbstractFactory
{
public:
	CAbstractFactory() {}
	~CAbstractFactory() {};

public:
	static CObject* Create()
	{
		CObject* pObj = new T;
		pObj->Initialize();
		return pObj;
	}

	static CObject* Create(float _fX, float _fY, float _fAngle = 0.f)
	{
		CObject* pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(_fX, _fY);

		if (0.f != _fAngle)
			pObj->Set_Angle(_fAngle);

		return pObj;
	}
};

