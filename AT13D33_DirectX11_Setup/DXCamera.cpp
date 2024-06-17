//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "DXCamera.h"
//-------------------------------------------------------
// �I�v�V�����֌W
#include "main.h"
#include "renderer.h"
//-------------------------------------------------------
// ���͏��֌W
#include "input.h"
//-------------------------------------------------------
// �V�[���֌W
#include "manager.h"	// ���݂̃V�[�����擾���邽��
#include "camera.h"
//-----------------------------------------------------------------------------
//      �X�e�[�g
//-----------------------------------------------------------------------------
enum CameraDCCKind
{
	CAM_DCC_NONE,		// �J��������Ȃ�
	CAM_DCC_ORBIT,		// ��]����
	CAM_DCC_TRACK,		// ���s�ړ�����	
	CAM_DCC_DOLLY,		// �Y�[������
	CAM_DCC_FLIGHT,		// ���R�ړ�����
};
//-----------------------------------------------------------------------------
//      �����������ł�.
//-----------------------------------------------------------------------------
void DXCamera::Init()
{
	m_pos = (DirectX::XMFLOAT3(0.0f, 0.0f, -10.0f));									// �ʒu
	m_look = (DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));										// ���_
	m_up = (DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f));										// �x�N�g��
	m_fovy = (DirectX::XMConvertToRadians(60.0f));										// ����
	m_aspect = (static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT));	// �A�X�y�N�g��
	m_near = (0.01f);																	// �j�A�O���b�v
	m_far = (1000.0f);																	// �t�@�[�O���b�v

	m_state = (CAM_DCC_NONE);	// ���݂̏��
	m_oldPos = { 0, 0 };		// �}�E�X�̑O����W
}
//-----------------------------------------------------------------------------
//      �`�揈���ł�.
//-----------------------------------------------------------------------------
void DXCamera::Draw()
{
	DirectX::SimpleMath::Matrix mat;
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(m_look.x, m_look.y, m_look.z);
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(0.1f, 0.1f, 0.1f);
	mat = S * T;
	DirectX::XMFLOAT4X4 fmat;
	DirectX::XMStoreFloat4x4(&fmat, DirectX::XMMatrixTranspose(mat));		// �]�u

	//�v���W�F�N�V�����s��̐���
	DirectX::SimpleMath::Matrix projectionMatrix;
	projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(m_fovy, m_aspect, m_near, m_far);	// ����n�ɂ����@20230511 by suzuki.tomoki
}
//-----------------------------------------------------------------------------
//      �X�V�����ł�.
//-----------------------------------------------------------------------------
void DXCamera::Update()
{
	UpdateState();

	if (m_state == CAM_DCC_NONE) return;

	Argument arg;
	// �}�E�X�ړ��ʂ��擾����
	POINT cursorPos;
	cursorPos = Input::Get()->GetMouseMove();
	arg.mouseMove = DirectX::XMFLOAT2((float)cursorPos.x, (float)cursorPos.y);
	// �J�������
	arg.vCamPos = DirectX::XMLoadFloat3(&m_pos);
	arg.vCamLook = DirectX::XMLoadFloat3(&m_look);
	DirectX::XMVECTOR vCamUp = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&m_up));
	DirectX::XMVECTOR vFront = DirectX::XMVectorSubtract(arg.vCamLook, arg.vCamPos);
	// �J�����p��
	arg.vCamFront = DirectX::XMVector3Normalize(vFront);
	arg.vCamSide = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(vCamUp, arg.vCamFront));
	arg.vCamUp = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(arg.vCamFront, arg.vCamSide));
	// �t�H�[�J�X����
	DirectX::XMStoreFloat(&arg.focus, DirectX::XMVector3Length(vFront));
	//-----------------------------------------------------------------------------
	// ���ꂼ��̃X�e�[�g�̍X�V����������
	switch (m_state)
	{
	case CAM_DCC_ORBIT:		UpdateOrbit(arg);	break;
	case CAM_DCC_TRACK:		UpdateTrack(arg);	break;
	case CAM_DCC_DOLLY:		UpdateDolly(arg);	break;
	case CAM_DCC_FLIGHT:	UpdateFlight(arg);	break;
	}

	//-----------------------------------------------------------------------------
	// �Ō�Ɍv�Z�������ꂼ��̕ϐ���ϊ����ăr���[�}�g���b�N�X�ɒl��^����
	// 
	// XMFLOAT3��XMVECTOR�֕ϊ�
	DirectX::SimpleMath::Vector3 eye = DirectX::XMLoadFloat3(&m_pos);			// �ʒu
	DirectX::SimpleMath::Vector3 focus = DirectX::XMLoadFloat3(&m_look);		// ���_
	DirectX::SimpleMath::Vector3 up = DirectX::XMLoadFloat3(&m_up);				// �x�N�g��

	// �r���[�ϊ����쐬
	m_ViewMatrix = DirectX::XMMatrixLookAtLH(eye, focus, up);
}
//-----------------------------------------------------------------------------
//      �r���[��Ԃ�
//-----------------------------------------------------------------------------
DirectX::XMFLOAT4X4 DXCamera::GetView(bool transpose)
{
	DirectX::XMMATRIX mat = DirectX::XMMatrixLookAtLH(
		DirectX::XMLoadFloat3(&m_pos), DirectX::XMLoadFloat3(&m_look), DirectX::XMLoadFloat3(&m_up)
	);
	if (transpose)
		mat = DirectX::XMMatrixTranspose(mat);
	DirectX::XMFLOAT4X4 fmat;
	DirectX::XMStoreFloat4x4(&fmat, mat);
	return fmat;
}
//-----------------------------------------------------------------------------
//      �v���W�F�N�V������Ԃ�
//-----------------------------------------------------------------------------
DirectX::XMFLOAT4X4 DXCamera::GetProj(bool transpose)
{
	DirectX::XMMATRIX mat = DirectX::XMMatrixPerspectiveFovLH(m_fovy, m_aspect, m_near, m_far);
	if (transpose)
		mat = DirectX::XMMatrixTranspose(mat);
	DirectX::XMFLOAT4X4 fmat;
	DirectX::XMStoreFloat4x4(&fmat, mat);
	return fmat;
}
//-----------------------------------------------------------------------------
//      �ʒu��Ԃ�
//-----------------------------------------------------------------------------
DirectX::SimpleMath::Vector3 DXCamera::GetPos()
{
	return m_pos;
}
//-----------------------------------------------------------------------------
//      ���_��Ԃ�
//-----------------------------------------------------------------------------
DirectX::SimpleMath::Vector3 DXCamera::GetLook()
{
	return m_look;
}
//-----------------------------------------------------------------------------
//      �x�N�g����Ԃ�
//-----------------------------------------------------------------------------
DirectX::SimpleMath::Vector3 DXCamera::GetUp()
{
	return m_up;
}
//-----------------------------------------------------------------------------
//      �ʒu���Z�b�g����
//-----------------------------------------------------------------------------
void DXCamera::SetPos(DirectX::SimpleMath::Vector3 pos)
{
	m_pos = pos;
}
//-----------------------------------------------------------------------------
//      ���_���Z�b�g����
//-----------------------------------------------------------------------------
void DXCamera::SetLook(DirectX::SimpleMath::Vector3 look)
{
	m_look = look;
}
//-----------------------------------------------------------------------------
//      �x�N�g�����Z�b�g����
//-----------------------------------------------------------------------------
void DXCamera::SetUp(DirectX::SimpleMath::Vector3 up)
{
	m_up = up;
}
//-----------------------------------------------------------------------------
//      �X�e�[�g���Ƃ̍X�V����
//-----------------------------------------------------------------------------
void DXCamera::UpdateState()
{
	// ���݂̃X�e�[�g
	CameraDCCKind prev = (CameraDCCKind)m_state;

	// ALT + ����
	if (Input::GetKeyPress(VK_MENU))
	{
		m_state = CAM_DCC_NONE;

		// ����������
		if (Input::GetKeyPress(VK_SHIFT))
		{
			// ����������
			m_pos = (DirectX::XMFLOAT3(0.0f, 0.0f, -10.0f));									// �ʒu
			m_look = (DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));										// ���_
			m_up = (DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f));										// �x�N�g��
		}

		if (Input::GetKeyPress(VK_LBUTTON)) m_state = CAM_DCC_ORBIT;		// ��]����
		if (Input::GetKeyPress(VK_MBUTTON)) m_state = CAM_DCC_TRACK;		// ���s�ړ�����
		if (Input::GetKeyPress(VK_RBUTTON)) m_state = CAM_DCC_DOLLY;		// �Y�[������
	}
	else if (Input::GetKeyPress(VK_RBUTTON))
	{
		// ���N���b�N�������ꂽ�Ƃ�
		m_state = CAM_DCC_FLIGHT;										// ���R�ړ�
	}
	else
	{
		m_state = CAM_DCC_NONE;
	}
	if (prev != m_state)
	{
		// �}�E�X�̈ړ����L������
		GetCursorPos(&m_oldPos);
	}
}
//-----------------------------------------------------------------------------
//      ��]����X�V����
//-----------------------------------------------------------------------------
void DXCamera::UpdateOrbit(Argument& arg)
{
	// �}�E�X�̈ړ��� / ��ʃT�C�Y �̔䗦����A��ʑS�̂łǂꂾ����]���邩�w�肷��B
	float angleX = 360.0f * arg.mouseMove.x / SCREEN_WIDTH;
	float angleY = 180.0f * arg.mouseMove.y / SCREEN_HEIGHT;

	// Y����]�ŃJ�����̉�������]
	DirectX::XMMATRIX matRotY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(angleX));
	DirectX::XMVECTOR vAxis = DirectX::XMVector3Normalize(DirectX::XMVector3TransformCoord(arg.vCamSide, matRotY));

	// ��]��̎��ŏc��]
	DirectX::XMMATRIX matRotSide = DirectX::XMMatrixRotationAxis(vAxis, DirectX::XMConvertToRadians(angleY));
	DirectX::XMVECTOR vRelative = DirectX::XMVectorScale(arg.vCamFront, arg.focus);
	vRelative = DirectX::XMVector3TransformCoord(vRelative, matRotY * matRotSide);

	// �J�������W�X�V
	DirectX::XMStoreFloat3(&m_pos, DirectX::XMVectorSubtract(arg.vCamLook, vRelative));

	// �A�b�v�x�N�g���X�V
	DirectX::XMStoreFloat3(&m_up, DirectX::XMVector3Normalize(DirectX::XMVector3Cross(DirectX::XMVector3Normalize(vRelative), vAxis)));
}
//-----------------------------------------------------------------------------
//      ���s����X�V����
//-----------------------------------------------------------------------------
void DXCamera::UpdateTrack(Argument& arg)
{
	// ����A�A���B�Ƃ���O�p�`�ɂ��� tan�� = A / B�����藧��
	// ��L�������ƂɊ���o�������i�̍������A�ړ��� / ��ʃT�C�Y �̔䗦����A�ړ��ʂƂ��ċ��߂�
	float farScreenHeight = tanf(m_fovy * 0.5f) * m_far;
	float screenRateW = arg.mouseMove.x / 640.0f;
	float screenRateH = arg.mouseMove.y / 360.0f;
	float farMoveX = -farScreenHeight * screenRateW * m_aspect;
	float farMoveY = farScreenHeight * screenRateH;

	// �J�����̎p�������ƂɈړ�
	float rate = arg.focus / m_far;
	DirectX::XMVECTOR vCamMove = DirectX::XMVectorZero();
	vCamMove = DirectX::XMVectorAdd(vCamMove, DirectX::XMVectorScale(arg.vCamSide, farMoveX * rate));
	vCamMove = DirectX::XMVectorAdd(vCamMove, DirectX::XMVectorScale(arg.vCamUp, farMoveY * rate));
	DirectX::XMStoreFloat3(&m_pos, DirectX::XMVectorAdd(arg.vCamPos, vCamMove));
	DirectX::XMStoreFloat3(&m_look, DirectX::XMVectorAdd(arg.vCamLook, vCamMove));
}
//-----------------------------------------------------------------------------
//      �Y�[������X�V����
//-----------------------------------------------------------------------------
void DXCamera::UpdateDolly(Argument& arg)
{
	float rate = arg.focus / m_far;

	DirectX::XMVECTOR vMove = DirectX::XMVectorScale(arg.vCamFront, m_far * 0.01f * rate * (arg.mouseMove.x + arg.mouseMove.y));
	DirectX::XMStoreFloat3(&m_pos, DirectX::XMVectorAdd(arg.vCamPos, vMove));

	//printf("%f \n", arg.focus);
}
//-----------------------------------------------------------------------------
//      ���R�ړ��X�V����
//-----------------------------------------------------------------------------
void DXCamera::UpdateFlight(Argument& arg)
{
	// �}�E�X�̈ړ��� / ��ʃT�C�Y �̔䗦����A��ʑS�̂łǂꂾ����]���邩�w�肷��B
	float angleX = 360.0f * arg.mouseMove.x / SCREEN_WIDTH;
	float angleY = 180.0f * arg.mouseMove.y / SCREEN_HEIGHT;

	// ����]
	DirectX::XMMATRIX matUpRot = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(angleX));
	DirectX::XMVECTOR vSideAxis = DirectX::XMVector3Normalize(DirectX::XMVector3TransformCoord(arg.vCamSide, matUpRot));

	// �c��]
	DirectX::XMMATRIX matSideRot = DirectX::XMMatrixRotationAxis(vSideAxis, DirectX::XMConvertToRadians(angleY));
	DirectX::XMVECTOR vFrontAxis = DirectX::XMVector3Normalize(DirectX::XMVector3TransformCoord(arg.vCamFront, matUpRot * matSideRot));

	// �L�[���͂ňړ�
	DirectX::XMVECTOR vCamMove = DirectX::XMVectorZero();
	if (Input::GetKeyPress('W')) vCamMove = DirectX::XMVectorAdd(vCamMove, vFrontAxis);
	if (Input::GetKeyPress('S')) vCamMove = DirectX::XMVectorSubtract(vCamMove, vFrontAxis);
	if (Input::GetKeyPress('A')) vCamMove = DirectX::XMVectorSubtract(vCamMove, vSideAxis);
	if (Input::GetKeyPress('D')) vCamMove = DirectX::XMVectorAdd(vCamMove, vSideAxis);
	if (Input::GetKeyPress('Q')) vCamMove = DirectX::XMVectorAdd(vCamMove, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	if (Input::GetKeyPress('E')) vCamMove = DirectX::XMVectorAdd(vCamMove, DirectX::XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f));

	// ����������
	if (Input::GetKeyPress(VK_SHIFT))
	{
		vCamMove = DirectX::XMVectorScale(vCamMove, 0.1f * m_MoveVal);		// �ړ� * �{��
	}
	else
	{
		vCamMove = DirectX::XMVectorScale(vCamMove, 0.1f);					// �ʏ�ړ�
	}

	// �X�V
	DirectX::XMVECTOR vCamPos = DirectX::XMVectorAdd(arg.vCamPos, vCamMove);
	DirectX::XMStoreFloat3(&m_pos, vCamPos);
	DirectX::XMStoreFloat3(&m_look, DirectX::XMVectorAdd(vCamPos, DirectX::XMVectorScale(vFrontAxis, arg.focus)));
	DirectX::XMStoreFloat3(&m_up, DirectX::XMVector3Normalize(DirectX::XMVector3Cross(vFrontAxis, vSideAxis)));
}