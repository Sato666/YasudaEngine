//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "main.h"
#include "input.h"

// �L�[����
BYTE Input::m_OldKeyState[256];
BYTE Input::m_KeyState[256];
// �}�E�X
POINT Input::m_MousePos;
POINT Input::m_OldMousePos;
// Window���
HWND Input::m_Hwnd;

//-----------------------------------------------------------------------------
//      �V���O���g��.
//-----------------------------------------------------------------------------
Input* Input::Get()
{
	static Input instance;
	return &instance;
}
//-----------------------------------------------------------------------------
//      �����������ł�.
//-----------------------------------------------------------------------------
void Input::Init()
{
	memset( m_OldKeyState, 0, 256 );
	memset( m_KeyState, 0, 256 );
}
//-----------------------------------------------------------------------------
//      ��������ł�.
//-----------------------------------------------------------------------------
void Input::Uninit()
{
}
//-----------------------------------------------------------------------------
//      �X�V�����ł�.
//-----------------------------------------------------------------------------
void Input::Update()
{
	memcpy( m_OldKeyState, m_KeyState, 256 );

	GetKeyboardState( m_KeyState );
}
//-----------------------------------------------------------------------------
//      �L�[���͏����ł�.
//-----------------------------------------------------------------------------
bool Input::GetKeyPress(BYTE KeyCode)
{
	return (m_KeyState[KeyCode] & 0x80);
}
//-----------------------------------------------------------------------------
//      �L�[1f���͏����ł�.
//-----------------------------------------------------------------------------
bool Input::GetKeyTrigger(BYTE KeyCode)
{
	return ((m_KeyState[KeyCode] & 0x80) && !(m_OldKeyState[KeyCode] & 0x80));
}
//-----------------------------------------------------------------------------
//      �}�E�X���W�Z�b�g�����ł�.
//-----------------------------------------------------------------------------
void Input::OnMouseMove(int x, int y)
{
	m_MousePos.x = x;
	m_MousePos.y = y;
}
//-----------------------------------------------------------------------------
//      �}�E�X���W��Ԃ������ł�.
//-----------------------------------------------------------------------------
POINT Input::GetMousePos()
{
	return m_MousePos;
}
//-----------------------------------------------------------------------------
//      �}�E�X�ړ��x��Ԃ������ł�.
//-----------------------------------------------------------------------------
POINT Input::GetMouseMove()
{
	POINT move;
	move.x = m_MousePos.x - m_OldMousePos.x;
	move.y = m_MousePos.y - m_OldMousePos.y;
	return move;
}
//-----------------------------------------------------------------------------
//      �}�E�X���W��Window�̒����ɃZ�b�g���鏈���ł�.
//-----------------------------------------------------------------------------
void Input::SetMouseCenter()
{
	// �X�N���[����̃E�C���h�E�̈�̍��W���擾
	RECT winRect;
	GetWindowRect(m_Hwnd, &winRect);

	// �E�C���h�E�̒��S�ʒu���v�Z
	POINT mousePos;
	mousePos.x = winRect.left + (winRect.right - winRect.left) / 2;
	mousePos.y = winRect.top + (winRect.bottom - winRect.top) / 2;

	// �}�E�X�J�[�\�����E�C���h�E�̒��S�Ɉړ�������
	SetCursorPos(mousePos.x, mousePos.y);

	ScreenToClient(m_Hwnd, &mousePos);// �X�N���[�����W���N���C�A���g���W�ɕϊ�
	m_OldMousePos = m_MousePos = mousePos;// ����Z�b�g�����}�E�X�ʒu�������o�[�ϐ��ɕۑ�
}
//-----------------------------------------------------------------------------
//      �}�E�X���g���b�L���O���鏈���ł�.
//-----------------------------------------------------------------------------
void Input::EnableMouseTracking()
{
	TRACKMOUSEEVENT msEvent;
	msEvent.cbSize = sizeof(msEvent);
	msEvent.dwFlags = TME_LEAVE;
	msEvent.dwHoverTime = 0;
	msEvent.hwndTrack = m_Hwnd;
	TrackMouseEvent(&msEvent);
}
//-----------------------------------------------------------------------------
//      �}�E�X���g���b�L���O�������鏈���ł�.
//-----------------------------------------------------------------------------
void Input::DisableMouseTracking()
{
	TRACKMOUSEEVENT msEvent;
	msEvent.cbSize = sizeof(msEvent);
	msEvent.dwFlags = TME_CANCEL | TME_LEAVE;
	msEvent.dwHoverTime = 0;
	msEvent.hwndTrack = m_Hwnd;
	TrackMouseEvent(&msEvent);
}
//-----------------------------------------------------------------------------
//      �}�E�X�����������鏈���ł�.
//-----------------------------------------------------------------------------
void Input::Refresh()
{
	m_OldMousePos = m_MousePos;
}
