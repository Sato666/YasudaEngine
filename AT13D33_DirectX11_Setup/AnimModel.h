//-------------------------------------------------------------------------------------
// Class : AnimModel
// 親Class   :   GameObject
// 
// アニメーション用Modelのゲームオブジェクトクラス
// 
// Yasuda.Aoto  2023.01.16
//-------------------------------------------------------------------------------------
#pragma once
#include "gameobject.h"
#include "AnimationData.h"

class AnimModel :
    public GameObject
{
private:
    // アニメーションデータ
    CAnimationData m_AnimData;
public:
    void GameObjInit() override;
    void GameObjUpdate() override;
    void GameObjUninit() override;
};

