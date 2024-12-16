#pragma once
#include "CScene.h"
// ���� ���� ���� ���� �ٹ̰� ������
// 1. TileManager�� ���� ��� �ٲپ� �ֱ�
// 2. SaveMapObj�Լ��� ���� ��� �ٲپ� �ֱ�
// 3. Create_MapObj�ȿ��� MapObj�ȿ� ��ӵ� ���鸸 �־��ֱ�
class CDungeonEditScene :public CScene
{
public:
	CDungeonEditScene();
	virtual ~CDungeonEditScene() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC hDC) override;
	void Release() override;
	void Key_Input() override;
	void Create_MapObj() override;
	void Offset() override;
private:
	void SaveMapObj();
	void LoadMapObj();
private:
	bool m_bIsShowTile;
};

