#include "pch.h"
#include "CSoundManager.h"
CSoundManager* CSoundManager::m_pInstance = nullptr;
CSoundManager::CSoundManager() 
{
	m_pSystem = nullptr;
}


CSoundManager::~CSoundManager()
{
	Release();
}

void CSoundManager::Initialize()
{
	FMOD::System_Create(&m_pSystem);
	m_pSystem->init(32, FMOD_INIT_NORMAL, NULL);
	LoadSoundFile();
}
void CSoundManager::Release()
{
	for (auto& Mypair : m_mapSound)
	{
		delete[] Mypair.first;
		(Mypair.second)->release();
	}
	m_mapSound.clear();

	m_pSystem->release();
	m_pSystem->close();
}

void CSoundManager::PlaySound(const TCHAR* pSoundKey, CHANNELID eID, float fVolume, bool ignore)
{
	map<TCHAR*, FMOD::Sound*>::iterator iter;

	// iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));
	iter = find_if(m_mapSound.begin(), m_mapSound.end(),
		[&](auto& iter)->bool
		{
			return !lstrcmp(pSoundKey, iter.first);
		});

	if (iter == m_mapSound.end())
		return;

	if (ignore) {
		FMOD_BOOL bPlay = FALSE;
		m_pSystem->playSound(iter->second, 0, FALSE, &m_pChannelArr[eID]);
		m_pChannelArr[eID]->setVolume(fVolume);
	}
	else {
		bool bIsPlaying = false;
		if (m_pChannelArr[eID]->isPlaying(&bIsPlaying)) {
			FMOD_BOOL bPlay = FALSE;
			m_pSystem->playSound(iter->second, 0, FALSE, &m_pChannelArr[eID]);
			m_pChannelArr[eID]->setVolume(fVolume);
		}
	}

	m_pSystem->update();
}

void CSoundManager::PlayBGM(const TCHAR* pSoundKey, float fVolume)
{
	map<TCHAR*, FMOD::Sound*>::iterator iter;

	// iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));
	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)->bool
		{
			return !lstrcmp(pSoundKey, iter.first);
		});

	if (iter == m_mapSound.end())
		return;


	FMOD_BOOL bPlay = FALSE;
	m_pSystem->playSound(iter->second, 0, FALSE, &m_pChannelArr[SOUND_BGM]);
	m_pChannelArr[SOUND_BGM]->setMode(FMOD_LOOP_NORMAL);
	m_pChannelArr[SOUND_BGM]->setVolume(fVolume);

	m_pSystem->update();
}

void CSoundManager::StopSound(CHANNELID eID)
{
	m_pChannelArr[eID]->stop();
}

void CSoundManager::StopAll()
{
	FMOD_BOOL bPaused = TRUE;
	for (int i = 0; i < SOUND_END; ++i)
		m_pChannelArr[i]->stop();
}

void CSoundManager::SetChannelVolume(CHANNELID eID, float fVolume)
{
	m_pChannelArr[eID]->setVolume(fVolume);
	m_pSystem->update();
}

void CSoundManager::LoadSoundFile()
{
	// _finddata_t : <io.h>���� �����ϸ� ���� ������ �����ϴ� ����ü
	_finddata_t fd;

	// _findfirst : <io.h>���� �����ϸ� ����ڰ� ������ ��� ������ ���� ù ��° ������ ã�� �Լ�
	long long handle = _findfirst("../Sound/*.*", &fd);

	if (handle == -1)
		return;

	int iResult = 0;

	char szCurPath[128] = "../Sound/";	 // ��� ���
	char szFullPath[128] = "";

	while (iResult != -1)
	{
		strcpy_s(szFullPath, szCurPath);

		// "../Sound/" + "Success.wav"
		strcat_s(szFullPath, fd.name);
		// "../Sound/Success.wav"

		FMOD::Sound* pSound = nullptr;

		FMOD_RESULT eRes = m_pSystem->createSound(szFullPath, FMOD_DEFAULT, 0, &pSound);
			
		if (eRes == FMOD_OK)
		{
			int iLength = strlen(fd.name) + 1;

			TCHAR* pSoundKey = new TCHAR[iLength];
			ZeroMemory(pSoundKey, sizeof(TCHAR) * iLength);

			// �ƽ�Ű �ڵ� ���ڿ��� �����ڵ� ���ڿ��� ��ȯ�����ִ� �Լ�
			MultiByteToWideChar(CP_ACP, 0, fd.name, iLength, pSoundKey, iLength);

			m_mapSound.emplace(pSoundKey, pSound);
		}
		//_findnext : <io.h>���� �����ϸ� ���� ��ġ�� ������ ã�� �Լ�, ���̻� ���ٸ� -1�� ����
		iResult = _findnext(handle, &fd);
	}

	m_pSystem->update();

	_findclose(handle);
}
