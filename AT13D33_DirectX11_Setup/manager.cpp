//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include	"manager.h"

// オプション関係
#include	"main.h"
#include	"Application.h"
#include	"renderer.h"
#include	"audio.h"
#include	"input.h"
#include	"ModelRenderer.h"
#include	"Geometory.h"
#include	"Sprite.h"

// imgui
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include <iostream>
#include <string>
#include <map>

// シーンマネージャ内メンバ変数の実体
std::shared_ptr<SceneMain> Manager::m_pScene{};
// ウィンドウハンドル
HWND g_Hwnd;
//-----------------------------------------------------------------------------
//      初期化処理です.
//-----------------------------------------------------------------------------
void Manager::Init(Application* ap)
{
	// タイマ解像度を１ミリ秒にする
	timeBeginPeriod(1);

	// レンダラーを初期化
	Renderer::Init(ap->GetWindow(), ap->GetWidth(), ap->GetHeight(), false);

	// ジオメトリ初期化
	Geometory::Init();

	// スプライト初期化
	Sprite::Init();

	// 音初期化
	Audio::InitMaster();

	// 入力を初期化
	Input::Get()->Init();

	// シーン作成
	m_pScene = std::make_shared<SceneMain>();
	m_pScene->Init();

	// 初期リソース作成
	auto rtv = m_pScene->CreateObj<RenderTarget>("RTV");
	rtv->CreateFromScreen();
	auto dsv = m_pScene->CreateObj<DepthStencil>("DSV");
	dsv->Create(ap->GetWidth(), ap->GetHeight(), false);

	// レンダーターゲット設定
	Renderer::SetRenderTargets(1, &rtv, dsv);

	// ImGui初期化
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports;
	// フォント読み込み(デフォ14.0f)
	io.Fonts->AddFontFromFileTTF("asset//font//UDDigiKyokashoN-R.ttc", 16.0f, nullptr,
		io.Fonts->GetGlyphRangesJapanese());
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(ap->GetWindow());
	ImGui_ImplDX11_Init(Renderer::GetDevice(), Renderer::GetDeviceContext());

	g_Hwnd = ap->GetWindow();	// Windowsハンドルを渡す
}
//-----------------------------------------------------------------------------
//      解放処理です.
//-----------------------------------------------------------------------------
void Manager::Uninit()
{
	// シーンの全解放
	m_pScene->UninitBase();
	m_pScene.reset();

	// ３Ｄモデルを全て解放
	ModelRenderer::UnloadAll();

	// スプライト終了
	Sprite::Uninit();

	// ジオメトリ終了
	Geometory::Uninit();

	// インプット終了
	Input::Uninit();

	// オーディオ終了
	Audio::UninitMaster();

	// ImGuiの解放
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// レンダラー終了
	Renderer::Uninit();

	// タイマ解像度を元に戻す
	timeEndPeriod(1);
}
//-----------------------------------------------------------------------------
//      更新処理です.
//-----------------------------------------------------------------------------
void Manager::Update(uint64_t d)
{
	Input::Update();

	m_pScene->UpdateBase((float)d);
}
//-----------------------------------------------------------------------------
//      描画処理です.
//-----------------------------------------------------------------------------
void Manager::Draw(uint64_t d)
{
	// レンダーターゲットと深度テクスチャを取得
	auto rtv = m_pScene->GetObj<RenderTarget>("RTV");
	auto dsv = m_pScene->GetObj<DepthStencil>("DSV");
	float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	// ゲッターで直接渡すとなぜか1f以降にエラーが発生するので回避用
	auto _rtv = rtv->GetView();
	// 深度ステンシルバッファをバインド **-(マルチビューポートに変更がある場合入れる処理)
	Renderer::GetDeviceContext()->OMSetRenderTargets(1, &_rtv, dsv->GetView());
	rtv->Clear(color);
	dsv->Clear();

	// シーンの描画
	m_pScene->DrawBase();

	// ImGuiの処理
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// 各シーンのImGuiの描画
	m_pScene->ImGuiBase();

	// 全シーン共通のImGuiの描画
	MyImGui();

	// 終了処理
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	// Update and Render additional Platform Windows
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	// 画面のスワップ実行
	Renderer::SwapEnd();
}
//-----------------------------------------------------------------------------
//      全シーン共通のImGuiの描画処理です.
//-----------------------------------------------------------------------------
void Manager::MyImGui()
{
	static bool show_fps_window = true;
	static bool show_setting_window = true;
	// バーを描画
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu(u8"共通項目"))
		{
			if (ImGui::BeginMenu(u8"Active ImGuiWindow"))
			{
				ImGui::Checkbox(u8"SceneEdit Window", &show_fps_window);
				ImGui::Checkbox(u8"ObjectList Window", &show_setting_window);
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	//-----------------------------------------------------------------------------
	// FPS表示
	if (show_fps_window)
	{
		// 選択された時の色
		ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
		// 選択していない時の色
		ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(1.0f, 0.0f, 1.0f, 1.0f));

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		// FPS描画
		ImGui::Begin("FPS", &show_fps_window);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::Text("%d vertices, %d indices (%d triangles)", io.MetricsRenderVertices, io.MetricsRenderIndices, io.MetricsRenderIndices / 3);
		ImGui::End();
		// 色をリセット
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
	}
	//-----------------------------------------------------------------------------
	// 共通設定項目
	if(show_setting_window)
	{
		ImGui::Begin(u8"共通設定項目", &show_setting_window);
		//-----------------------------------------------------------------------------
		// マウス設定
		if (ImGui::TreeNode(u8"マウス設定"))
		{
			// マウスの位置
			ImGui::Text(u8"MousePos	X : %d Y : %d", Input::Get()->GetMousePos().x, Input::Get()->GetMousePos().y);

			// マウスを中央に戻す端の幅を設定
			static float pos = 10;
			ImGui::InputFloat(u8"中央に戻す幅", &pos, 0.5f);
			// マウスポイント修正幅に数値をセットする
			Input::Get()->SetMousePointRepair(pos);

			ImGui::TreePop();
		}
		//-----------------------------------------------------------------------------
		// ウィンドウビュー設定
		if (ImGui::TreeNode(u8"ウィンドウ設定"))
		{
			// ウィンドウを常に手前にするかどうか
			static bool isTopWindow = false;
			ImGui::Checkbox(u8"手前に表示", &isTopWindow);
			// マウスを中央に戻すか戻さないかの処理
			if (!isTopWindow)
				SetWindowPos(g_Hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);				// 手前に表示解除
			else
				SetWindowPos(g_Hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);				// 常に手前に表示

			ImGui::TreePop();
		}

		ImGui::End();
	}
}
