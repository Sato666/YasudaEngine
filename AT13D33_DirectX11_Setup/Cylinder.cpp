//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "cylinder.h"
//-------------------------------------------------------
// オプション関係
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "modelRenderer.h"
#include "ShaderComponent.h"
#include "ModelSetting.h"
//-----------------------------------------------------------------------------
//      初期化処理です.
//-----------------------------------------------------------------------------
void Cylinder::GameObjInit()
{
	//-------------------------------------------------------
	// Model読み込み
	AddComponent<ShaderComponent>()->Load();
	AddComponent<ModelRenderer>()->Load("asset\\model\\Box\\cylinder.obj");

	// シェーダー機能設定追加
	AddComponent<ModelSetting>()->Init();
}
