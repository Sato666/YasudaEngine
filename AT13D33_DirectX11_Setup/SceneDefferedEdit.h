//-------------------------------------------------------------------------------------
// Class : SceneDefferedEdit
// �eClass   :   Scene
// 
// SceneDefferedEdit�Ɋւ��鏈�����l�܂��Ă���
// 
// Yasuda.Aoto  2023.01.18
//-------------------------------------------------------------------------------------
#ifndef __SCENE_DEFFEREDEDIT_H__
#define __SCENE_DEFFEREDEDIT_H__

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Scene.hpp"
#include "main.h"		// �E�B���h�E�T�C�Y�擾�p

class SceneDefferedEdit :
    public Scene
{
public:
	void Init() override;
	void Uninit() override;
	void Update(float tick) override;
	void Draw() override;
	void ImGui() override;
private:

};

#endif