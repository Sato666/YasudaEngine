//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "AnimModel.h"

#include "ShaderComponent.h"
#include "ModelSetting.h"
#include "AnimModelMeshLoad.h"
#include "AnimModelMeshRenderer.h"

void AnimModel::GameObjInit()
{
	// �V�F�[�_�[�@�\�ǉ�
	AddComponent<ShaderComponent>()->Load();

	// �A�j���[�V�����t�����f��
	AddComponent<AnimModelMeshLoad>();
	AddComponent<AnimModelMeshRenderer>();

	// �V�F�[�_�[�@�\�ݒ�ǉ�
	AddComponent<ModelSetting>()->Init();

	// �A�j���[�V�����f�[�^�ǂݍ���
	//m_AnimData.LoadAnimation("filename[1]", "Run");
}

void AnimModel::GameObjUpdate()
{
	//static int frame = 0;

	// �A�j���[�V�������b�V���X�V(�{�[���R���r�l�[�V�����s����X�V)
	//GetComponent<AnimModelMeshLoad>()->Update(m_AnimData, "NULL", frame);

	//frame++;

	// �A�j���[�V�������b�V�������_���X�V�i�{�[���R���r�l�[�V�����s���萔�o�b�t�@�ɔ��f������j
	//GetComponent<AnimModelMeshRenderer>()->Update(*GetComponent<AnimModelMeshLoad>());
}

void AnimModel::GameObjUninit()
{
	delete &m_AnimData;
}
