//-------------------------------------------------------------------------------------
// Class : SceneMain
// �eClass   :   Scene
// 
// �V�[����؂�ւ�����Ǘ�����@�\������
// 
// Yasuda.Aoto  2023.11.09
//-------------------------------------------------------------------------------------
#ifndef __SCENE_MAIN_H__
#define __SCENE_MAIN_H__

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Scene.hpp"

class SceneMain :
    public Scene
{
public:
	void Init() override;
	void Uninit() override;
	void Update(float tick) override;
	void Draw() override;

private:
	// �V�[���̕ύX
	void ChangeScene();

private:
	int m_index;					// �V�[���̔ԍ�
	int i_duplicationMouse = 0;		// �}�E�X�\����\�������Ăяo���֎~�ϐ�
};

#endif // __SCENE_ROOT_H__