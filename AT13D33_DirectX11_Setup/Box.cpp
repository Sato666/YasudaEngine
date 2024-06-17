//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Box.h"
//-------------------------------------------------------
// �I�v�V�����֌W
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "ModelRenderer.h"
#include "ShaderComponent.h"
#include "ModelSetting.h"
//-----------------------------------------------------------------------------
//      �����������ł�.
//-----------------------------------------------------------------------------
void Box::GameObjInit()
{
	//-------------------------------------------------------
	// Model�ǂݍ���
	AddComponent<ShaderComponent>()->Load();
	AddComponent<ModelRenderer>()->Load("asset\\model\\Box\\box.obj");
	// �V�F�[�_�[�@�\�ݒ�ǉ�
	AddComponent<ModelSetting>()->Init();
}
