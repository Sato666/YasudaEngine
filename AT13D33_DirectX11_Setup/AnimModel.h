//-------------------------------------------------------------------------------------
// Class : AnimModel
// �eClass   :   GameObject
// 
// �A�j���[�V�����pModel�̃Q�[���I�u�W�F�N�g�N���X
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
    // �A�j���[�V�����f�[�^
    CAnimationData m_AnimData;
public:
    void GameObjInit() override;
    void GameObjUpdate() override;
    void GameObjUninit() override;
};

