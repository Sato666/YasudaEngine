//-------------------------------------------------------------------------------------
// Class : Camera
// �eClass   :   GameObject
// 
// �J�����̃Q�[���I�u�W�F�N�g�N���X
// �r���[�}�g���b�N�X���󂯎��@�\�Ɠn���@�\������
// �V�[���ɂ���ăJ�����̃R���|�[�l���g��؂�ւ���@�\������
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

	class Audio* m_BGM{};	// BGM�p

	float m_MoveVal = 2.0f;
public:
	void GameObjInit() override;

	// �ړ��{���擾
	float GetMoveVal() { return this->GetComponent<DXCamera>()->GetMoveVal(); }
	// �ړ��{���Z�b�g
	void SetMoveVal(float _val) { m_MoveVal = _val;
										this->GetComponent<DXCamera>()->SetMoveVal(m_MoveVal); }
	// �r���[�s���Ԃ�
	DirectX::SimpleMath::Matrix GetViewMatrix() { return this->GetComponent<DXCamera>()->GetView(); }
	// �v���W�F�N�V�����s���Ԃ�
	DirectX::SimpleMath::Matrix GetProjMatrix() { return this->GetComponent<DXCamera>()->GetProj(); }
};