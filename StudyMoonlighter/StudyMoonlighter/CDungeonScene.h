#pragma once
#include "CScene.h"
class CDungeonScene : public CScene
{
    // CScene��(��) ���� ��ӵ�
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(HDC hDC) override;
    void Release() override;
    void Key_Input() override;
    void Create_MapObj() override;
    void Offset() override;
};

