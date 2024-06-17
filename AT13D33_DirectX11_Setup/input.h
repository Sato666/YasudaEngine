//-------------------------------------------------------------------------------------
// Class : Input
// 
// ���͏������Ǘ�����N���X
// �V���O���g���̋@�\������
// 
// Yasuda.Aoto  2023.08.03
//-------------------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <Windows.h>

class Input
{
private:
	static BYTE m_OldKeyState[256];
	static BYTE m_KeyState[256];

	static POINT m_MousePos;
	static POINT m_OldMousePos;

	static HWND m_Hwnd;

	//�}�E�X�����߂��t���O
	bool m_MoseCenter = false;
	// �}�E�X�|�C���g�C���g��ԕϐ�
	float m_pointRepair = 10;

public:
	// �V���O���g���@�B��̃C���X�^���X��Ԃ�
	static Input* Get();

	static void Init();		// ������
	static void Uninit();	// ���
	static void Update();	// �X�V

	// �L�[����
	static bool GetKeyPress( BYTE KeyCode );
	// �L�[1f����
	static bool GetKeyTrigger( BYTE KeyCode );

	// �}�E�X�J�[�\���̈ʒu���Z�b�g����
	void OnMouseMove(int x, int y);
	// �}�E�X�J�[�\���̌��݂̃N���C�A���g���W���擾����
	POINT GetMousePos();
	// �O�t���[������̃}�E�X�ړ��ʂ��擾����
	POINT GetMouseMove();
	// �}�E�X�J�[�\�����E�C���h�E�̒��S�Ɉړ�������
	void SetMouseCenter();

	// Windows���b�Z�[�W�ŃE�C���h�E����}�E�X�J�[�\����
	// �o���ʒm���󂯎��
	void EnableMouseTracking();
	// ��̒ʒm���L�����Z������
	void DisableMouseTracking();
	// ���
	void Refresh();

	// HWND���Z�b�g����
	void Set_mHwnd(HWND _mHwnd) { m_Hwnd = _mHwnd; }
	// �}�E�X�𒆐S�_�ɖ߂����t���O�̃Z�b�^�[�E�Q�b�^�[
	void SetMouseCenter(bool IsMoseCenter) { m_MoseCenter = IsMoseCenter; }
	bool GetMouseCenter() { return m_MoseCenter; }
	// �}�E�X�|�C���g�̏C���g�ϐ��̃Q�b�^�[�Z�b�^�[
	void SetMousePointRepair(float _num) { m_pointRepair = _num; }  // set
	float GetMousePointRepair() { return m_pointRepair; }           // get
};
