//-------------------------------------------------------------------------------------
// Class : Sprite2D
// 親Class   :   GameObject
// 
// 2DUIを表示するゲームオブジェクト
// 
// Yasuda.Aoto  2023.07.14
//-------------------------------------------------------------------------------------
#pragma once
#include "gameobject.h"
class Sprite2D :
    public GameObject
{
public:
    enum class State
    {
        In,	// フェードイン
        Out,	// フェードアウト
        Finish	// フェードアウト終了
    };
public:
    class SpriteComponent* m_Sprite{};

    State m_State = State::In;

    MATERIAL material{};                // マテリアル

    MATERIAL GetMaterial() { return material; } // マテリアルの値を返す
private:
    void GameObjInit() override;
    void GameObjUpdate() override;

    float m_Alpha = 0.0f;
};

