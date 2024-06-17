//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Application.h"
#include <chrono>
#include <thread>
#include <Windows.h>
#include <windowsx.h>
//-----------------------------------------------------------------------------
// 必要な機能
#include "fpscontrol.h"     // FPS
#include "manager.h"
#include "input.h"          // 入力処理
#include "main.h"
#include "renderer.h"
//-----------------------------------------------------------------------------
// ImGui
#include "imgui_impl_win32.h"

// グローバル変数
Manager* manager{};
//-----------------------------------------------------------------------------
// Constant Values.
//-----------------------------------------------------------------------------
const auto ClassName = TEXT("AT13D_33 YasudaAoto");     //!< ウィンドウクラス名.
auto WindowName = TEXT("YasudaEngine");    //!< ウィンドウ名.

///////////////////////////////////////////////////////////////////////////////
// App class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      コンストラクタです.
//-----------------------------------------------------------------------------
Application::Application(uint32_t width, uint32_t height)
: m_hInst   (nullptr)
, m_hWnd    (nullptr)
, m_Width   (width)
, m_Height  (height)
{ /* DO_NOTHING */ }

//-----------------------------------------------------------------------------
//      デストラクタです.
//-----------------------------------------------------------------------------
Application::~Application()
{ /* DO_NOTHING */ }

//-----------------------------------------------------------------------------
//      実行します.
//-----------------------------------------------------------------------------
void Application::Run()
{
    // 初期化処理
    if (InitApp())
    { 
        MainLoop(); 
    }

    // 終了処理
    TermApp();
}

//-----------------------------------------------------------------------------
//      初期化処理です.
//-----------------------------------------------------------------------------
bool Application::InitApp()
{
    // ウィンドウの初期化.
    if (!InitWnd())
    { 
        // エラー
        return false; 
    }

    // 正常終了.
    return true;
}

//-----------------------------------------------------------------------------
//      終了処理です.
//-----------------------------------------------------------------------------
void Application::TermApp()
{
    // ウィンドウの終了処理.
    TermWnd();
}

//-----------------------------------------------------------------------------
//      ウィンドウの初期化処理です.
//-----------------------------------------------------------------------------
bool Application::InitWnd()
{
    // インスタンスハンドルを取得.
    auto hInst = GetModuleHandle(nullptr);
    if (hInst == nullptr)
    { 
        return false; 
    }

    // ウィンドウの設定.
    WNDCLASSEX wc = {};
    wc.cbSize           = sizeof(WNDCLASSEX);
    wc.style            = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc      = WndProc;
    wc.hIcon            = LoadIcon(hInst, IDI_APPLICATION);
    wc.hCursor          = LoadCursor(hInst, IDC_ARROW);
    wc.hbrBackground    = GetSysColorBrush(COLOR_BACKGROUND);
    wc.lpszMenuName     = nullptr;
    wc.lpszClassName    = ClassName;
    wc.hIconSm          = LoadIcon(hInst, IDI_APPLICATION);

    // ウィンドウの登録.
    if (!RegisterClassEx(&wc))
    { return false; }

    // インスタンスハンドル設定.
    m_hInst = hInst;

    // ウィンドウのサイズを設定.
    RECT rc = {};
    rc.right = static_cast<LONG>(m_Width);
    rc.bottom = static_cast<LONG>(m_Height);

    // ウィンドウサイズを調整.
    auto style = WS_POPUP | WS_CAPTION | WS_SYSMENU;
    AdjustWindowRect(&rc, style, FALSE);

    // ウィンドウを生成.
    m_hWnd = CreateWindowEx(
        0,
        ClassName,      // クラス名
        WindowName,     // Windowクラス名
        style,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        rc.right  - rc.left,
        rc.bottom - rc.top,
        nullptr,
        nullptr,
        m_hInst,
        nullptr);

    // 作成できないとfalseを返す
    if (m_hWnd == nullptr)
    { return false; }

    // ウィンドウを表示.
    ShowWindow(m_hWnd, SW_SHOWNORMAL);

    // ウィンドウをフォアグラウンドにする。
    SetForegroundWindow(m_hWnd);

    // ウィンドウを更新.
    UpdateWindow(m_hWnd);

    // ウィンドウにフォーカスを設定.
    SetFocus(m_hWnd);

    // Inputにウィンドウ情報を与える
    Input::Get()->Set_mHwnd(m_hWnd);

    // 正常終了.
    return true;
}

//-----------------------------------------------------------------------------
//      ウィンドウの終了処理です.
//-----------------------------------------------------------------------------
void Application::TermWnd()
{
    // ウィンドウの登録を解除.
    if (m_hInst != nullptr)
    { UnregisterClass(ClassName, m_hInst); }

    m_hInst = nullptr;
    m_hWnd  = nullptr;
}

//-----------------------------------------------------------------------------
//      メインループです.
//-----------------------------------------------------------------------------
void Application::MainLoop()
{
    MSG msg = {};

    // FPS調整クラス
    FPS fpsrate(100);

    // マネージャーの初期化
    manager->Init(this);

    // 一旦フルスクリーンにしてからウィンドウモードにする
    // Windows設定のスクリーン解像度の倍率を100%に戻すため
    Renderer::GetSwapChain()->SetFullscreenState(TRUE, NULL);
    Renderer::GetSwapChain()->SetFullscreenState(FALSE, NULL);

    ImGui::SetNextWindowSize(ImVec2(m_Width, m_Height), ImGuiCond_FirstUseEver);

    while(WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) == TRUE)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            // 開始してからの時間格納変数
            uint64_t delta_time;

            // デルタタイムを計算
            delta_time = fpsrate.CalcDelta();

            // マネージャーのアップデート
            manager->Update(delta_time);

            // マネージャー描画
            manager->Draw(delta_time);

            // 入力情報のリフレッシュ
            Input::Get()->Refresh();

            // 規定時間まで時間調整
            fpsrate.Wait();
        }
    }

    // マネージャーの解放
    manager->Uninit();
}


#ifndef WM_DPICHANGED
#define WM_DPICHANGED 0x02E0 // From Windows SDK 8.1+ headers
#endif

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

//-----------------------------------------------------------------------------
//      ウィンドウプロシージャです.
//-----------------------------------------------------------------------------
LRESULT CALLBACK Application::WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    // ImGuiプロシージャ
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wp, lp)) { return true; }

    // マウスが外に出たフラグ
    static bool IsMouseOut = false;

    switch(msg)
    {
        case WM_DESTROY:
            { PostQuitMessage(0); }
            break;
        case WM_DPICHANGED:
            if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
            {
                const RECT* suggested_rect = (RECT*)lp;
                ::SetWindowPos(hWnd, nullptr, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
            }
            break;
        case WM_SYSCOMMAND:
            if ((wp & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
                return 0;
            break;
        case WM_KEYDOWN:
            // キーが押された時のリアクションを書く
            // ESCが押されたのかどうかチェック
            if (LOWORD(wp) == VK_ESCAPE)
            {
                // メッセージボックスで修了確認
                int result;
                result = MessageBox(NULL, "終了してよろしいですか？",
                    "終了確認", MB_YESNO | MB_ICONQUESTION);
                if (result == IDYES) // 「はい」ボタンが押された時
                {
                    // xボタンが押されたのと同じ効果を発揮する
                    PostMessage(hWnd, WM_CLOSE, wp, lp);
                }
            }
            break;
        case WM_MOUSEMOVE: // マウスカーソルが動いたとき
            Input::Get()->OnMouseMove(GET_X_LPARAM(lp), GET_Y_LPARAM(lp));

            // マウスの場所を取得する
            POINT mousePos = Input::Get()->GetMousePos();

            if (Input::Get()->GetMouseCenter() == true)
            {
                // マウスが範囲を超えればマウスを中央に戻す
                if (    mousePos.x <= SCREEN_WIDTH  - SCREEN_WIDTH + Input::Get()->GetMousePointRepair()
                    ||  mousePos.x >= SCREEN_WIDTH  - Input::Get()->GetMousePointRepair()
                    ||  mousePos.y <= SCREEN_HEIGHT - SCREEN_HEIGHT + Input::Get()->GetMousePointRepair()
                    ||  mousePos.y >= SCREEN_HEIGHT - Input::Get()->GetMousePointRepair())
                {
                    Input::Get()->SetMouseCenter();
                }
                // マウスが外に出て帰ってきた
                if (IsMouseOut) 
                {
                    Input::Get()->EnableMouseTracking();	// 再度トラッキング
                    IsMouseOut = false;
                }
            }
            break;

        case WM_MOUSELEAVE:
            // マウスがウィンドウ外に出た
            if (Input::Get()->GetMouseCenter() == true)
            {
                Input::Get()->SetMouseCenter();
                IsMouseOut = true;
            }
            break;
        default:
            { /* DO_NOTHING */ }
            break;
    }

    return DefWindowProc(hWnd, msg, wp, lp);
}
