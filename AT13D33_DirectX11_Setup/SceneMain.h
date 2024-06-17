//-------------------------------------------------------------------------------------
// Class : SceneMain
// 親Class   :   Scene
// 
// シーンを切り替えたり管理する機能を持つ
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
	// シーンの変更
	void ChangeScene();

private:
	int m_index;					// シーンの番号
	int i_duplicationMouse = 0;		// マウス表示非表示複数呼び出し禁止変数
};

#endif // __SCENE_ROOT_H__