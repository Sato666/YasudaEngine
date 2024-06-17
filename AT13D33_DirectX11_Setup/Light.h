//-------------------------------------------------------------------------------------
// Class : Light
// �eClass   :   Component
// 
// Light�̃R���|�[�l���g�N���X
// Light�̈ړ��ƐF�ω��𑀍�ł���
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

	LIGHT light{};		// ���C�g

	float m_radXZ;
	float m_radY;
	float m_hue;
	float m_saturationValue;
	DirectX::XMFLOAT3 m_pos;	// �ʒu
public:
	void Init() override;
	void Draw() override;
	void Update() override;
public:

	void UpdateParam();		// �ړ�Update
	void UpdateColorParam();	// �F�ω�Updata
	DirectX::SimpleMath::Vector4 HSV2Color(float h, float sv);

	void SetRot(float xz, float y);
	float GetRotXZ();
	float GetRotY();

	void SetHSV(float h, float sv);
	float GetH();
	float GetSV();

	// �ʒu�̃Z�b�^�[�Q�b�^�[
	void SetPosition(DirectX::SimpleMath::Vector3 _pos) {
		m_pos = DirectX::SimpleMath::Vector3(
			_pos.x,
			_pos.y,
			_pos.z);
	}
	DirectX::XMFLOAT3 GetPos() { return m_pos; }		// �ʒu��n��

	// �����̃Z�b�^�[�Q�b�^�[
	void SetDirection(DirectX::SimpleMath::Vector3 _pos) {
		light.Direction = DirectX::SimpleMath::Vector3(
			_pos.x,
			_pos.y,
			_pos.z);
	}
	DirectX::SimpleMath::Vector3 GetDirection() { return light.Direction; }

	// ���̐F�̃Z�b�^�[�Q�b�^�[
	void SetAmbient(DirectX::SimpleMath::Color _color) {
		light.Ambient = DirectX::SimpleMath::Color(
			_color.x,
			_color.y,
			_color.z,
			_color.w);
	};
	DirectX::SimpleMath::Color GetAmbient() { return light.Ambient; }
	// ���̊g�U�̃Q�b�^�[�Z�b�^�[
	void SetDiffuse(DirectX::SimpleMath::Color _color) {
		light.Diffuse = DirectX::SimpleMath::Color(
			_color.x,
			_color.y,
			_color.z,
			_color.w);
	};
	DirectX::SimpleMath::Color GetDiffuse() { return light.Diffuse; }
};
