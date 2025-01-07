#pragma once
#include "CObject.h"
#include "CGolemDeadObj.h"

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
		pObj->Set_Pos(_fX, _fY);
		pObj->Initialize();
		return pObj;
	}

	static CObject* Create(float _fX, float _fY, float _fCX, float _fCY)
	{
		CObject* pObj = new T;
		pObj->Set_Pos(_fX, _fY);
		pObj->Initialize();
		pObj->Set_Size(_fCX, _fCY);
		return pObj;
	}

	static CObject* Create(float _fX, float _fY, DIRECTION _dir)
	{
		CObject* pObj = new T;
		pObj->Set_Pos(_fX, _fY);
		pObj->Initialize();
		pObj->Set_DIR(_dir);
		return pObj;
	}

	static CObject* Create(float _fX, float _fY, DEADOBJ _type)
	{
		CObject* pObj = new T;
		pObj->Set_Pos(_fX, _fY);
		pObj->Initialize();
		static_cast<CGolemDeadObj*>(pObj)->Set_DeadType(_type);
		return pObj;
	}
};

