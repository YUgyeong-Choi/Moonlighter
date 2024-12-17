#pragma once
#include "CScene.h"
class CDungeonScene : public CScene
{
public:
    CDungeonScene();
    CDungeonScene(const TCHAR* _pFilePath, int _x, int _y);
    virtual ~CDungeonScene() { Release(); }
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(HDC hDC) override;
    void Release() override;
    void Key_Input() override;
    void Create_MapObj() override;
    void Offset() override;
public:
    void Load_Map();
    void Set_MapXIndex(int _x) { m_iMapXIndex = _x; }
    void Set_MapYIndex(int _y) { m_iMapYIndex = _y; }
private:
    const TCHAR* pFilePath;
    int m_iMapXIndex;
    int m_iMapYIndex;
};

