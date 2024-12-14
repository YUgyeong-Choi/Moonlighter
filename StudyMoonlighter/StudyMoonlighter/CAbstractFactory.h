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

	static CObject* Create(float _fX, float _fY)
	{
		CObject* pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(_fX, _fY);
		return pObj;
	}

	static CObject* Create(float _fX, float _fY, float _fCX, float _fCY)
	{
		CObject* pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(_fX, _fY);
		pObj->Set_Size(_fCX, _fCY);
		return pObj;
	}
};

