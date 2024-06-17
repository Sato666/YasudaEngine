//-------------------------------------------------------------------------------------
// Class : DXCamera
// 親Class   :   Component
// 
// デバッグカメラのコンポーネントクラス
// 回転制御・平行移動制御・ズーム制御・自由移動機能を持つ
// 左クリックしながらWSで前進後進ADで右左移動QEで真上真下移動
// Alt押しながら右クリックで回転制御
// Alt押しながら中クリックで平行移動制御
// Alt押しながら右クリックでズーム制御
// 
// Yasuda.Aoto  2023.08.21
//-------------------------------------------------------------------------------------
#pragma once

#include "component.h"

#include <DirectXMath.h>
#include <SimpleMath.h>

#include <Windows.h>

class DXCamera :
    public Component
{
private:
	using Component::Component;

	// カメラの値を構造体で持つ
	struct Argument
	{
		DirectX::XMFLOAT2 mouseMove;	// マウスの移動量
		DirectX::SimpleMath::Vector3 vCamFront;	// カメラフロント
		DirectX::SimpleMath::Vector3 vCamSide;		// カメラの側
		DirectX::SimpleMath::Vector3 vCamUp;		// カメラのベクトル
		DirectX::SimpleMath::Vector3 vCamPos;		// カメラの位置
		DirectX::SimpleMath::Vector3 vCamLook;		// カメラの視点
		float focus;					// 視野
	};

	DirectX::SimpleMath::Matrix		m_ViewMatrix{};	// 計算後のマトリックス補間用
public:
	// 初期値
	void Init();
	// 描画
	void Draw() override;
	// 更新
	void Update() override;
	// ビュー行列を返す
	DirectX::XMFLOAT4X4 GetView(bool transpose = true);
	// プロジェクション行列を返す
	DirectX::XMFLOAT4X4 GetProj(bool transpose = true);
	// 位置取得
	DirectX::SimpleMath::Vector3 GetPos();
	// 視点取得
	DirectX::SimpleMath::Vector3 GetLook();
	// ベクトル取得
	DirectX::SimpleMath::Vector3 GetUp();
	// 位置セット
	void SetPos(DirectX::SimpleMath::Vector3 pos);
	// 視点セット
	void SetLook(DirectX::SimpleMath::Vector3 look);
	// ベクトルセット
	void SetUp(DirectX::SimpleMath::Vector3 up);

	// 移動倍率取得
	float GetMoveVal() { return m_MoveVal; }
	// 移動倍率セット
	void SetMoveVal(float _val) { m_MoveVal = _val; }
protected:
	// 位置・視点・ベクトル変数
	DirectX::SimpleMath::Vector3 m_pos, m_look, m_up;
	// 視野・アスペクト比・ニアクリップ・ファークリップ
	float m_fovy, m_aspect, m_near, m_far;

private:
	// ステート変更判定
	void UpdateState();
	// 回転制御処理
	void UpdateOrbit(Argument& arg);
	// 平行移動制御処理
	void UpdateTrack(Argument& arg);
	// ズーム制御処理
	void UpdateDolly(Argument& arg);
	// 自由移動処理
	void UpdateFlight(Argument& arg);

private:
	int m_state;		// 現在のステート
	POINT m_oldPos;		// 前回のマウス座標

	bool isBackCamera = false;	// 追従カメラかどうか

	float m_MoveVal = 2.0f;	// 移動倍率
};
