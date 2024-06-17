//-------------------------------------------------------------------------------------
// Class : GameObject 
// 
// 親ゲームオブジェクトクラス
// 位置・角度・サイズをセットまたは、返す機能を持つ
// 前向きベクトルを返す機能を持つ
// 自身を消す機能を持つ
// このClassを継承した子クラスにコンポーネントを追加または返す機能を持つ
// このClassを継承した子クラスに子オブジェクトを追加する機能を持つ
// それぞれ追加したコンポーネントクラスを一括に初期化・解放・更新・描画する機能を持つ
// 
// 更新処理の順番（07.17)
// ↓子オブジェクト
// ↓コンポーネント
// ↓自身
// 
// Yasuda.Aoto  2023.07.17
//-------------------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "renderer.h"

#include <list>
#include <SimpleMath.h>

#include "component.h"
#include "Shader.h"

class GameObject
{
protected:
	bool		m_Destroy = false;	// 解放フラグ
	bool		m_Active = true;	// 描画フラグ
	bool		m_isinfrustum = true;		// フラスタム内かどうか

	std::string m_ObjectName;		// オブジェクトの名前
	std::string m_ID;				// オブジェクトのID

	DirectX::SimpleMath::Vector3	m_Position = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);	// 位置
	DirectX::SimpleMath::Vector3	m_Rotation = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);	// 角度
	DirectX::SimpleMath::Vector3	m_Scale = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);		// サイズ

	std::list<Component*> m_Component;			// コンポーネントクラス格納
	std::list<GameObject*> m_ChildGameObject;	// 子オブジェクト格納
public:
	GameObject() {}//コンストラクタ
	virtual ~GameObject() {}//デストラクタ（仮想関数）

	/// <summary>
	/// 名前を取得
	/// </summary>
	/// <returns>string型</returns>
	std::string GetName() { return m_ObjectName; }
	/// <summary>
	/// IDを取得
	/// </summary>
	/// <returns>string型</returns>
	std::string GetID() { return m_ID; }

	// 位置・角度・サイズ・色を返す関数
	/// <summary>
	/// 位置を返す
	/// </summary>
	/// <returns>Vector3型</returns>
	DirectX::SimpleMath::Vector3 GetPosition() { return m_Position; }		// 位置
	/// <summary>
	/// 角度を返す
	/// </summary>
	/// <returns>Vector3型</returns>
	DirectX::SimpleMath::Vector3 GetRotation() { return m_Rotation; }		// 角度
	/// <summary>
	/// サイズを返す
	/// </summary>
	/// <returns>Vector3型</returns>
	DirectX::SimpleMath::Vector3 GetScale() { return m_Scale; }				// サイズ

	// 名前をセット
	void SetName(std::string _name) { m_ObjectName = _name; }
	// IDをセット
	void SetID(std::string _id) { m_ID = _id; }

	// 位置・角度・サイズ・色をセットする関数
	/// <summary>
	/// 位置をセット
	/// </summary>
	/// <param name="_position">Vector3型</param>
	void SetPosition(DirectX::SimpleMath::Vector3 _position) { m_Position = _position; }		// 位置
	/// <summary>
	/// 角度をセット
	/// </summary>
	/// <param name="_rotation">Vector3型</param>
	void SetRotation(DirectX::SimpleMath::Vector3 _rotation) { m_Rotation = _rotation; }		// 角度
	/// <summary>
	/// サイズをセット
	/// </summary>
	/// <param name="_scale">Vector3型</param>
	void SetScale(DirectX::SimpleMath::Vector3 _scale) { m_Scale = _scale; }					// サイズ

	/// <summary>
	/// 前向きベクトルを返す関数
	/// </summary>
	/// <returns>Vector3型</returns>
	DirectX::SimpleMath::Vector3 GetForward()
	{
		DirectX::SimpleMath::Matrix rot;
		rot = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(m_Rotation.y, m_Rotation.x, m_Rotation.z);

		DirectX::SimpleMath::Vector3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;
	}

	// 自信を解放する関数
	void SetDestroy() { m_Destroy = true; }
	void SetActive(bool _is) { m_Active = _is; }

	bool Destroy()
	{
		// 解放するなら
		if (m_Destroy)
		{
			Uninit();	// 解放
			delete this;
			return true;
		}
		else
		{
			return false;
		}
	}

	virtual void GameObjInit() {}		// 初期化
	virtual void GameObjUninit() {}		// 解放
	virtual void GameObjUpdate() {}		// 更新
	virtual void GameObjDraw() {}		// 描画
	virtual void PreDraw() {}

	/// <summary>
	/// コンポーネントを追加する関数
	/// </summary>
	/// <typeparam name="T">コンポーネントのクラス</typeparam>
	/// <returns>コンポーネントのクラス</returns>
	template <typename T>
	T* AddComponent()
	{
		T* component = new T(this);
		m_Component.push_back(component);
		((Component*)component)->Init();	// コンポーネントの初期化

		return component;
	}
	
	/// <summary>
	/// コンポーネントを返す関数
	/// </summary>
	/// <returns>どれも一致しなければNullを返す</returns>
	template <typename T>
	T* GetComponent()
	{
		for (Component* component : m_Component)
		{
			// 追加されているコンポーネントの名前が一緒なら
			if (typeid(*component) == typeid(T))
			{
				return (T*)component;	// 返す
			}
		}
		return nullptr;		// どれも一致しなければNullを返す
	}

	template <typename T>
	T* DeleteComponent()
	{
		for (Component* component : m_Component)
		{
			// 追加されているコンポーネントの名前が一緒なら
			if (typeid(*component) == typeid(T))
			{
				m_Component.pop_back(component);
				return true;	// 成功
			}
		}
		return false;		// 失敗
	}

	/// <summary>
	/// 子供オブジェクトを追加する
	/// </summary>
	template <typename T>
	T* AddChild()
	{
		T* child = new T();
		m_ChildGameObject.push_back(child);
		child->InitBase();	// 初期化処理

		return child;
	}

	// 初期化ベース
	void Init()
	{
		GameObjInit();
	}

	// 解放ベース
	void Uninit()
	{
		// 子オブジェクトの解放
		for (GameObject* child : m_ChildGameObject)
		{
			child->Uninit();
			delete child;
		}
		m_ChildGameObject.clear();//リストのクリア

		// 親オブジェクトの解放
		for (Component* component : m_Component)
		{
			component->Uninit();
			delete component;
		}
		m_Component.clear();//リストのクリア

		GameObjUninit();
	}

	// 更新ベース
	void Update()
	{
		// 子オブジェクトの更新
		for (GameObject* child : m_ChildGameObject)
		{
			child->Update();
		}
		// 親オブジェクトの更新
		for (Component* component : m_Component)
		{
			component->Update();
		}
		// 自身の更新
		GameObjUpdate();
	}

	void DrawChildBase(DirectX::SimpleMath::Matrix ParentMatrix)
	{
		// マトリクス設定
		DirectX::SimpleMath::Matrix world, scale, rot, trans;
		scale = DirectX::SimpleMath::Matrix::CreateScale(m_Scale.x, m_Scale.y, m_Scale.z);
		rot = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(m_Rotation.y, m_Rotation.x, m_Rotation.z);
		trans = DirectX::SimpleMath::Matrix::CreateTranslation(m_Position.x, m_Position.y, m_Position.z);
		world = scale * rot * trans * ParentMatrix;

		PreDraw();

		// 子オブジェクトの描画
		for (GameObject* child : m_ChildGameObject)
		{
			child->DrawChildBase(world);
		}

		// 親オブジェクトの描画
		for (Component* component : m_Component)
		{
			// アクティブ状態なら
			if (m_Active == true)
			{
				component->Draw();
			}
		}

		GameObjDraw();
	}

	/// <summary>
	/// オブジェクトの描画
	/// </summary>
	/// <param name="_setvsshader">セットされているVSShader</param>
	/// <param name="_mat">変換行列</param>
	void Draw(Shader* _setvsshader, DirectX::XMFLOAT4X4 _mat[3])
	{
		// 単位行列にする
		DirectX::XMFLOAT4X4 matrix;
		DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixIdentity());

		// マトリクス設定
		DirectX::SimpleMath::Matrix world, scale, rot, trans;
		scale = DirectX::SimpleMath::Matrix::CreateScale(m_Scale.x, m_Scale.y, m_Scale.z);
		rot = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(m_Rotation.y, m_Rotation.x, m_Rotation.z);
		trans = DirectX::SimpleMath::Matrix::CreateTranslation(m_Position.x, m_Position.y, m_Position.z);
		world = scale * rot * trans;		// ワールド行列の左手系の計算

		PreDraw();

		// 転置する
		world = world.Transpose();			// 左手系に行列を入れ替える

		_mat[0] = world;	// ワールドだけ更新
		_setvsshader->WriteBuffer(0, _mat);

		// 親オブジェクトの描画
		for (Component* component : m_Component)
		{
			// アクティブ状態なら
			if (m_Active == true)
			{
				component->Draw();
			}
		}

		GameObjDraw();
	}

	/// <summary>
	/// ワールド変換無しの描画
	/// </summary>
	void DefaultDraw()
	{
		// 親オブジェクトの描画
		for (Component* component : m_Component)
		{
			// アクティブ状態なら
			if (m_Active == true)
			{
				component->Draw();
			}
		}

		GameObjDraw();
	}

	void SetInFrustumn(bool flag) {
		m_isinfrustum = flag;
	}

	bool IsInFrustumn() {
		return m_isinfrustum;
	}
};