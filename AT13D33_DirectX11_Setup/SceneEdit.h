//-------------------------------------------------------------------------------------
// Class : SceneEdit
// �eClass   :   Scene
// 
// �V�[��Edit�Ɋւ��鏈�����l�܂��Ă���
// 
// Yasuda.Aoto  2023.12.18
//-------------------------------------------------------------------------------------
#ifndef __SCENE_VISUAL_H__
#define __SCENE_VISUAL_H__

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Scene.hpp"
#include "main.h"		// �E�B���h�E�T�C�Y�擾�p
// ���O�֌W
#include "FileName.h"

class SceneEdit : public Scene
{
public:
	void Init() override;
	void Uninit() override;
	void Update(float tick) override;
	void Draw() override;
	void ImGui() override;
private:
	// Guide
	void GuideLine();

	// Render
	void LastRender();
	void DefaultRender(bool p_active);
	void OutlineRender(bool p_active);
	void GBufferRender(bool p_active, std::string p_rtv, std::string p_dsv);
	void AlwaysGammaRender(bool p_active);
	void ShadowRender(bool p_active, std::string p_rtv);
	void DepthOfFieldRender(bool p_active);
	void PostEffectRender(bool p_active);

	// Material
	void ModelMaterialSet(int p_number, bool p_set);

	// ImGui
	void ImGui_UI_SceneEdit(bool* p_open);
	void ImGui_UI_UserGuide(bool* p_open);
	void ImGui_UI_AddGameObejcts(bool* p_open);
	void ImGui_UI_GameObjectList(bool* p_open);
	void ImGui_UI_Instance(bool* p_open);
	void ImGui_UI_Screen(bool* p_open);

	// Image
	void ImGui_RTVImageRender(std::string p_rtvname);
	void ImGui_DSVImageRender(std::string p_dsvname);

	// Setting
	void ImGui_ShaderSetting();					// �V�F�[�_�[�̐ݒ�
	void ImGui_CommonObject();					// ���̃V�[���̋��ʐݒ�
	void ImGui_ObjectSetting();					// ���f���̕ҏW
	void ImGui_ObjectSetting_Shader();			// ���f���̃V�F�[�_�[�ύX
	void ImGui_ObjectSetting_ShaderSetting();	// ���f���̌ʃV�F�[�_�[�̏ڍאݒ�
	void ImGui_ObjectSetting_Transfrom();		// ���f���̈ʒu�E�p�x�E�T�C�Y�ݒ�
	void ImGui_ObjectSetting_Material();		// ���f���̃}�e���A���ݒ�
	void ImGui_ObjectComponentSetting();		// �I�u�W�F�N�g�̃R���|�[�l���g�ݒ�
	// Model
	void ImGui_ObjectAddr();															// ���f���̐���
	void ImGui_ObjModelLoad(std::string _filename[], int _filenum, bool _isshadow);		// ���f���̃��[�h
	void ImGui_ObjLists();																// ���f���̃��X�g

	// ���̑�
	void ImGui_UserGuide();			// ������@

private:
	int m_index;		// PS�V�F�[�_�[�̔ԍ��ۊǗp
	int m_Count = 0;	// ���O���h�~�p�J�E���g�ϐ�
	int m_Number = 0;	// �擾�ԍ�

	float m_AlphaLen = 0.75f;					// �A���t�@�f�B�U�K������
	float m_OutLineVal = 0.001f;				// �֊s���̕�
	float m_GammaValue = 1.5f;					// �K���}�l
	float m_Time = 0.0f;						// ����
	float m_ModelOutLineVal;					// ���f���ʂ̗֊s���̕�
	float m_ViewClipping[2] = { 0.2f,2.0f };	// ��ʊE�[�x�̋����ݒ�p
	float m_blurStrength = 0.25f;

	bool m_IsClick = false;		// �N���b�N����
	bool m_Outline = true;		// �֊s���̕`��
	bool m_GBuffer = true;		// G�o�b�t�@�̕`��
	bool m_AlwaysGamma = false;	// ��ɃK���}�␳�K���`��t���O
	bool m_IsShadow = true;		// �e�̕`��t���O
	bool m_blur = true;			// �u���[�t���O
	bool m_DoF = true;			// ��ʊE�[�x�t���O

	bool m_ModelListOriginalSetting = false;	// ���f���X�̃V�F�[�_�[�ݒ�
	bool m_ModelOutline;						// ���f���ʂ̗֊s���̕`��
	bool m_FrustumCulling = true;				// �t���X�^���J�����O
	bool m_isMulti = false;						// ��ʓh��Ԃ��t���O

	ImVec4 m_ModelOutLineColor;								// ���f���ʂ̗֊s���̐F�ϐ�
	ImVec4 m_LightColor;									// Light�̐F�ϐ�
	ImVec4 m_OutLineColor = { 0.0f,0.0f,0.0f,1.0f };		// �֊s���̐F�ϐ�
	
	// �e�̃N�I���e�B
	enum ShadowQuality
	{
		HIGH,
		MEDIUM,
		LOW
	};
	int m_ShadowQualityLevel;	// �e�̕i��
private:
	// ImGui
	bool m_ShowSceneEditWindow = true;			// ���̃V�[���̋��ʐݒ�GUI�\���t���O
	bool m_ShowUserGuideWindow = true;			// ���̃V�[���̑������GUI�\���t���O
	bool m_ShowAddGameObejctsWindow = true;		// �Q�[���I�u�W�F�N�g�𐶐�����GUI�\���t���O
	bool m_ShowGameObjectListWindow = true;		// ���������Q�[���I�u�W�F�N�g�̈ꗗGUI�\���t���O
	bool m_ShowInstanceWindow = true;			// �w�肵���I�u�W�F�N�g�̏ڍאݒ�GUI�\���t���O
	bool m_ShowScreenWindow = true;				// �����_�[�^�[�Q�b�g�̉摜GUI�\���t���O
};

#endif // __SCENE_VISUAL_H__