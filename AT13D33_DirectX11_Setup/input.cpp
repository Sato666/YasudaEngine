//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "main.h"
#include "input.h"

// キー入力
BYTE Input::m_OldKeyState[256];
BYTE Input::m_KeyState[256];
// マウス
POINT Input::m_MousePos;
POINT Input::m_OldMousePos;
// Window情報
HWND Input::m_Hwnd;

//-----------------------------------------------------------------------------
//      シングルトン.
//-----------------------------------------------------------------------------
Input* Input::Get()
{
	static Input instance;
	return &instance;
}
//-----------------------------------------------------------------------------
//      初期化処理です.
//-----------------------------------------------------------------------------
void Input::Init()
{
	memset( m_OldKeyState, 0, 256 );
	memset( m_KeyState, 0, 256 );
}
//-----------------------------------------------------------------------------
//      解放処理です.
//-----------------------------------------------------------------------------
void Input::Uninit()
{
}
//-----------------------------------------------------------------------------
//      更新処理です.
//-----------------------------------------------------------------------------
void Input::Update()
{
	memcpy( m_OldKeyState, m_KeyState, 256 );

	GetKeyboardState( m_KeyState );
}
//-----------------------------------------------------------------------------
//      キー入力処理です.
//-----------------------------------------------------------------------------
bool Input::GetKeyPress(BYTE KeyCode)
{
	return (m_KeyState[KeyCode] & 0x80);
}
//-----------------------------------------------------------------------------
//      キー1f入力処理です.
//-----------------------------------------------------------------------------
bool Input::GetKeyTrigger(BYTE KeyCode)
{
	return ((m_KeyState[KeyCode] & 0x80) && !(m_OldKeyState[KeyCode] & 0x80));
}
//-----------------------------------------------------------------------------
//      マウス座標セット処理です.
//-----------------------------------------------------------------------------
void Input::OnMouseMove(int x, int y)
{
	m_MousePos.x = x;
	m_MousePos.y = y;
}
//-----------------------------------------------------------------------------
//      マウス座標を返す処理です.
//-----------------------------------------------------------------------------
POINT Input::GetMousePos()
{
	return m_MousePos;
}
//-----------------------------------------------------------------------------
//      マウス移動度を返す処理です.
//-----------------------------------------------------------------------------
POINT Input::GetMouseMove()
{
	POINT move;
	move.x = m_MousePos.x - m_OldMousePos.x;
	move.y = m_MousePos.y - m_OldMousePos.y;
	return move;
}
//-----------------------------------------------------------------------------
//      マウス座標をWindowの中央にセットする処理です.
//-----------------------------------------------------------------------------
void Input::SetMouseCenter()
{
	// スクリーン上のウインドウ領域の座標を取得
	RECT winRect;
	GetWindowRect(m_Hwnd, &winRect);

	// ウインドウの中心位置を計算
	POINT mousePos;
	mousePos.x = winRect.left + (winRect.right - winRect.left) / 2;
	mousePos.y = winRect.top + (winRect.bottom - winRect.top) / 2;

	// マウスカーソルをウインドウの中心に移動させる
	SetCursorPos(mousePos.x, mousePos.y);

	ScreenToClient(m_Hwnd, &mousePos);// スクリーン座標をクライアント座標に変換
	m_OldMousePos = m_MousePos = mousePos;// 今回セットしたマウス位置をメンバー変数に保存
}
//-----------------------------------------------------------------------------
//      マウスをトラッキングする処理です.
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
//      マウスをトラッキング解除する処理です.
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
//      マウスを初期化する処理です.
//-----------------------------------------------------------------------------
void Input::Refresh()
{
	m_OldMousePos = m_MousePos;
}
