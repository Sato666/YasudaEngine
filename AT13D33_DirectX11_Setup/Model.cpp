//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Model.h"

#include "ShaderComponent.h"
#include "ModelSetting.h"
#include "ModelMeshLoad.h"
#include "ModelMeshRenderer.h"

void Model::GameObjInit()
{
	// �V�F�[�_�[�@�\�ǉ�
	AddComponent<ShaderComponent>()->Load();

	// �ʏ탂�f��
	AddComponent<ModelMeshLoad>();
	AddComponent<ModelMeshRenderer>();

	// �V�F�[�_�[�@�\�ݒ�ǉ�
	AddComponent<ModelSetting>()->Init();
}
