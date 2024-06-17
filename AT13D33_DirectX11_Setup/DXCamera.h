//-------------------------------------------------------------------------------------
// Class : DXCamera
// �eClass   :   Component
// 
// �f�o�b�O�J�����̃R���|�[�l���g�N���X
// ��]����E���s�ړ�����E�Y�[������E���R�ړ��@�\������
// ���N���b�N���Ȃ���WS�őO�i��iAD�ŉE���ړ�QE�Ő^��^���ړ�
// Alt�����Ȃ���E�N���b�N�ŉ�]����
// Alt�����Ȃ��璆�N���b�N�ŕ��s�ړ�����
// Alt�����Ȃ���E�N���b�N�ŃY�[������
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

	// �J�����̒l���\���̂Ŏ���
	struct Argument
	{
		DirectX::XMFLOAT2 mouseMove;	// �}�E�X�̈ړ���
		DirectX::SimpleMath::Vector3 vCamFront;	// �J�����t�����g
		DirectX::SimpleMath::Vector3 vCamSide;		// �J�����̑�
		DirectX::SimpleMath::Vector3 vCamUp;		// �J�����̃x�N�g��
		DirectX::SimpleMath::Vector3 vCamPos;		// �J�����̈ʒu
		DirectX::SimpleMath::Vector3 vCamLook;		// �J�����̎��_
		float focus;					// ����
	};

	DirectX::SimpleMath::Matrix		m_ViewMatrix{};	// �v�Z��̃}�g���b�N�X��ԗp
public:
	// �����l
	void Init();
	// �`��
	void Draw() override;
	// �X�V
	void Update() override;
	// �r���[�s���Ԃ�
	DirectX::XMFLOAT4X4 GetView(bool transpose = true);
	// �v���W�F�N�V�����s���Ԃ�
	DirectX::XMFLOAT4X4 GetProj(bool transpose = true);
	// �ʒu�擾
	DirectX::SimpleMath::Vector3 GetPos();
	// ���_�擾
	DirectX::SimpleMath::Vector3 GetLook();
	// �x�N�g���擾
	DirectX::SimpleMath::Vector3 GetUp();
	// �ʒu�Z�b�g
	void SetPos(DirectX::SimpleMath::Vector3 pos);
	// ���_�Z�b�g
	void SetLook(DirectX::SimpleMath::Vector3 look);
	// �x�N�g���Z�b�g
	void SetUp(DirectX::SimpleMath::Vector3 up);

	// �ړ��{���擾
	float GetMoveVal() { return m_MoveVal; }
	// �ړ��{���Z�b�g
	void SetMoveVal(float _val) { m_MoveVal = _val; }
protected:
	// �ʒu�E���_�E�x�N�g���ϐ�
	DirectX::SimpleMath::Vector3 m_pos, m_look, m_up;
	// ����E�A�X�y�N�g��E�j�A�N���b�v�E�t�@�[�N���b�v
	float m_fovy, m_aspect, m_near, m_far;

private:
	// �X�e�[�g�ύX����
	void UpdateState();
	// ��]���䏈��
	void UpdateOrbit(Argument& arg);
	// ���s�ړ����䏈��
	void UpdateTrack(Argument& arg);
	// �Y�[�����䏈��
	void UpdateDolly(Argument& arg);
	// ���R�ړ�����
	void UpdateFlight(Argument& arg);

private:
	int m_state;		// ���݂̃X�e�[�g
	POINT m_oldPos;		// �O��̃}�E�X���W

	bool isBackCamera = false;	// �Ǐ]�J�������ǂ���

	float m_MoveVal = 2.0f;	// �ړ��{��
};
