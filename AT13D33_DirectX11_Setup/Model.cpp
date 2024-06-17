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
	// シェーダー機能追加
	AddComponent<ShaderComponent>()->Load();

	// 通常モデル
	AddComponent<ModelMeshLoad>();
	AddComponent<ModelMeshRenderer>();

	// シェーダー機能設定追加
	AddComponent<ModelSetting>()->Init();
}
