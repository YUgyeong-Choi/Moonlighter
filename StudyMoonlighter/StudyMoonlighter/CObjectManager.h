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
	CObject* Get_LastMapObj() { return m_ObjList[OBJ_MAPOBJ].back(); }
	CObject* Get_LastFloor() { return m_ObjList[OBJ_FLOOR].back(); }
	CObject* Get_Player() { return m_ObjList[OBJ_PLAYER].back(); }
	list<CObject*> Get_MapObjList(){ return m_ObjList[OBJ_MAPOBJ]; }
	list<CObject*> Get_PortalList(){ return m_ObjList[OBJ_PORTAL]; }
	list<CObject*> Get_FloorList(){ return m_ObjList[OBJ_FLOOR]; }
	int Get_MonsterSize() { return (int)(m_ObjList[OBJ_MONSTER].size()); }
	bool Get_IsMapMove() { return m_IsMapMove; }
	void Set_IsMapMove(bool _b) { m_IsMapMove = _b; }
	int Get_MapMoveX() { return m_iMapMoveX; }
	void Set_MapMoveX(int _i) { m_iMapMoveX = _i; }

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
	bool m_IsMapMove;
	int m_iMapMoveX;
};

