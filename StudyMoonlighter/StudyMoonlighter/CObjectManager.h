#pragma once
#include "CObject.h"
class CObjectManager
{
private:
	CObjectManager();
	~CObjectManager() { Release(); }
public:
	void		Add_Object(OBJID eID, CObject* pObj);
	int			Update();
	void		Late_Update();
	void		Render(HDC hDC);
	void		Release();

	void		Delete_ID(OBJID eID);
	void		RenderListClear()
	{
		for (size_t i = 0; i < RENDER_END; ++i)
		{
			m_RenderList[i].clear();
		}
	}
	CObject* Get_LastPortal() { return m_ObjList[OBJ_PORTAL].back(); }
	CObject* Get_Player() { return m_ObjList[OBJ_PLAYER].back(); }
	list<CObject*> Get_MapObjList(){ return m_ObjList[OBJ_MAPOBJ]; }

public:
	static CObjectManager* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CObjectManager;

		return m_pInstance;
	}

	static void		DestroyInstance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
private:
	static CObjectManager* m_pInstance;
	list<CObject*>		m_ObjList[OBJ_END];
	list<CObject*>		m_RenderList[RENDER_END];
};

