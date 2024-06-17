//-------------------------------------------------------------------------------------
// Class : SceneDefferedEdit
// 親Class   :   Scene
// 
// SceneDefferedEditに関する処理が詰まっている
// 
// Yasuda.Aoto  2023.01.18
//-------------------------------------------------------------------------------------
#ifndef __SCENE_DEFFEREDEDIT_H__
#define __SCENE_DEFFEREDEDIT_H__

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Scene.hpp"
#include "main.h"		// ウィンドウサイズ取得用

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