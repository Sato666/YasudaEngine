//-------------------------------------------------------------------------------------
// Class : Camera
// 親Class   :   GameObject
// 
// カメラのゲームオブジェクトクラス
// ビューマトリックスを受け取る機能と渡す機能を持つ
// シーンによってカメラのコンポーネントを切り替える機能を持つ
// 
// Yasuda.Aoto  2023.07.14
//-------------------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include	"gameObject.h"
#include	"DXCamera.h"

class Camera : public GameObject
{
private:
	DirectX::SimpleMath::Matrix		m_ViewMatrix{};

	class Audio* m_BGM{};	// BGM用

	float m_MoveVal = 2.0f;
public:
	void GameObjInit() override;

	// 移動倍率取得
	float GetMoveVal() { return this->GetComponent<DXCamera>()->GetMoveVal(); }
	// 移動倍率セット
	void SetMoveVal(float _val) { m_MoveVal = _val;
										this->GetComponent<DXCamera>()->SetMoveVal(m_MoveVal); }
	// ビュー行列を返す
	DirectX::SimpleMath::Matrix GetViewMatrix() { return this->GetComponent<DXCamera>()->GetView(); }
	// プロジェクション行列を返す
	DirectX::SimpleMath::Matrix GetProjMatrix() { return this->GetComponent<DXCamera>()->GetProj(); }
};