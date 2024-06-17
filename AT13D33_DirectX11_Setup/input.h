//-------------------------------------------------------------------------------------
// Class : Input
// 
// 入力処理を管理するクラス
// シングルトンの機能を持つ
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

	//マウス中央戻すフラグ
	bool m_MoseCenter = false;
	// マウスポイント修正枠補間変数
	float m_pointRepair = 10;

public:
	// シングルトン　唯一のインスタンスを返す
	static Input* Get();

	static void Init();		// 初期化
	static void Uninit();	// 解放
	static void Update();	// 更新

	// キー入力
	static bool GetKeyPress( BYTE KeyCode );
	// キー1f入力
	static bool GetKeyTrigger( BYTE KeyCode );

	// マウスカーソルの位置をセットする
	void OnMouseMove(int x, int y);
	// マウスカーソルの現在のクライアント座標を取得する
	POINT GetMousePos();
	// 前フレームからのマウス移動量を取得する
	POINT GetMouseMove();
	// マウスカーソルをウインドウの中心に移動させる
	void SetMouseCenter();

	// Windowsメッセージでウインドウからマウスカーソルが
	// 出た通知を受け取る
	void EnableMouseTracking();
	// 上の通知をキャンセルする
	void DisableMouseTracking();
	// 解放
	void Refresh();

	// HWNDをセットする
	void Set_mHwnd(HWND _mHwnd) { m_Hwnd = _mHwnd; }
	// マウスを中心点に戻すかフラグのセッター・ゲッター
	void SetMouseCenter(bool IsMoseCenter) { m_MoseCenter = IsMoseCenter; }
	bool GetMouseCenter() { return m_MoseCenter; }
	// マウスポイントの修正枠変数のゲッターセッター
	void SetMousePointRepair(float _num) { m_pointRepair = _num; }  // set
	float GetMousePointRepair() { return m_pointRepair; }           // get
};
