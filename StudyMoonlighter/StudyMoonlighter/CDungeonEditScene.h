#pragma once
#include "CScene.h"
// 유의 사항 던전 씬을 꾸미고 싶을때
// 1. TileManager의 파일 경로 바꾸어 주기
// 2. SaveMapObj함수의 파일 경로 바꾸어 주기
// 3. Create_MapObj안에는 MapObj안에 상속된 값들만 넣어주기
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

