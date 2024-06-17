//-------------------------------------------------------------------------------------
// Class : Light
// 親Class   :   Component
// 
// Lightのコンポーネントクラス
// Lightの移動と色変化を操作できる
// 
// Yasuda.Aoto  2023.09.13
//-------------------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "component.h"
#include "Input.h"
#include "renderer.h"

#include <SimpleMath.h>

class Light :
    public Component
{
private:
	using Component::Component;

	LIGHT light{};		// ライト

	float m_radXZ;
	float m_radY;
	float m_hue;
	float m_saturationValue;
	DirectX::XMFLOAT3 m_pos;	// 位置
public:
	void Init() override;
	void Draw() override;
	void Update() override;
public:

	void UpdateParam();		// 移動Update
	void UpdateColorParam();	// 色変化Updata
	DirectX::SimpleMath::Vector4 HSV2Color(float h, float sv);

	void SetRot(float xz, float y);
	float GetRotXZ();
	float GetRotY();

	void SetHSV(float h, float sv);
	float GetH();
	float GetSV();

	// 位置のセッターゲッター
	void SetPosition(DirectX::SimpleMath::Vector3 _pos) {
		m_pos = DirectX::SimpleMath::Vector3(
			_pos.x,
			_pos.y,
			_pos.z);
	}
	DirectX::XMFLOAT3 GetPos() { return m_pos; }		// 位置を渡す

	// 方向のセッターゲッター
	void SetDirection(DirectX::SimpleMath::Vector3 _pos) {
		light.Direction = DirectX::SimpleMath::Vector3(
			_pos.x,
			_pos.y,
			_pos.z);
	}
	DirectX::SimpleMath::Vector3 GetDirection() { return light.Direction; }

	// 光の色のセッターゲッター
	void SetAmbient(DirectX::SimpleMath::Color _color) {
		light.Ambient = DirectX::SimpleMath::Color(
			_color.x,
			_color.y,
			_color.z,
			_color.w);
	};
	DirectX::SimpleMath::Color GetAmbient() { return light.Ambient; }
	// 光の拡散のゲッターセッター
	void SetDiffuse(DirectX::SimpleMath::Color _color) {
		light.Diffuse = DirectX::SimpleMath::Color(
			_color.x,
			_color.y,
			_color.z,
			_color.w);
	};
	DirectX::SimpleMath::Color GetDiffuse() { return light.Diffuse; }
};
