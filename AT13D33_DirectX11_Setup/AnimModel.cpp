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
	// シェーダー機能追加
	AddComponent<ShaderComponent>()->Load();

	// アニメーション付きモデル
	AddComponent<AnimModelMeshLoad>();
	AddComponent<AnimModelMeshRenderer>();

	// シェーダー機能設定追加
	AddComponent<ModelSetting>()->Init();

	// アニメーションデータ読み込み
	//m_AnimData.LoadAnimation("filename[1]", "Run");
}

void AnimModel::GameObjUpdate()
{
	//static int frame = 0;

	// アニメーションメッシュ更新(ボーンコンビネーション行列を更新)
	//GetComponent<AnimModelMeshLoad>()->Update(m_AnimData, "NULL", frame);

	//frame++;

	// アニメーションメッシュレンダラ更新（ボーンコンビネーション行列を定数バッファに反映させる）
	//GetComponent<AnimModelMeshRenderer>()->Update(*GetComponent<AnimModelMeshLoad>());
}

void AnimModel::GameObjUninit()
{
	delete &m_AnimData;
}
