//-------------------------------------------------------------------------------------
// Class : Manager
// 
// すべてのシーンに必要な機能の初期化解放等を行う
// シーン機能を管理し現在のシーンを渡す機能を持つ
// 
// Yasuda.Aoto  2023.07.14
//-------------------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <cinttypes>
#include	"SceneMain.h"

// 前方参照
class Application;

class Manager
{
private:
	static std::shared_ptr<SceneMain> m_pScene;
public:
	static void Init(Application* ap);
	static void Uninit();
	static void Update(uint64_t d);
	static void Draw(uint64_t d);

	// シーンをゲットする
	static std::shared_ptr<SceneMain> GetScene()
	{
		return m_pScene;
	}

private:
	// 全シーン共通のImGuiの描画
	static void MyImGui();

private:
};