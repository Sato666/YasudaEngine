//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Scene.hpp"

Scene::Objects Scene::m_objects;

Scene::Scene()
	: m_pParent(nullptr)
	, m_pSubScene(nullptr)
{
}
Scene::~Scene()
{
	// サブシーンを削除
	RemoveSubScene();

	// 削除
	Items::iterator it = m_items.begin();
	while (it != m_items.end())
	{
		m_objects.erase(*it);
		++it;
	}
	m_items.clear();

	// 親の参照を削除
	if (m_pParent)
		m_pParent->m_pSubScene = nullptr;
}
/// <summary>
/// サブシーンを考慮した更新処理
/// </summary>
/// <param name="tick">時間</param>
void Scene::_update(float tick)
{
	if (m_pSubScene)
		m_pSubScene->_update(tick);
	Update(tick);
}
/// <summary>
/// サブシーンを考慮した描画処理
/// </summary>
void Scene::_draw()
{
	if (m_pSubScene)
		m_pSubScene->_draw();
	Draw();
}
/// <summary>
/// サブシーンを考慮したImGui描画処理
/// </summary>
void Scene::_imgui()
{
	if (m_pSubScene)
		m_pSubScene->_imgui();
	ImGui();
}

/// <summary>
/// @brief サブシーンの削除
/// </summary>
void Scene::RemoveSubScene()
{
	// 削除するサブシーンが存在するか
	if (!m_pSubScene) return;

	// 階層内のサブシーンを優先して削除
	m_pSubScene->RemoveSubScene();

	// 直下のサブシーンを削除
	m_pSubScene->Uninit();
	delete m_pSubScene;
	m_pSubScene = nullptr;
}

/// <summary>
/// 生成したオブジェクトの削除
/// </summary>
/// <param name="name">名前検索</param>
void Scene::DestroyObj(const char* name)
{
	m_objects.erase(name);
	m_items.remove(name);
}