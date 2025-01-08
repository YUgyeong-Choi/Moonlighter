#pragma once
#include "Define.h"
class CSoundManager
{
public:
	static CSoundManager* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CSoundManager;

		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	CSoundManager();
	~CSoundManager();

public:
	void Initialize();
	void Release();

public:
	void PlaySound(const TCHAR* pSoundKey, CHANNELID eID, float fVolume, bool ignore);
	void PlayBGM(const TCHAR* pSoundKey, float fVolume, bool ignore);
	void StopSound(CHANNELID eID);
	void StopAll();
	void PlayStopSound(CHANNELID eID);
	void SetChannelVolume(CHANNELID eID, float fVolume);

private:
	void LoadSoundFile();

private:
	static CSoundManager* m_pInstance;

	// ���� ���ҽ� ������ ���� ��ü 
	map<TCHAR*, FMOD::Sound*> m_mapSound;

	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD::Channel* m_pChannelArr[SOUND_END];

	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
	FMOD::System* m_pSystem;
};
