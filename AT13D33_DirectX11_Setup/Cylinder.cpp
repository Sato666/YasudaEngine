//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "cylinder.h"
//-------------------------------------------------------
// �I�v�V�����֌W
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "modelRenderer.h"
#include "ShaderComponent.h"
#include "ModelSetting.h"
//-----------------------------------------------------------------------------
//      �����������ł�.
//-----------------------------------------------------------------------------
void Cylinder::GameObjInit()
{
	//-------------------------------------------------------
	// Model�ǂݍ���
	AddComponent<ShaderComponent>()->Load();
	AddComponent<ModelRenderer>()->Load("asset\\model\\Box\\cylinder.obj");

	// �V�F�[�_�[�@�\�ݒ�ǉ�
	AddComponent<ModelSetting>()->Init();
}
