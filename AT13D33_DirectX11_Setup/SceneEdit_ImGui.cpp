//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "SceneEdit.h"

// ������֌W
#include <sstream>
#include <iostream>
#include "strconv2.h"

// ������ϊ��֌W
#include "utility.h"

// ���̃V�[���Ŏg��GameObject�C���N���[�h
#include "field.h"		// �t�B�[���h
#include "Box.h"		// �{�b�N�X
#include "Cylinder.h"	// �V�����_�[
#include "field.h"		// �t�B�[���h
#include "MoveLight.h"	// ���C�g

void SceneEdit::ImGui()
{
	// �o�[��`��
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("SceneEdit")) {
			if (ImGui::BeginMenu(u8"Active ImGuiWindow")) {
				ImGui::Checkbox(u8"SceneEdit Window", &m_ShowSceneEditWindow);			// ���̃V�[���̋��ʐݒ�E�B���h�E
				ImGui::Checkbox(u8"UserGuide Window", &m_ShowUserGuideWindow);			// ��������E�B���h�E
				ImGui::Checkbox(u8"CreateObject Window", &m_ShowAddGameObejctsWindow);	// �I�u�W�F�N�g�̐����E�B���h�E
				ImGui::Checkbox(u8"ObjectList Window", &m_ShowGameObjectListWindow);	// ���������I�u�W�F�N�g�̈ꗗ�E�B���h�E
				ImGui::Checkbox(u8"Instance Window", &m_ShowInstanceWindow);			// ���������I�u�W�F�N�g�̏ڍאݒ�E�B���h�E
				ImGui::Checkbox(u8"SceneScreen Window", &m_ShowScreenWindow);			// �X�N���[���E�B���h�E
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	// �eImGui�E�B���h�E�`��
	if (m_ShowSceneEditWindow)
		ImGui_UI_SceneEdit(&m_ShowSceneEditWindow);				// ���̃V�[���̋��ʐݒ�E�B���h�E����
	if (m_ShowUserGuideWindow)
		ImGui_UI_UserGuide(&m_ShowUserGuideWindow);				// ��������E�B���h�E����
	if (m_ShowAddGameObejctsWindow)
		ImGui_UI_AddGameObejcts(&m_ShowAddGameObejctsWindow);	// �I�u�W�F�N�g�̐����E�B���h�E����
	if (m_ShowGameObjectListWindow)
		ImGui_UI_GameObjectList(&m_ShowGameObjectListWindow);	// ���������I�u�W�F�N�g�̈ꗗ�E�B���h�E����
	if (m_ShowInstanceWindow)
		ImGui_UI_Instance(&m_ShowInstanceWindow);				// ���������I�u�W�F�N�g�̏ڍאݒ�E�B���h�E����
	if (m_ShowScreenWindow)
		ImGui_UI_Screen(&m_ShowScreenWindow);					// �X�N���[���E�B���h�E����
}


/// <summary>
/// ���̃V�[���̋��ʐݒ�
/// </summary>
/// <param name="p_open">�E�B���h�E��\�����邩</param>
void SceneEdit::ImGui_UI_SceneEdit(bool* p_open)
{
	if (ImGui::Begin("SceneEdit", p_open)) {
		// ���݂̃|�X�g�v���Z�X���R���{�{�b�N�X�ŕ\��
		static const char* current_item = g_posteffectname[m_index].c_str();	// �������i�[

		ImGuiStyle& style = ImGui::GetStyle();
		float w = ImGui::CalcItemWidth();
		float spacing = style.ItemInnerSpacing.x;
		float button_sz = ImGui::GetFrameHeight();
		ImGui::PushItemWidth(w - spacing * 2.0f - button_sz * 2.0f);
		if (ImGui::BeginCombo("##combo", current_item)) {
			for (int n = 0; n < IM_ARRAYSIZE(g_posteffectname); n++) {
				bool is_selected = (current_item == g_posteffectname[n]);		 // ���O����v���Ă��邩�m�F
				if (ImGui::Selectable(g_posteffectname[n].c_str(), is_selected)) // ���ڒǉ�
				{
					m_index = n;								// �R���{�{�b�N�X��n�ԖڂɓK��
					current_item = g_posteffectname[n].c_str();
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();
		ImGui::SameLine(0, style.ItemInnerSpacing.x);
		ImGui::Text(u8"���݂̃|�X�g�v���Z�X");
		current_item = g_posteffectname[m_index].c_str();		// m_index�ɓK��

		// ���L�ݒ�̈ꗗ
		if (ImGui::BeginTabBar("#tabs", ImGuiTabBarFlags_None)) {
			ImGui_ShaderSetting();
			ImGui_CommonObject();
			ImGui::EndTabBar();
		}
	}
	ImGui::End();
}

/// <summary>
/// �������
/// </summary>
/// <param name="p_open">�E�B���h�E��\�����邩</param>
void SceneEdit::ImGui_UI_UserGuide(bool* p_open)
{
	ImGui::Begin(u8"������@", p_open);

	ImGui::Separator();	// ��������

	if (ImGui::BeginTabBar("#tabs", ImGuiTabBarFlags_None))
		ImGui_UserGuide();

	ImGui::End();
}

/// <summary>
/// �I�u�W�F�N�g�̐���
/// </summary>
/// <param name="p_open">�E�B���h�E��\�����邩</param>
void SceneEdit::ImGui_UI_AddGameObejcts(bool* p_open)
{
	// �I�u�W�F�N�g�����֘A
	if (ImGui::Begin(u8"Create", p_open)) {
		ImGui::Separator();	// ��������

		if (ImGui::BeginTabBar("#tabs", ImGuiTabBarFlags_None)) {
			ImGui_ObjectAddr();	// �I�u�W�F�N�g����

			ImGui::EndTabBar();
		}
		ImGui::Separator();
	}
	ImGui::End();
}

/// <summary>
/// ���������I�u�W�F�N�g�̈ꗗ
/// </summary>
/// <param name="p_open">�E�B���h�E��\�����邩</param>
void SceneEdit::ImGui_UI_GameObjectList(bool* p_open)
{
	// ���������I�u�W�F�N�g�̈ꗗ
	if (ImGui::Begin(u8"ObjectList", p_open))
		ImGui_ObjLists();	// ���X�g�`��

	ImGui::End();
}

/// <summary>
/// ���������I�u�W�F�N�g��Imstance
/// </summary>
/// <param name="p_open">�E�B���h�E��\�����邩</param>
void SceneEdit::ImGui_UI_Instance(bool* p_open)
{
	// ���������I�u�W�F�N�g��Instance
	if (ImGui::Begin(u8"Instance", p_open))
		ImGui_ObjectSetting();

	ImGui::End();
}

/// <summary>
/// �X�N���[���\��
/// </summary>
/// <param name="p_open">�E�B���h�E��\�����邩</param>
void SceneEdit::ImGui_UI_Screen(bool* p_open)
{
	if (ImGui::Begin(u8"�u���[", p_open))
		ImGui_RTVImageRender("BlurV");
	// ImGuiUI�I��
	ImGui::End();

	if (ImGui::Begin(u8"�A���x�h�i�F�j�o��", p_open))
		ImGui_RTVImageRender("albedoRTV");
	// ImGuiUI�I��
	ImGui::End();

	if (ImGui::Begin(u8"�[�x�l", p_open))
		ImGui_RTVImageRender("DepthRTV");
	// ImGuiUI�I��
	ImGui::End();

	if (ImGui::Begin(u8"�@��", p_open))
		ImGui_RTVImageRender("NomalRTV");
	// ImGuiUI�I��
	ImGui::End();

	if (ImGui::Begin(u8"Gbuffer", p_open)) {
		if (m_GBuffer) {
			switch (m_ShadowQualityLevel) {
			default:
			case HIGH:		ImGui_DSVImageRender("HighDepthWriteDSV");	break;
			case MEDIUM:	ImGui_DSVImageRender("DepthWriteDSV");		break;
			case LOW:		ImGui_DSVImageRender("LowDepthWriteDSV");	break;
			}
		}
	}

	if (ImGui::Begin(u8"Edit���", p_open))
		ImGui_RTVImageRender("LastRTV");
	// ImGuiUI�I��
	ImGui::End();

	// ImGuiUI�I��
	ImGui::End();
}

/// <summary>
/// RTV�摜��\������ImGui�֐�
/// </summary>
/// <param name="p_rtvname">�\����������������RTV�̖��O</param>
void SceneEdit::ImGui_RTVImageRender(std::string p_rtvname)
{
	// �����_�[�^�[�Q�b�g�̃e�N�X�`���擾
	RenderTarget* pImGuiRTV = GetObj<RenderTarget>(p_rtvname.c_str());

	// �e�N�X�`���\��
	ImGui::Image(pImGuiRTV->GetResource(),
		ImVec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2)
	);
}

/// <summary>
/// DSV�摜��\������ImGui�֐�
/// </summary>
/// <param name="p_dsvname">�\����������������DSV�̖��O</param>
void SceneEdit::ImGui_DSVImageRender(std::string p_dsvname)
{
	// �[�x�e�N�X�`���擾
	DepthStencil* pImGuiDSV = GetObj<DepthStencil>(p_dsvname.c_str());

	// �e�N�X�`���\��
	ImGui::Image(pImGuiDSV->GetResource(),
		ImVec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2)
	);
}

/// <summary>
/// ���ʐݒ��Shader�ݒ�
/// </summary>
void SceneEdit::ImGui_ShaderSetting()
{
	if (ImGui::BeginTabItem(u8"���ʐݒ�"))		// Tab
	{
		if (ImGui::TreeNode(u8"�t���X�^���J�����O")) {
			ImGui::Checkbox(u8"�L��", &m_FrustumCulling);
			ImGui::TreePop();
		}
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem(u8"�V�F�[�_�[�ݒ�"))		// Tab
	{
		if (ImGui::TreeNode(u8"�V���h�E")) {
			// G�o�b�t�@�\�����邩�ǂ���
			ImGui::Checkbox(u8"�e�pG�o�b�t�@�\��", &m_GBuffer);

			// G�o�b�t�@���\������Ă�����e�̕\����\���`�F�b�N�{�b�N�X��\��
			if (m_GBuffer) {
				ImGui::Checkbox(u8"�e�̕\��", &m_IsShadow);
			}

			// �e�̕i���ύX�R���{�{�b�N�X
			const char* shadowquality[] = { "High", "Medium", "Low" };
			static const char* current_item = shadowquality[m_ShadowQualityLevel];
			ImGuiComboFlags flags = ImGuiComboFlags_NoArrowButton;

			ImGuiStyle& style = ImGui::GetStyle();
			float w = ImGui::CalcItemWidth();
			float spacing = style.ItemInnerSpacing.x;
			float button_sz = ImGui::GetFrameHeight();
			ImGui::PushItemWidth(w - spacing * 2.0f - button_sz * 2.0f);
			if (ImGui::BeginCombo("##custom combo", current_item, ImGuiComboFlags_NoArrowButton)) {
				for (int n = 0; n < IM_ARRAYSIZE(shadowquality); n++) {
					bool is_selected = (current_item == shadowquality[n]);
					if (ImGui::Selectable(shadowquality[n], is_selected))
						current_item = shadowquality[n];
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();
			ImGui::SameLine(0, style.ItemInnerSpacing.x);
			ImGui::Text(u8"�e�̕i��");

			// �e�̕i���؂�ւ�
			if (current_item == "High")
				m_ShadowQualityLevel = ShadowQuality::HIGH;
			if (current_item == "Medium")
				m_ShadowQualityLevel = ShadowQuality::MEDIUM;
			if (current_item == "Low")
				m_ShadowQualityLevel = ShadowQuality::LOW;

			ImGui::TreePop();
		}

		if (ImGui::TreeNode(u8"�֊s��"))
		{
			ImGui::Checkbox(u8"�֊s���\��", &m_Outline);
			if (m_Outline) {
				ImGui::InputFloat(u8"�֊s���̑���", &m_OutLineVal, 0.001f);

				ImGui::ColorEdit3(u8"OutLineColor", (float*)&m_OutLineColor);	// �֊s���̐F
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNode(u8"�A���t�@�f�B�U"))
		{
			ImGui::InputFloat(u8"�A���t�@�f�B�U�K������", &m_AlphaLen, 0.05f);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode(u8"�K���}�␳"))
		{
			ImGui::Checkbox(u8"��ɃK���}�␳", &m_AlwaysGamma);

			ImGui::InputFloat(u8"�K���}�l", &m_GammaValue, 0.01f);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode(u8"��ʊE�[�x"))
		{
			ImGui::Checkbox(u8"��ʊE�[�x�K��", &m_DoF);
			if (m_DoF) {
				m_blur = true;	// �u���[���ʃI��
				ImGui::InputFloat(u8"�u���[�̋���", &m_blurStrength, 0.01f);
				ImGui::InputFloat(u8"��O�K���l", &m_ViewClipping[0], 0.01f);
				ImGui::InputFloat(u8"���K���l", &m_ViewClipping[1], 0.01f);
			}
			else {
				m_blur = false;	// �u���[���ʃI�t
			}
			ImGui::TreePop();
		}
		ImGui::EndTabItem();
	}
}

/// <summary>
/// ���ʐݒ�̋��ʃI�u�W�F�N�g�̐ݒ�
/// </summary>
void SceneEdit::ImGui_CommonObject()
{
	if (ImGui::BeginTabItem(u8"���ʃI�u�W�F�N�g�ݒ�"))		// Tab
	{
		if (ImGui::TreeNode(u8"Camera")) {
			Camera* pCamera = GetObj<Camera>("Camera");

			static float cameramoveval = pCamera->GetMoveVal();	// �J�����ړ��{��

			ImGui::InputFloat(u8"�ړ��{��", &cameramoveval, 1.0f);
			ImGui::SameLine();	// ���ɕ\��
			ImGui::Text(u8"+Shift");

			pCamera->SetMoveVal(cameramoveval);		// �J�����̈ړ��{�����Z�b�g

			ImGui::TreePop();
		}

		if (ImGui::TreeNode(u8"Light")) {
			MoveLight* pLight = GetObj<MoveLight>("Light");

			if (ImGui::ColorEdit3(u8"Light color", (float*)&m_LightColor)) {	// �F�ύX
				// ���C�g�̐F���Z�b�g
				pLight->GetComponent<Light>()->SetAmbient(
					DirectX::SimpleMath::Color(m_LightColor.x / 5, m_LightColor.y / 5, m_LightColor.z / 5, m_LightColor.w));	// ALL 1.0f = (0.2f,0.2f,0.2f,1.0f)�
				pLight->GetComponent<Light>()->SetDiffuse(
					DirectX::SimpleMath::Color(m_LightColor.x, m_LightColor.y, m_LightColor.z, m_LightColor.w));
			}
			else {
				// ���C�g�̐F���擾
				m_LightColor.x = pLight->GetComponent<Light>()->GetAmbient().x * 5;
				m_LightColor.y = pLight->GetComponent<Light>()->GetAmbient().y * 5;
				m_LightColor.z = pLight->GetComponent<Light>()->GetAmbient().z * 5;
				m_LightColor.w = pLight->GetComponent<Light>()->GetAmbient().w * 5;
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}

/// <summary>
/// �I�u�W�F�N�g�̐���
/// </summary>
void SceneEdit::ImGui_ObjectAddr()
{
	//-----------------------------------------------------------------------------
	if (ImGui::BeginTabItem(u8"�����n"))
	{
		// �X�N���[�����X�g
		ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(0, 0), ImGuiChildFlags_Border);

		// assimp�̈ꗗ���琶��
		ImGui_ObjModelLoad(g_objfilename, g_objfilenum, false);

		// �����_�[�^�[�Q�b�g�̃e�N�X�`���擾
		Texture* pImGuiTex = GetObj<Texture>("IconTex");
		// �X�N���[�����X�g
		ImGui::BeginChild(ImGui::GetID((void*)10), ImVec2(120, 120), ImGuiChildFlags_AlwaysUseWindowPadding, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		if (ImGui::ImageButton(u8"�{�b�N�X����", pImGuiTex->GetResource(), ImVec2(64, 64))) {
			m_Count++;
			// (���O���h�~)
			std::stringstream ModelName;
			ModelName << "Model" << m_Count;
			Box* pBox = CreateObj<Box>(ModelName.str().c_str());
			pBox->SetID(ModelName.str().c_str());
			pBox->SetName("BoxObject");
			pBox->Init();
			pBox->GetComponent<ModelSetting>()->SetCharacterFlag(false);
			pBox->GetComponent<ShaderComponent>()->SetVSShader(GetObj<Shader>("VS_Object"));		// Vs�V�F�[�_�[�Z�b�g
			pBox->GetComponent<ShaderComponent>()->SetOldVSShader(GetObj<Shader>("VS_Object"));		// �ۑ�Vs�V�F�[�_�[�Z�b�g
			pBox->GetComponent<ShaderComponent>()->SetPSShader(GetObj<Shader>("PS_TexColor"));		// Ps�V�F�[�_�[�Z�b�g
			pBox->GetComponent<ShaderComponent>()->SetOldPSShader(GetObj<Shader>("PS_TexColor"));	// �ۑ�Ps�V�F�[�_�[�Z�b�g
			// �{�b�N�X�I�u�W�F�N�g�ǉ�
			ObjeList.push_back(pBox);
		}
		ImGui::Text(u8"Box");
		ImGui::EndChild();
		ImGui::SameLine();	// ���ɕ\��
		ImGui::BeginChild(ImGui::GetID((void*)11), ImVec2(120, 120), ImGuiChildFlags_AlwaysUseWindowPadding, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		if (ImGui::ImageButton(u8"�V�����_�[����", pImGuiTex->GetResource(), ImVec2(64, 64))) {
			m_Count++;
			// (���O���h�~)
			std::stringstream ModelName;
			ModelName << "Model" << m_Count;
			Cylinder* pCylinder = CreateObj<Cylinder>(ModelName.str().c_str());
			pCylinder->SetID(ModelName.str().c_str());
			pCylinder->SetName("CylinderObject");
			pCylinder->Init();
			pCylinder->GetComponent<ModelSetting>()->SetCharacterFlag(false);
			pCylinder->GetComponent<ShaderComponent>()->SetVSShader(GetObj<Shader>("VS_Object"));	// Vs�V�F�[�_�[�Z�b�g
			pCylinder->GetComponent<ShaderComponent>()->SetOldVSShader(GetObj<Shader>("VS_Object"));	// �ۑ�Vs�V�F�[�_�[�Z�b�g
			pCylinder->GetComponent<ShaderComponent>()->SetPSShader(GetObj<Shader>("PS_TexColor"));	// Ps�V�F�[�_�[�Z�b�g
			pCylinder->GetComponent<ShaderComponent>()->SetOldPSShader(GetObj<Shader>("PS_TexColor"));	// �ۑ�Ps�V�F�[�_�[�Z�b�g
			// �V�����_�[�I�u�W�F�N�g�ǉ�
			ObjeList.push_back(pCylinder);
		}
		ImGui::Text(u8"�V�����_�[");
		ImGui::EndChild();
		ImGui::SameLine();	// ���ɕ\��
		ImGui::BeginChild(ImGui::GetID((void*)12), ImVec2(120, 120), ImGuiChildFlags_AlwaysUseWindowPadding, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		if (ImGui::ImageButton(u8"�t�B�[���h����", pImGuiTex->GetResource(), ImVec2(64, 64))) {
			m_Count++;
			// (���O���h�~)
			std::stringstream ModelName;
			ModelName << "Model" << m_Count;
			Field* pField = CreateObj<Field>(ModelName.str().c_str());
			pField->SetID(ModelName.str().c_str());
			pField->SetName("FieldObject");
			pField->Init();
			pField->GetComponent<ModelSetting>()->SetCharacterFlag(false);
			pField->GetComponent<ShaderComponent>()->SetVSShader(GetObj<Shader>("VS_Object"));	// Vs�V�F�[�_�[�Z�b�g
			pField->GetComponent<ShaderComponent>()->SetOldVSShader(GetObj<Shader>("VS_Object"));	// �ۑ�Vs�V�F�[�_�[�Z�b�g
			pField->GetComponent<ShaderComponent>()->SetPSShader(GetObj<Shader>("PS_TexColor"));	// Ps�V�F�[�_�[�Z�b�g
			pField->GetComponent<ShaderComponent>()->SetOldPSShader(GetObj<Shader>("PS_TexColor"));	// �ۑ�Ps�V�F�[�_�[�Z�b�g
			// �t�B�[���h�I�u�W�F�N�g�ǉ�
			ObjeList.push_back(pField);
		}
		ImGui::Text(u8"�t�B�[���h");
		ImGui::EndChild();
		ImGui::EndChild();
		ImGui::EndTabItem();
	}
	//-----------------------------------------------------------------------------
	if (ImGui::BeginTabItem(u8"�L�����N�^�[�n"))
	{
		// �X�N���[�����X�g
		ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(0, 0), ImGuiChildFlags_Border);

		// assimp�̈ꗗ���琶��
		ImGui_ObjModelLoad(g_modelfilename, g_modelfilenum, true);

		ImGui::EndChild();
		ImGui::EndTabItem();
	}
}

/// <summary>
/// Assimp�Ń��f���ǂݍ��݈ꗗ
/// </summary>
void SceneEdit::ImGui_ObjModelLoad(std::string _filename[], int _filenum, bool _isshadow)
{
	// �����_�[�^�[�Q�b�g�̃e�N�X�`���擾
	Texture* pImGuiTex = GetObj<Texture>("IconTex");
	bool isfirstnumber = true;		// ���������p�ԍ�
	for (int i = 0; i < _filenum; i++)
	{
		// �X�N���[�����X�g
		ImGui::BeginChild(ImGui::GetID((void*)i), ImVec2(120, 120), ImGuiChildFlags_AlwaysUseWindowPadding, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		if (ImGui::ImageButton(_filename[i].c_str(), pImGuiTex->GetResource(), ImVec2(64, 64))) {
			m_Count++;
			// (���O���h�~)
			std::stringstream ModelName;
			ModelName << "Model" << m_Count;

			// u8�G���R�[�h��path���R���X�g���N�g
			std::filesystem::path p = std::filesystem::u8path(_filename[i]);
			// Model�ǂݍ���
			Model* pCharObject = CreateObj<Model>(ModelName.str().c_str());
			pCharObject->SetID(_filename[i]);																		// ID�Z�b�g
			pCharObject->SetName(p.stem().u8string());																// ���O�Z�b�g
			pCharObject->Init();
			pCharObject->GetComponent<ModelSetting>()->SetCharacterFlag(_isshadow);									// �e�L�����t���O�Z�b�g
			pCharObject->GetComponent<ShaderComponent>()->SetVSShader(GetObj<Shader>("VS_Object"));					// Vs�V�F�[�_�[�Z�b�g
			pCharObject->GetComponent<ShaderComponent>()->SetOldVSShader(GetObj<Shader>("VS_Object"));				// �ۑ�Vs�V�F�[�_�[�Z�b�g
			pCharObject->GetComponent<ShaderComponent>()->SetPSShader(GetObj<Shader>("PS_Material"));				// Ps�V�F�[�_�[�Z�b�g
			pCharObject->GetComponent<ShaderComponent>()->SetOldPSShader(GetObj<Shader>("PS_Material"));			// �ۑ�Ps�V�F�[�_�[�Z�b�g
			pCharObject->GetComponent<ModelMeshLoad>()->Load(_filename[i].c_str());									// ModelLoad
			pCharObject->GetComponent<ModelMeshRenderer>()->Init(*pCharObject->GetComponent<ModelMeshLoad>());		// ModelRenderer
			pCharObject->SetScale(DirectX::SimpleMath::Vector3(0.05f, 0.05f, 0.05f));								// �T�C�Y
			// �v���C���[�I�u�W�F�N�g�ǉ�
			ObjeList.push_back(pCharObject);
		}
		// u8�G���R�[�h��path���R���X�g���N�g
		std::filesystem::path ModelName = std::filesystem::u8path(_filename[i]);
		ImGui::Text(ModelName.stem().u8string().c_str());
		ImGui::EndChild();

		if (isfirstnumber)			// �����0�ŏ������ɓ����Ă��܂����߉������
		{
			if (i == 0)
			{
				ImGui::SameLine();	// ���ɕ\��
				isfirstnumber = false;
			}
		}
		else if ((i + 1) % 7 != 0)	// 7�̔{������Ȃ���Ή��ɕ\��(�����0�l���ς�)
			ImGui::SameLine();	// ���ɕ\��
	}
}

/// <summary>
/// �I�u�W�F�N�g���X�g�\������
/// </summary>
void SceneEdit::ImGui_ObjLists()
{
	// ���������I�u�W�F�N�g��Loop����
	for (int i = 0; i < ObjeList.size(); i++)
	{
		if (i != 0) {
			// ���O�\��(���O���h�~)
			std::stringstream objname;
			objname << i << ": " << ObjeList[i]->GetName().c_str();
			if (ImGui::Button(objname.str().c_str()))
			{
				m_Number = i;	// instance�̎Q�ƃI�u�W�F�N�g���Z�b�g����
			}

			// ���N���b�N����(���O���h�~)
			std::stringstream clickright;
			clickright << i << ": isSet";
			if (ImGui::BeginPopup(clickright.str().c_str())) {
				if (ImGui::Button(u8"�폜")) {
					m_Number = 0;	// instance�̎Q�ƃI�u�W�F�N�g�����Z�b�g����

					// i�Ԗڂ̃I�u�W�F�N�g�̃R���|�[�l���g�Ȃǂ�����
					ObjeList[i]->Uninit();

					// �I�u�W�F�N�g��ID������
					DestroyObj(ObjeList[i]->GetID().c_str());

					ObjeList.erase(ObjeList.begin() + i);
					i--;
				}

				ImGui::EndPopup();
			}
			// ���N���b�N���菈��
			ImGui::OpenPopupOnItemClick(clickright.str().c_str(), ImGuiPopupFlags_MouseButtonRight);
		}
	}
}

/// <summary>
/// �I�������I�u�W�F�N�g�̏ڍאݒ�
/// </summary>
void SceneEdit::ImGui_ObjectSetting()
{
	// null�`�F�b�N
	if (m_Number == 0) 		return;

	// �I�u�W�F�N�g�̖��O�ϐ�
	static char objectname[256]{};
	strcpy(objectname, ObjeList[m_Number]->GetName().c_str());	// �I�u�W�F�N�g�̖��O���擾����
	ImGui::InputText("ObjectName", objectname, 256);			// ���O���w�肷��

	ObjeList[m_Number]->SetName(objectname);					// ��ɖ��O���X�V����

	ImGui_ObjectSetting_Shader();			// ���f���̃V�F�[�_�[�ύX
	ImGui_ObjectSetting_ShaderSetting();	// ���f���̌ʃV�F�[�_�[�̏ڍאݒ�
	ImGui_ObjectSetting_Transfrom();		// ���f���̈ʒu�E�p�x�E�T�C�Y�ݒ�
	ImGui_ObjectSetting_Material();			// ���f���̃}�e���A���ݒ�
	//ImGui_ObjectComponentSetting();
}

/// <summary>
/// ���f���̃V�F�[�_�[�ύX
/// </summary>
void SceneEdit::ImGui_ObjectSetting_Shader()
{
	if (ImGui::BeginTabBar("#tabs", ImGuiTabBarFlags_None))
	{
		if (ImGui::BeginTabItem(u8"�f�t�H���g")) {
			// �X�N���[�����X�g
			ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(0, 100), ImGuiWindowFlags_NoTitleBar);
			if (ImGui::Button(u8"�f�t�H���g�F")) {
				// assimp�œǂݍ��񂾃I�u�W�F�N�g�����f����
				if (ObjeList[m_Number]->GetComponent<ModelMeshRenderer>() != nullptr) {
					ObjeList[m_Number]->GetComponent<ShaderComponent>()->SetOldPSShader(GetObj<Shader>("PS_Material"));
				}
				else {
					ObjeList[m_Number]->GetComponent<ShaderComponent>()->SetOldPSShader(GetObj<Shader>("PS_TexColor"));
				}

			}
			ImGui::EndChild();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem(u8"�����ڌn")) {
			// �X�N���[�����X�g
			ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(0, 100), ImGuiWindowFlags_NoTitleBar);

			if (ImGui::Button(u8"�g�D�[��"))
				ObjeList[m_Number]->GetComponent<ShaderComponent>()->SetOldPSShader(GetObj<Shader>("PS_ModelToon"));

			if (ImGui::Button(u8"���m�N��"))
				ObjeList[m_Number]->GetComponent<ShaderComponent>()->SetOldPSShader(GetObj<Shader>("PS_Monochrome"));

			if (ImGui::Button(u8"�Z�s�A"))
				ObjeList[m_Number]->GetComponent<ShaderComponent>()->SetOldPSShader(GetObj<Shader>("PS_Sepia"));

			ImGui::EndChild();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem(u8"�f�B�U�n")) {
			// �X�N���[�����X�g
			ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(0, 100), ImGuiWindowFlags_NoTitleBar);
			if (ImGui::Button(u8"�A���t�@�f�B�U"))
				ObjeList[m_Number]->GetComponent<ShaderComponent>()->SetOldPSShader(GetObj<Shader>("PS_AlphaDither"));

			ImGui::EndChild();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem(u8"�m�C�Y�n")) {
			// �X�N���[�����X�g
			ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(0, 100), ImGuiWindowFlags_NoTitleBar);
			if (ImGui::Button(u8"�|�X�^���[�[�V����"))
				ObjeList[m_Number]->GetComponent<ShaderComponent>()->SetOldPSShader(GetObj<Shader>("PS_Posterization"));
			if (ImGui::Button(u8"�F����"))
				ObjeList[m_Number]->GetComponent<ShaderComponent>()->SetOldPSShader(GetObj<Shader>("PS_RGBSplit"));
			ImGui::EndChild();
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
}

/// <summary>
/// ���f���̌ʃV�F�[�_�[�̏ڍאݒ�
/// </summary>
void SceneEdit::ImGui_ObjectSetting_ShaderSetting()
{
	// ���f���Z�b�e�B���O�R���|�[�l���g�����݂��Ă����珈������
	if (ObjeList[m_Number]->GetComponent<ModelSetting>() != nullptr)
	{
		// ���f���X�̐ݒ�󋵂��Ƃ��Ă���
		m_ModelListOriginalSetting = ObjeList[m_Number]->GetComponent<ModelSetting>()->GetOriginalSetting();

		if (ImGui::BeginTabBar("#tabs1", ImGuiTabBarFlags_None)) {
			if (ImGui::BeginTabItem(u8"�ʐݒ�")) {
				// �X�N���[�����X�g
				ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(0, 150), ImGuiWindowFlags_NoTitleBar);

				// ���f���X�̐ݒ��K�����邩�ǂ���
				ImGui::Checkbox(u8"�ʂɃV�F�[�_�[�ݒ�", &m_ModelListOriginalSetting);
				ObjeList[m_Number]->GetComponent<ModelSetting>()->SetOriginalSetting(m_ModelListOriginalSetting);	// �Z�b�g

				ImGui::EndChild();
				ImGui::EndTabItem();
			}

			// �I���W�i���ݒ肷��ꍇ
			if (m_ModelListOriginalSetting) {
				if (ImGui::BeginTabItem(u8"�V�F�[�_�[�ݒ�")) {
					// �X�N���[�����X�g
					ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(0, 150), ImGuiWindowFlags_NoTitleBar);

					if (ImGui::TreeNode(u8"�֊s���̕`��")) {
						// �A�E�g���C����`�悷�邩�ǂ����擾
						m_ModelOutline = ObjeList[m_Number]->GetComponent<ModelSetting>()->GetOutLineDraw();
						ImGui::Checkbox(u8"�֊s���̕`������邩�ǂ���", &m_ModelOutline);
						ObjeList[m_Number]->GetComponent<ModelSetting>()->SetOutLineDraw(m_ModelOutline);		// �Z�b�g

						// �֊s����`�悷��Ȃ�֊s���̐ݒ荀�ڂ�\������
						if (m_ModelOutline = ObjeList[m_Number]->GetComponent<ModelSetting>()->GetOutLineDraw()) {
							// �A�E�g���C���̑������擾
							m_ModelOutLineVal = ObjeList[m_Number]->GetComponent<ModelSetting>()->GetOutLineVal();
							ImGui::InputFloat(u8"�֊s���̑���", &m_ModelOutLineVal, 0.001f);
							ObjeList[m_Number]->GetComponent<ModelSetting>()->SetOutLineVal(m_ModelOutLineVal);		// �Z�b�g

							// �A�E�g���C���̐F���擾
							m_ModelOutLineColor = ObjeList[m_Number]->GetComponent<ModelSetting>()->GetOutLineColor();
							ImGui::ColorEdit3(u8"OutLineColor", (float*)&m_ModelOutLineColor); // Edit 3 floats representing a color
							ObjeList[m_Number]->GetComponent<ModelSetting>()->SetOutLineColor(m_ModelOutLineColor);		// �Z�b�g
						}
						ImGui::TreePop();
					}
					ImGui::EndChild();
					ImGui::EndTabItem();
				}
			}
			ImGui::EndTabBar();
		}
	}
}

/// <summary>
/// ���f���̈ʒu�E�p�x�E�T�C�Y�ݒ�
/// </summary>
void SceneEdit::ImGui_ObjectSetting_Transfrom()
{
	//-----------------------------------------------------------------------------
	// ���[�J���錾
	//-----------------------------------------------------------------------------
	// ���������ʒu�ۊǕϐ��ɃI�u�W�F�N�g�̈ʒu������
	DirectX::SimpleMath::Vector3 pos = (ObjeList[m_Number]->GetPosition());
	// ���������p�x�ۊǕϐ��ɃI�u�W�F�N�g�̊p�x������
	DirectX::SimpleMath::Vector3 rot = (ObjeList[m_Number]->GetRotation());
	// ���������T�C�Y�ۊǕϐ��ɃI�u�W�F�N�g�̃T�C�Y������
	DirectX::SimpleMath::Vector3 size = (ObjeList[m_Number]->GetScale());

	//-----------------------------------------------------------------------------
	//	�ʒu
	//-----------------------------------------------------------------------------
	// ������́Z�Ԗڂ��q����
	if (ImGui::TreeNode(u8"Position"))
	{
		// ������́Z�Ԗڂ��q����
		std::stringstream stream;
		stream << "X";
		ImGui::InputFloat(stream.str().c_str(), &pos.x, 0.5f);		// X���W
		stream.str("");		// �����񏉊���
		stream << "Y";
		ImGui::InputFloat(stream.str().c_str(), &pos.y, 0.5f);		// Y���W
		stream.str("");
		stream << "Z";
		ImGui::InputFloat(stream.str().c_str(), &pos.z, 0.5f);		// Z���W
		stream.str("");
		ObjeList[m_Number]->SetPosition(pos);						// �ݒ肵���ʒu���Z�b�g����
		ImGui::TreePop();
	}
	//-----------------------------------------------------------------------------
	//	�p�x
	//-----------------------------------------------------------------------------
	if (ImGui::TreeNode(u8"Rotation"))
	{
		// ������́Z�Ԗڂ��q����
		std::stringstream stream;
		stream << "X";
		ImGui::InputFloat(stream.str().c_str(), &rot.x, 0.5f);		// X���W
		stream.str("");		// �����񏉊���
		stream << "Y";
		ImGui::InputFloat(stream.str().c_str(), &rot.y, 0.5f);		// Y���W
		stream.str("");
		stream << "Z";
		ImGui::InputFloat(stream.str().c_str(), &rot.z, 0.5f);		// Z���W
		stream.str("");
		ObjeList[m_Number]->SetRotation(rot);						// �ݒ肵���p�x���Z�b�g����
		ImGui::TreePop();
	}
	//-----------------------------------------------------------------------------
	//	�T�C�Y
	//-----------------------------------------------------------------------------
	if (ImGui::TreeNode(u8"Size"))
	{
		// ������́Z�Ԗڂ��q����
		std::stringstream stream;
		stream << "X";
		ImGui::InputFloat(stream.str().c_str(), &size.x, 0.05f);		// X���W
		stream.str("");		// �����񏉊���
		stream << "Y";
		ImGui::InputFloat(stream.str().c_str(), &size.y, 0.05f);		// Y���W
		stream.str("");
		stream << "Z";
		ImGui::InputFloat(stream.str().c_str(), &size.z, 0.05f);		// Z���W
		stream.str("");
		ObjeList[m_Number]->SetScale(size);							// �ݒ肵���ʒu���Z�b�g����
		ImGui::TreePop();
	}
}

/// <summary>
/// ���f���̃}�e���A���ݒ�
/// </summary>
void SceneEdit::ImGui_ObjectSetting_Material()
{
	// ModelMeshRenderer���Ȃ���Έȍ~�̏��������Ȃ�
	if (ObjeList[m_Number]->GetComponent<ModelMeshRenderer>() == nullptr)	return;

	std::vector<ID3D11ShaderResourceView*>	m_DiffuseSRVtable
		= ObjeList[m_Number]->GetComponent<ModelMeshRenderer>()->GetDiffuseRTV();	// �f�B�t���[�Y�e�N�X�`��SRV
	std::vector<SUBSET>	m_Subsets
		= ObjeList[m_Number]->GetComponent<ModelMeshRenderer>()->GetSubSet();		// �T�u�Z�b�g���
	std::vector<CMaterial*> m_Materials
		= ObjeList[m_Number]->GetComponent<ModelMeshRenderer>()->GetMaterial();		// �}�e���A���Q

	ImVec4 m_DiffuseColor;
	ImVec4 m_AmbientColor;

	int count = 0;				// �摜3���ɕ��ׂ�悤count

	if (ImGui::TreeNode(u8"ModelMaterial"))
	{
		if (ImGui::TreeNode(u8"Texture")) {
			for (auto& subset : m_Subsets) {
				// �e�N�X�`���\��
				if (count == 0) {
					// �e�N�X�`���\��
					ImGui::Image(m_DiffuseSRVtable[subset.MaterialIdx],
						ImVec2(100, 100)
					);
					count++;			// count��i�߂�
				}
				else if (count < 3) {
					count++;			// count��i�߂�

					ImGui::SameLine();	// ���ɕ\��
					// �e�N�X�`���\��
					ImGui::Image(m_DiffuseSRVtable[subset.MaterialIdx],
						ImVec2(100, 100)
					);
				}
				else {
					count = 1;			// �J�E���g���Z�b�g
					// �e�N�X�`���\��
					ImGui::Image(m_DiffuseSRVtable[subset.MaterialIdx],
						ImVec2(100, 100)
					);
				}
			}
			ImGui::TreePop();
		}
		// �}�e���A�����ꗗ�\��
		if (ImGui::TreeNode(u8"Diffuse")) {
			// �T�u�Z�b�g�̐������[�v�i�T�u�Z�b�g���̓}�e���A���̐��j
			for (auto& subset : m_Subsets) {
				m_DiffuseColor.x = m_Materials[subset.MaterialIdx]->GetDiffuse().x;
				m_DiffuseColor.y = m_Materials[subset.MaterialIdx]->GetDiffuse().y;
				m_DiffuseColor.z = m_Materials[subset.MaterialIdx]->GetDiffuse().z;
				m_DiffuseColor.w = m_Materials[subset.MaterialIdx]->GetDiffuse().w;

				// ���O�\��(���O���h�~)
				std::stringstream materialval;
				materialval << subset.MaterialIdx << "Color";
				if (ImGui::ColorEdit4(materialval.str().c_str(), (float*)&m_DiffuseColor))
				{
					ObjeList[m_Number]->GetComponent<ModelMeshRenderer>()->GetMaterial()[subset.MaterialIdx]->SetDiffuse(
						DirectX::SimpleMath::Color(m_DiffuseColor.x, m_DiffuseColor.y, m_DiffuseColor.z, m_DiffuseColor.w));
				}
			}
			ImGui::TreePop();
		}
		// �}�e���A�����ꗗ�\��
		if (ImGui::TreeNode(u8"Ambient")) {
			// �T�u�Z�b�g�̐������[�v�i�T�u�Z�b�g���̓}�e���A���̐��j
			for (auto& subset : m_Subsets) {
				m_AmbientColor.x = m_Materials[subset.MaterialIdx]->GetAmbient().x;
				m_AmbientColor.y = m_Materials[subset.MaterialIdx]->GetAmbient().y;
				m_AmbientColor.z = m_Materials[subset.MaterialIdx]->GetAmbient().z;
				m_AmbientColor.w = m_Materials[subset.MaterialIdx]->GetAmbient().w;

				// ���O�\��(���O���h�~)
				std::stringstream materialval;
				materialval << subset.MaterialIdx << "Color";
				if (ImGui::ColorEdit4(materialval.str().c_str(), (float*)&m_AmbientColor))
				{
					ObjeList[m_Number]->GetComponent<ModelMeshRenderer>()->GetMaterial()[subset.MaterialIdx]->SetAmbient(
						DirectX::SimpleMath::Color(m_AmbientColor.x, m_AmbientColor.y, m_AmbientColor.z, m_AmbientColor.w));
				}
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}

/// <summary>
/// �I�u�W�F�N�g�̃R���|�[�l���g�ݒ�
/// </summary>
void SceneEdit::ImGui_ObjectComponentSetting()
{
	if (ImGui::TreeNode(u8"Component"))
	{

		ImGui::TreePop();
	}

	static bool isactive = false;
	if (ImGui::Button(u8"AddComponent")) {
		isactive = !isactive;
	}

	if (ImGui::Begin(u8"ComponentList", &isactive))
	{
		// �X�N���[�����X�g
		ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(300, 0), ImGuiWindowFlags_NoTitleBar);
		if (ImGui::Button(u8"un")) {

		}
		ImGui::EndChild();
	}
	ImGui::End();
}

/// <summary>
/// ImGui�������
/// </summary>
void SceneEdit::ImGui_UserGuide()
{
	if (ImGui::BeginTabItem(u8"�J��������"))
	{
		// �������ʒu�̏ڍ׏��
		ImGui::Text(u8"ALT + SHIFT(�ʒu���Z�b�g)");
		// ���R�ړ��̏ڍ׏��
		if (ImGui::TreeNode(u8"���N���b�N(���R�ړ�)"))
		{
			ImGui::Text(u8"+Shift		: �J�����{�����x�K��");
			ImGui::Text(u8"WS			: �J�����O�i��i");
			ImGui::Text(u8"AD			: �J�����E�ړ����ړ�");
			ImGui::Text(u8"QE			: �J�����^��ړ��^���ړ�");
			ImGui::TreePop();
		}
		// ��]����̏ڍ׏��
		ImGui::Text(u8"ALT + �E�N���b�N(��]����)");
		// ���s�ړ�����̏ڍ׏��
		ImGui::Text(u8"ALT + ���N���b�N(���s�ړ�����)");
		// �Y�[������̏ڍ׏��
		ImGui::Text(u8"ALT + ���N���b�N(�Y�[������)");

		ImGui::EndTabItem();
	}

	if (ImGui::BeginTabItem(u8"light����"))
	{
		// light�ړ��̏ڍ׏��
		if (ImGui::TreeNode(u8"light�ړ�"))
		{
			ImGui::Text(u8"[L�L�[]�������Ȃ���A�ȉ��̃L�[�Ŋe������s��");
			ImGui::Text(u8"[AD�L�[] ���_�𒆐S��Y����]");
			ImGui::Text(u8"[WS�L�[] ���_�𒆐S�ɏc��]");
			ImGui::TreePop();
		}
		// light�ϐF�̏ڍ׏��
		if (ImGui::TreeNode(u8"light�ϐF"))
		{
			ImGui::Text(u8"[R�L�[]�������Ȃ���A�ȉ��̃L�[�Ŋe������s��");
			ImGui::Text(u8"[R�L�[] light�̐F�ɖ߂�");
			ImGui::Text(u8"[���E���L�[] �F�ʂ̕ύX(HSV��H��ύX)");
			ImGui::Text(u8"[�㉺���L�[] �ʓx�Ɩ��x��ύX(1.0(�ʓx0/���x1)�`0.5f(�ʓx1/���x1)�`0.0f(�ʓx1/���x0))");
			ImGui::TreePop();
		}
		ImGui::EndTabItem();
	}

	if (ImGui::BeginTabItem(u8"�I�u�W�F�N�g����"))
	{
		if (ImGui::TreeNode(u8"Instance����"))
		{
			// �������ʒu�̏ڍ׏��
			ImGui::Text(u8"�e��V�F�[�_�[���ڂŌ����ڂ�ύX�ł���");
			ImGui::Text(u8"�p�x�E�ʒu�E�T�C�Y��XYZ��ύX�ł���");
			ImGui::Text(u8"+�{�^���@���Z : -�{�^���@���Z");
			ImGui::TreePop();
		}
		if (ImGui::TreeNode(u8"ObjectList����"))
		{
			ImGui::Text(u8"�I�u�W�F�N�g�����N���b�N��Instance�ɏڍאݒ�\");
			ImGui::Text(u8"�I�u�W�F�N�g���E�N���b�N�ō폜�{�^���o��");
			ImGui::TreePop();
		}

		ImGui::EndTabItem();
	}
	ImGui::EndTabBar();
}
