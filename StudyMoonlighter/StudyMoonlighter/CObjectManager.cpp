#include "pch.h"
#include "CObjectManager.h"
#include "CCollisionManager.h"


CObjectManager* CObjectManager::m_pInstance = nullptr;

CObjectManager::CObjectManager():m_iMapXIndex(0), m_iMapYIndex(0), m_dir(DIR_END)
{
}

void CObjectManager::Add_Object(OBJID eID, CObject* pObj)
{
	if (OBJ_END <= eID || nullptr == pObj)
		return;

	m_ObjList[eID].push_back(pObj);
}

int CObjectManager::Update()
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto iter = m_ObjList[i].begin();iter != m_ObjList[i].end(); )
		{
			int iResult = (*iter)->Update();

			if (OBJ_DEAD == iResult)
			{
				Safe_Delete<CObject*>(*iter);
				iter = m_ObjList[i].erase(iter);
			}
			else
				++iter;
		}
	}
	return 0;
}

void CObjectManager::Late_Update()
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto& pObj : m_ObjList[i])
		{
			pObj->Late_Update();

			if (m_ObjList[i].empty())
				break;

			RENDERID	eID = pObj->Get_GroupID();
			m_RenderList[eID].push_back(pObj);
		}
	}

	CCollisionManager::CollisionRectExMapObj(m_ObjList[OBJ_PLAYER], m_ObjList[OBJ_MAPOBJ]); // �÷��̾� ��ü �浹
	CCollisionManager::CollisionRectExMapObj(m_ObjList[OBJ_MONSTER], m_ObjList[OBJ_MAPOBJ]); // ���� ��ü �浹
	CCollisionManager::CollisionFloor(m_ObjList[OBJ_MONSTER], m_ObjList[OBJ_FLOOR]); //���� �������� �浹
	CCollisionManager::CollisionFloor(m_ObjList[OBJ_PLAYER], m_ObjList[OBJ_FLOOR]); //�÷��̾� �������� �浹
	CCollisionManager::CollisionRect(m_ObjList[OBJ_PLAYER], m_ObjList[OBJ_PORTAL]); //�÷��̾� ��Ż �浹
	CCollisionManager::CollisionRect(m_ObjList[OBJ_MONSTER_BULLET], m_ObjList[OBJ_MAPOBJ]); //���� �Ѿ� ���� �浹
	CCollisionManager::CollisionRect(m_ObjList[OBJ_PLAYER], m_ObjList[OBJ_MONSTER_BULLET]); //�÷��̾� ���� �Ѿ� �浹
	CCollisionManager::CollisionRect(m_ObjList[OBJ_PLAYER], m_ObjList[OBJ_MONSTER]); //�÷��̾� ���� �浹
	CCollisionManager::CollisionRectWeapon(m_ObjList[OBJ_MONSTER], m_ObjList[OBJ_PLAYER]); //���� ���� �÷��̾� �浹
	CCollisionManager::CollisionRectWeapon(m_ObjList[OBJ_PLAYER], m_ObjList[OBJ_MAPOBJ]); //�÷��̾� ���� �ν����°� �浹
	CCollisionManager::CollisionRect(m_ObjList[OBJ_PLAYER], m_ObjList[OBJ_SLIME]);
	CCollisionManager::CollisionRect(m_ObjList[OBJ_PLAYER], m_ObjList[OBJ_ITEM]);
	CCollisionManager::CollisionRect(m_ObjList[OBJ_NPC], m_ObjList[OBJ_MAPOBJ]);
}

void CObjectManager::Render(HDC hDC)
{
	for (size_t i = 0; i < RENDER_END; ++i)
	{
		m_RenderList[i].sort([](CObject* pDst, CObject* pSrc)->bool
			{
				return pDst->Get_Info().fY < pSrc->Get_Info().fY;
			});

		for (auto& pObj : m_RenderList[i])
			pObj->Render(hDC);

		m_RenderList[i].clear();
	}
}

void CObjectManager::Release()
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for_each(m_ObjList[i].begin(), m_ObjList[i].end(), Safe_Delete<CObject*>);
		m_ObjList[i].clear();
	}
}

void CObjectManager::Delete_ID(OBJID eID)
{
	for (auto& pObj : m_ObjList[eID])
		Safe_Delete(pObj);

	m_ObjList[eID].clear();
}

void CObjectManager::Delete_ALL()
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto& pObj : m_ObjList[i])
			Safe_Delete(pObj);

		m_ObjList[i].clear();
	}

}

void CObjectManager::Delete_ALLNotPlayer()
{
	for (size_t i = 1; i < OBJ_END; ++i)
	{
		for (auto& pObj : m_ObjList[i])
			Safe_Delete(pObj);

		m_ObjList[i].clear();
	}

	RenderListClear();
}
