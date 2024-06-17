#include "SceneDefferedEdit.h"

void SceneDefferedEdit::Init()
{
	// �V�F�[�_�[�ǂݍ���
	Shader* shader[] = {
		CreateObj<VertexShader>("VS_Object"),			// �f�t�H���g�̒��_�V�F�[�_�[�i�s��Ԋ҂Ȃǁj
		CreateObj<PixelShader>("PS_TexColor"),			// �f�t�H���g�̃s�N�Z���V�F�[�_�[�i���̂܂ܐF���o���j
	};
	// �V�F�[�_�[��ǂݍ���ł���
	const char* filePath[] = {
	"Shader/VS/Default/VS_Object.cso",
	"Shader/PS/Default/PS_TexColor.cso",
	};
	static_assert(_countof(shader) == _countof(filePath), "");
	for (int i = 0; i < _countof(shader); ++i)
		shader[i]->Load(filePath[i]);

	// ��̃I�u�W�F�N�g�����Ă���
	Model* pNone = CreateObj<Model>("None");
	pNone->SetID("None");
	pNone->Init();
	pNone->GetComponent<ShaderComponent>()->SetVSShader(GetObj<Shader>("VS_Object"));		// Vs�V�F�[�_�[�Z�b�g
	pNone->GetComponent<ShaderComponent>()->SetOldVSShader(GetObj<Shader>("VS_Object"));	// �ۑ�Vs�V�F�[�_�[�Z�b�g
	pNone->GetComponent<ShaderComponent>()->SetPSShader(GetObj<Shader>("PS_TexColor"));		// Ps�V�F�[�_�[�Z�b�g
	pNone->GetComponent<ShaderComponent>()->SetOldPSShader(GetObj<Shader>("PS_TexColor"));	// �ۑ�Ps�V�F�[�_�[�Z�b�g
	ObjeList.push_back(pNone);		// ���X�g�ɒǉ�

	// Model���擾
	Model* pModel = CreateObj<Model>("Model");
	pModel->SetID("Model");
	pModel->SetName(u8"�f�B�t�@�[�h���f���N");
	pModel->Init();
	pModel->GetComponent<ShaderComponent>()->SetVSShader(GetObj<Shader>("VS_Object"));		// Vs�V�F�[�_�[�Z�b�g
	pModel->GetComponent<ShaderComponent>()->SetOldVSShader(GetObj<Shader>("VS_Object"));	// �ۑ�Vs�V�F�[�_�[�Z�b�g
	pModel->GetComponent<ShaderComponent>()->SetPSShader(GetObj<Shader>("PS_TexColor"));	// Ps�V�F�[�_�[�Z�b�g
	pModel->GetComponent<ShaderComponent>()->SetOldPSShader(GetObj<Shader>("PS_TexColor"));// �ۑ�Ps�V�F�[�_�[�Z�b�g
	// ���f����ǂݍ���
	pModel->AddComponent<ModelMeshLoad>()->Load(u8"asset/model/assimp/GothicMiku/Gothic Miku (Apearance Ver.)5.pmx");
	pModel->AddComponent<ModelMeshRenderer>()->Init(*pModel->GetComponent<ModelMeshLoad>());
	pModel->SetScale(DirectX::SimpleMath::Vector3(0.05f, 0.05f, 0.05f));	// �T�C�Y
	ObjeList.push_back(pModel);		// ���X�g�ɒǉ�

}

void SceneDefferedEdit::Uninit()
{
	// ���������I�u�W�F�N�g���폜���s��
	for (int i = 0; i < ObjeList.size(); i++)
	{
		// ���X�g�ɓ����Ă���I�u�W�F�N�g�̉��
		ObjeList[i]->Uninit();
	}

	// ���X�g�̉��
	ObjeList.clear();
}

void SceneDefferedEdit::Update(float tick)
{
	// ���������I�u�W�F�N�g��Loop����
	for (int i = 1; i < ObjeList.size(); i++)
	{
		// ���X�g�ɓ����Ă���I�u�W�F�N�g�̃A�b�v�f�[�g
		ObjeList[i]->Update();
	}
}

void SceneDefferedEdit::Draw()
{
	// ���L�I�u�W�F�N�g�̎擾
	Camera* pCamera = GetObj<Camera>("Camera");
	MoveLight* pLight = GetObj<MoveLight>("Light");

	// �ϊ��s��ݒ�
	DirectX::XMFLOAT4X4 mat[3];
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixIdentity());		// ���[���h
	mat[1] = pCamera->GetComponent<DXCamera>()->GetView();				// �r���[
	mat[2] = pCamera->GetComponent<DXCamera>()->GetProj();				// �v���W�F�N�V����

	// �V�F�[�_�[�ɒ萔�o�b�t�@��n��
	GetObj<Shader>("VS_Object")->WriteBuffer(0, mat);					// VS�f�t�H���g�i�ϊ��s��j

	// ���������I�u�W�F�N�g��Loop����
	for (int i = 1; i < ObjeList.size(); i++)
	{
		ObjeList[i]->GetComponent<ShaderComponent>()->SetVSShader(GetObj<Shader>("VS_Object"));	// �ۑ�Vs�V�F�[�_�[�Z�b�g
		ObjeList[i]->GetComponent<ShaderComponent>()->SetPSShader(
			ObjeList[i]->GetComponent<ShaderComponent>()->GetOldPSShader());	// �O��Ps�V�F�[�_�[�Z�b�g
		ObjeList[i]->Draw(GetObj<Shader>("VS_Object"), mat);					// ObjList�̕`��
	}
}

void SceneDefferedEdit::ImGui()
{

}
