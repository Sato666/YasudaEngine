//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include	"manager.h"

// �I�v�V�����֌W
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

// �V�[���}�l�[�W���������o�ϐ��̎���
std::shared_ptr<SceneMain> Manager::m_pScene{};
// �E�B���h�E�n���h��
HWND g_Hwnd;
//-----------------------------------------------------------------------------
//      �����������ł�.
//-----------------------------------------------------------------------------
void Manager::Init(Application* ap)
{
	// �^�C�}�𑜓x���P�~���b�ɂ���
	timeBeginPeriod(1);

	// �����_���[��������
	Renderer::Init(ap->GetWindow(), ap->GetWidth(), ap->GetHeight(), false);

	// �W�I���g��������
	Geometory::Init();

	// �X�v���C�g������
	Sprite::Init();

	// ��������
	Audio::InitMaster();

	// ���͂�������
	Input::Get()->Init();

	// �V�[���쐬
	m_pScene = std::make_shared<SceneMain>();
	m_pScene->Init();

	// �������\�[�X�쐬
	auto rtv = m_pScene->CreateObj<RenderTarget>("RTV");
	rtv->CreateFromScreen();
	auto dsv = m_pScene->CreateObj<DepthStencil>("DSV");
	dsv->Create(ap->GetWidth(), ap->GetHeight(), false);

	// �����_�[�^�[�Q�b�g�ݒ�
	Renderer::SetRenderTargets(1, &rtv, dsv);

	// ImGui������
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports;
	// �t�H���g�ǂݍ���(�f�t�H14.0f)
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

	g_Hwnd = ap->GetWindow();	// Windows�n���h����n��
}
//-----------------------------------------------------------------------------
//      ��������ł�.
//-----------------------------------------------------------------------------
void Manager::Uninit()
{
	// �V�[���̑S���
	m_pScene->UninitBase();
	m_pScene.reset();

	// �R�c���f����S�ĉ��
	ModelRenderer::UnloadAll();

	// �X�v���C�g�I��
	Sprite::Uninit();

	// �W�I���g���I��
	Geometory::Uninit();

	// �C���v�b�g�I��
	Input::Uninit();

	// �I�[�f�B�I�I��
	Audio::UninitMaster();

	// ImGui�̉��
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// �����_���[�I��
	Renderer::Uninit();

	// �^�C�}�𑜓x�����ɖ߂�
	timeEndPeriod(1);
}
//-----------------------------------------------------------------------------
//      �X�V�����ł�.
//-----------------------------------------------------------------------------
void Manager::Update(uint64_t d)
{
	Input::Update();

	m_pScene->UpdateBase((float)d);
}
//-----------------------------------------------------------------------------
//      �`�揈���ł�.
//-----------------------------------------------------------------------------
void Manager::Draw(uint64_t d)
{
	// �����_�[�^�[�Q�b�g�Ɛ[�x�e�N�X�`�����擾
	auto rtv = m_pScene->GetObj<RenderTarget>("RTV");
	auto dsv = m_pScene->GetObj<DepthStencil>("DSV");
	float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	// �Q�b�^�[�Œ��ړn���ƂȂ���1f�ȍ~�ɃG���[����������̂ŉ��p
	auto _rtv = rtv->GetView();
	// �[�x�X�e���V���o�b�t�@���o�C���h **-(�}���`�r���[�|�[�g�ɕύX������ꍇ����鏈��)
	Renderer::GetDeviceContext()->OMSetRenderTargets(1, &_rtv, dsv->GetView());
	rtv->Clear(color);
	dsv->Clear();

	// �V�[���̕`��
	m_pScene->DrawBase();

	// ImGui�̏���
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// �e�V�[����ImGui�̕`��
	m_pScene->ImGuiBase();

	// �S�V�[�����ʂ�ImGui�̕`��
	MyImGui();

	// �I������
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	// Update and Render additional Platform Windows
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	// ��ʂ̃X���b�v���s
	Renderer::SwapEnd();
}
//-----------------------------------------------------------------------------
//      �S�V�[�����ʂ�ImGui�̕`�揈���ł�.
//-----------------------------------------------------------------------------
void Manager::MyImGui()
{
	static bool show_fps_window = true;
	static bool show_setting_window = true;
	// �o�[��`��
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu(u8"���ʍ���"))
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
	// FPS�\��
	if (show_fps_window)
	{
		// �I�����ꂽ���̐F
		ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
		// �I�����Ă��Ȃ����̐F
		ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(1.0f, 0.0f, 1.0f, 1.0f));

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		// FPS�`��
		ImGui::Begin("FPS", &show_fps_window);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::Text("%d vertices, %d indices (%d triangles)", io.MetricsRenderVertices, io.MetricsRenderIndices, io.MetricsRenderIndices / 3);
		ImGui::End();
		// �F�����Z�b�g
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
	}
	//-----------------------------------------------------------------------------
	// ���ʐݒ荀��
	if(show_setting_window)
	{
		ImGui::Begin(u8"���ʐݒ荀��", &show_setting_window);
		//-----------------------------------------------------------------------------
		// �}�E�X�ݒ�
		if (ImGui::TreeNode(u8"�}�E�X�ݒ�"))
		{
			// �}�E�X�̈ʒu
			ImGui::Text(u8"MousePos	X : %d Y : %d", Input::Get()->GetMousePos().x, Input::Get()->GetMousePos().y);

			// �}�E�X�𒆉��ɖ߂��[�̕���ݒ�
			static float pos = 10;
			ImGui::InputFloat(u8"�����ɖ߂���", &pos, 0.5f);
			// �}�E�X�|�C���g�C�����ɐ��l���Z�b�g����
			Input::Get()->SetMousePointRepair(pos);

			ImGui::TreePop();
		}
		//-----------------------------------------------------------------------------
		// �E�B���h�E�r���[�ݒ�
		if (ImGui::TreeNode(u8"�E�B���h�E�ݒ�"))
		{
			// �E�B���h�E����Ɏ�O�ɂ��邩�ǂ���
			static bool isTopWindow = false;
			ImGui::Checkbox(u8"��O�ɕ\��", &isTopWindow);
			// �}�E�X�𒆉��ɖ߂����߂��Ȃ����̏���
			if (!isTopWindow)
				SetWindowPos(g_Hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);				// ��O�ɕ\������
			else
				SetWindowPos(g_Hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);				// ��Ɏ�O�ɕ\��

			ImGui::TreePop();
		}

		ImGui::End();
	}
}
