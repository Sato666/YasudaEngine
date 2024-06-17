//-------------------------------------------------------------------------------------
// Class : Sprite2D
// �eClass   :   GameObject
// 
// 2DUI��\������Q�[���I�u�W�F�N�g
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
        In,	// �t�F�[�h�C��
        Out,	// �t�F�[�h�A�E�g
        Finish	// �t�F�[�h�A�E�g�I��
    };
public:
    class SpriteComponent* m_Sprite{};

    State m_State = State::In;

    MATERIAL material{};                // �}�e���A��

    MATERIAL GetMaterial() { return material; } // �}�e���A���̒l��Ԃ�
private:
    void GameObjInit() override;
    void GameObjUpdate() override;

    float m_Alpha = 0.0f;
};

