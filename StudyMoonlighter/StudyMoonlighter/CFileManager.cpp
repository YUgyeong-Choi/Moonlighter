#include "pch.h"
#include "CFileManager.h"
#include "CObjectManager.h"
#include "CMapObj.h"
#include "CScrollRoll.h"
#include "CGolemHole.h"
#include "CTurretBroken.h"
#include "CAbstractFactory.h"
#include "CCollisionBox.h"
#include "CScrollWasd.h"
#include "CGolemDoor.h"
#include "CBabySlime.h"
#include "CGolemBreakable.h"

CFileManager* CFileManager::m_pInstance = nullptr;

void CFileManager::Save_MapObj(const TCHAR* _pFilePath)
{
	HANDLE hFile = CreateFile(_pFilePath, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD	dwByte(0);
	
	list<CObject*> mapObjList = CObjectManager::Get_Instance()->Get_MapObjList(); //맵 관련
	for (auto& mapObj : mapObjList)
	{
		CMapObj* obj = static_cast<CMapObj*>(mapObj);
		WriteFile(hFile, obj, sizeof(CMapObj), &dwByte, NULL);

		if (dynamic_cast<CMapObj*>(mapObj)->Get_MapObjType() == SCROLLROLL) {
			TUTORIALSCROLL _type;
			_type = static_cast<CScrollRoll*>(mapObj)->Get_ScrollType();
			WriteFile(hFile, &_type, sizeof(TUTORIALSCROLL), &dwByte, NULL);
		}
	}

	list<CObject*> protalList = CObjectManager::Get_Instance()->Get_PortalList(); //이동 관련
	for (auto& mapObj : protalList)
	{
		DIRECTION _dir;
		_dir = mapObj->Get_DIR();
		CMapObj* obj = static_cast<CMapObj*>(mapObj);
		WriteFile(hFile, mapObj, sizeof(CMapObj), &dwByte, NULL);
		WriteFile(hFile, &_dir, sizeof(DIRECTION), &dwByte, NULL);
		bool _type = static_cast<CGolemDoor*>(mapObj)->Get_IsOpened();
		WriteFile(hFile, &_type, sizeof(bool), &dwByte, NULL);
	}

	list<CObject*> floorList = CObjectManager::Get_Instance()->Get_FloorList(); //이동 관련 (던전)
	for (auto& mapObj : floorList)
	{
		HOLETYPE _type = dynamic_cast<CGolemHole*>(mapObj)->Get_HoleType();
		CMapObj* obj = static_cast<CMapObj*>(mapObj);
		WriteFile(hFile, obj, sizeof(CMapObj), &dwByte, NULL);
		WriteFile(hFile, &_type, sizeof(HOLETYPE), &dwByte, NULL);
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"MapObj Save", L"성공", MB_OK);
}

void CFileManager::Save_MonsterObj(const TCHAR* _pFilePath)
{
	HANDLE hFile = CreateFile(_pFilePath, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD	dwByte(0);

	list<CObject*> monsterList = CObjectManager::Get_Instance()->Get_MonsterList(); //몬스터 관련
	for (auto& monster : monsterList)
	{
		if (dynamic_cast<CTurretBroken*>(monster)) {
			DIRECTION _dir = monster->Get_DIR();
			CMonster* obj = static_cast<CMonster*>(monster);
			WriteFile(hFile, obj, sizeof(CMonster), &dwByte, NULL);
			WriteFile(hFile, &_dir, sizeof(DIRECTION), &dwByte, NULL);
		}
		else {
			CMonster* obj = static_cast<CMonster*>(monster);
			WriteFile(hFile, obj, sizeof(CMonster), &dwByte, NULL);
		}
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"Monster Save", L"성공", MB_OK);
}

void CFileManager::Load_MapObjFile(const TCHAR* _pFilePath, int _y)
{
	HANDLE hFile = CreateFile(_pFilePath, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD	dwByte(0);
	CMapObj	_MapObj;

	while (true)
	{
		bool a = ReadFile(hFile, &_MapObj, sizeof(CMapObj), &dwByte, NULL);

		if (0 == dwByte)
			break;

		if (_MapObj.Get_MapObjType() == COLLISION) {
			CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CCollisionBox>::Create((1024 * _y) + _MapObj.Get_Info().fX, _MapObj.Get_Info().fY, _MapObj.Get_Info().fCX, _MapObj.Get_Info().fCY));
		}
		else if (_MapObj.Get_MapObjType() == SCROLLWASD) {
			CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CScrollWasd>::Create((1024* _y)+_MapObj.Get_Info().fX, _MapObj.Get_Info().fY, 0, 0));
		}
		else if (_MapObj.Get_MapObjType() == SCROLLROLL) {
			TUTORIALSCROLL _type;
			bool b = ReadFile(hFile, &_type, sizeof(TUTORIALSCROLL), &dwByte, NULL);
			CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CScrollRoll>::Create((1024 * _y)+_MapObj.Get_Info().fX, _MapObj.Get_Info().fY, 0, 0));
			static_cast<CScrollRoll*>(CObjectManager::Get_Instance()->Get_LastMapObj())->Set_ScrollType(_type);
		}
		else if (_MapObj.Get_MapObjType() == GOLEM_DOOR) {
			DIRECTION _dir;
			bool b = ReadFile(hFile, &_dir, sizeof(DIRECTION), &dwByte, NULL);
			CObjectManager::Get_Instance()->Add_Object(OBJ_PORTAL, CAbstractFactory<CGolemDoor>::Create((1024 * _y)+_MapObj.Get_Info().fX, _MapObj.Get_Info().fY, _MapObj.Get_Info().fCX, _MapObj.Get_Info().fCY));
			CObjectManager::Get_Instance()->Get_LastPortal()->Set_DIR(_dir);
			bool _type;
			b = ReadFile(hFile, &_type, sizeof(bool), &dwByte, NULL);
			static_cast<CGolemDoor*>(CObjectManager::Get_Instance()->Get_LastPortal())->Set_IsOpened(_type);
		}
		else if (_MapObj.Get_MapObjType() == GOLEM_HOLE) {
			HOLETYPE _type;
			bool b = ReadFile(hFile, &_type, sizeof(HOLETYPE), &dwByte, NULL);
			CObjectManager::Get_Instance()->Add_Object(OBJ_FLOOR, CAbstractFactory<CGolemHole>::Create((1024 * _y)+_MapObj.Get_Info().fX, _MapObj.Get_Info().fY, _MapObj.Get_Info().fCX, _MapObj.Get_Info().fCY));
			dynamic_cast<CGolemHole*>(CObjectManager::Get_Instance()->Get_LastFloor())->Set_HoleType(_type);
		}
		else if (_MapObj.Get_MapObjType() == GOLEM_BROKEN) {
			CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CGolemBreakable>::Create((1024 * _y) + _MapObj.Get_Info().fX, _MapObj.Get_Info().fY));
		}

	}

	CloseHandle(hFile);
}

void CFileManager::Load_MonsterFile(const TCHAR* _pFilePath, int _y)
{
	HANDLE hFile = CreateFile(_pFilePath, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD	dwByte(0);
	CMonster _monsterObj;

	int _x = CObjectManager::Get_Instance()->Get_MapYIndex();

	while (true)
	{
		bool a = ReadFile(hFile, &_monsterObj, sizeof(CMonster), &dwByte, NULL);

		if (0 == dwByte)
			break;

		if (_monsterObj.Get_MonsterType() == TURRETBROKEN) {
			DIRECTION _dir = _monsterObj.Get_DIR();
			bool b = ReadFile(hFile, &_dir, sizeof(DIRECTION), &dwByte, NULL);
			CObjectManager::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CTurretBroken>::Create((1024 * _y)+_monsterObj.Get_Info().fX, _monsterObj.Get_Info().fY, _dir));
		}
		else if (_monsterObj.Get_MonsterType() == BABYSLIME) {
			CObjectManager::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CBabySlime>::Create((1024 * _y) + _monsterObj.Get_Info().fX, _monsterObj.Get_Info().fY));
			static_cast<CBabySlime*>(CObjectManager::Get_Instance()->Get_LastMonster())->Set_TargetObj(CObjectManager::Get_Instance()->Get_Player());
		}

	}

	CloseHandle(hFile);
}
