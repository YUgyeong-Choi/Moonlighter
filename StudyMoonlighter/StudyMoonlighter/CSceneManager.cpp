#include "pch.h"
#include "CSceneManager.h"
#include "CMenuScene.h"
#include "CVillageScene.h"
#include "CFieldScene.h"
#include "CDungeonScene.h"
#include "CTutorialScene.h"

CSceneManager* CSceneManager::m_pInstance = nullptr;

CSceneManager::CSceneManager(): m_pScene(nullptr), m_eCurScene(SC_MENU), m_ePreScene(SC_END)
{
}

void CSceneManager::SetScene(SCENEID eID)
{
	m_eCurScene = eID;

	if (m_ePreScene != m_eCurScene)
	{
		Safe_Delete(m_pScene);

		switch (m_eCurScene)
		{
		case SC_MENU:
			m_pScene = new CMenuScene;
			break;
		case SC_VILLAGE:
			m_pScene = new CVillageScene;
			break;
		case SC_FIELD:
			m_pScene = new CFieldScene;
			break;
		case SC_DUNGEON:
			m_pScene = new CDungeonScene;
			break;
		case SC_TUTORIAL:
			m_pScene = new CTutorialScene;
		}

		m_pScene->Initialize();

		m_ePreScene = m_eCurScene;
	}
}

int CSceneManager::Update()
{
	m_pScene->Update();
    return 0;
}

void CSceneManager::LateUpdate()
{
	m_pScene->LateUpdate();
}

void CSceneManager::Render(HDC hDC)
{
	m_pScene->Render(hDC);
}

void CSceneManager::Release()
{
	Safe_Delete(m_pScene);
}
