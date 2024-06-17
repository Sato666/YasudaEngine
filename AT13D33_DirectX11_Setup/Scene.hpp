//-------------------------------------------------------------------------------------
// Class : Scene
// 
// シーンを管理するクラス
// サブシーンの作成とシーン内でオブジェクトの生成の機能を持つ
// 
// Yasuda.Aoto  2023.11.09
//-------------------------------------------------------------------------------------

#ifndef __SCENE_HPP__
#define __SCENE_HPP__

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <memory>
#include <map>
#include <string>
#include <list>
#include <Windows.h>
#include <array>

#include "Geometory.h"

// GameObject
#include "gameobject.h"

// resource
#include "Sprite.h"
#include "Shader.h"

// カメラ
#include "camera.h"
#include "DXCamera.h"

// light
#include "MoveLight.h"
#include "Light.h"

// シェーダー系
#include "ShaderComponent.h"

// モデル系
#include "Model.h"
#include "ModelMeshLoad.h"
#include "ModelMeshRenderer.h"

#include "AnimModel.h"
#include "AnimModelMeshLoad.h"
#include "AnimModelMeshRenderer.h"

#include "ModelSetting.h"

// 入力情報
#include "Input.h"

// ImGui
#include "imgui/imgui.h"

// オブジェクトの追加用基底クラス
class SceneObjectBase
{
public:
	virtual ~SceneObjectBase() {}
};

// リソースの所有権共有クラス
template<class T>
class SceneObject : public SceneObjectBase
{
public:
	SceneObject(std::shared_ptr<T> ptr) : m_pObj(ptr) {}
	virtual ~SceneObject() {}
	std::shared_ptr<T> m_pObj;
};

// シーン基本クラス
class Scene
{
private:
	using Objects = std::map<std::string, std::shared_ptr<SceneObjectBase>>;
	using Items = std::list<std::string>;
private:
	static Objects m_objects;		// オブジェクトリスト（名前とクラスポインタで保存）
protected:
	Scene* m_pParent;			// 自身のシーン
	Scene* m_pSubScene;			// サブシーン
	Items m_items;				// アイテムリスト（名前で保存）

	std::vector<GameObject*> ObjeList;	// 生成したオブジェクトのリスト
public:
	Scene();
	virtual ~Scene();
	void _update(float tick);
	void _draw();
	void _imgui();

	// シーン操作関数
	template<class T> T* AddSubScene();
	void RemoveSubScene();

	// オブジェクト操作関数
	template<class T> T* CreateObj(const char* name);
	void DestroyObj(const char* name);
	template<class T> T* GetObj(const char* name);

	// 継承シーンの一通りの処理
	virtual void Init() {};
	virtual void Uninit() {};
	virtual void Update(float tick) {};
	virtual void Draw() {};
	virtual void ImGui() {};

	// シーンに登録されているオブジェクトの初期化処理
	void InitBase()
	{
		Init();
	}

	// シーンに登録されているオブジェクトの解放処理
	void UninitBase()
	{
		Uninit();
	}

	// シーンに登録されているオブジェクトの更新処理
	void UpdateBase(float tick)
	{
		_update(tick);
	}

	// シーンに登録されているオブジェクトの描画処理
	void DrawBase()
	{
		_draw();
	}

	// シーンに登録されているオブジェクトのImGut描画処理
	void ImGuiBase()
	{
		_imgui();
	}
};

/// <summary>
/// サブシーンの追加
/// </summary>
/// <typeparam name="T">サブシーンの型</typeparam>
/// <returns>生成したサブシーン</returns>
template<class T> T* Scene::AddSubScene()
{
	RemoveSubScene();
	T* pScene = new T;
	m_pSubScene = pScene;
	pScene->m_pParent = this;
	pScene->Init();
	return pScene;
}

/// <summary>
/// オブジェクトの生成
/// </summary>
/// <typeparam name="T">オブジェクトの型</typeparam>
/// <param name="name">オブジェクトの名称</param>
/// <returns>生成したオブジェクト</returns>
template<class T> T* Scene::CreateObj(const char* name)
{
#ifdef _DEBUG
	// デバッグ中のみ、名称ダブりがないかチェック
	Objects::iterator it = m_objects.find(name);
	if (it != m_objects.end()) {
		static char buf[256];
		sprintf_s(buf, sizeof(buf), "Failed to create object. %s", name);
		MessageBox(NULL, buf, "Error", MB_OK);
		return nullptr;
	}
#endif // _DEBUG

	// オブジェクト生成
	std::shared_ptr<T> ptr = std::make_shared<T>();
	m_objects.insert(std::pair<std::string, std::shared_ptr<SceneObjectBase>>(name, std::make_shared<SceneObject<T>>(ptr)));
	m_items.push_back(name);
	return ptr.get();
}

/// <summary>
/// オブジェクトの取得
/// </summary>
/// <typeparam name="T">オブジェクトの型</typeparam>
/// <param name="name">オブジェクトの名称</param>
/// <returns>取得したオブジェクト</returns>
template<class T> T* Scene::GetObj(const char* name)
{
	// オブジェクトの探索
	Objects::iterator it = m_objects.find(name);		// 名前で検索
	if (it == m_objects.end()) return nullptr;			// なければここでnullptr返す

	// 型変換
	std::shared_ptr<SceneObject<T>> ptr = std::reinterpret_pointer_cast<SceneObject<T>>(it->second);
	return ptr->m_pObj.get();							// オブジェクトのポインタ返す
}

#endif // __SCENE_BASE_HPP__