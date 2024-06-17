#include "SceneDefferedEdit.h"

void SceneDefferedEdit::Init()
{
	// シェーダー読み込み
	Shader* shader[] = {
		CreateObj<VertexShader>("VS_Object"),			// デフォルトの頂点シェーダー（行列返還など）
		CreateObj<PixelShader>("PS_TexColor"),			// デフォルトのピクセルシェーダー（そのまま色を出す）
	};
	// シェーダーを読み込んでくる
	const char* filePath[] = {
	"Shader/VS/Default/VS_Object.cso",
	"Shader/PS/Default/PS_TexColor.cso",
	};
	static_assert(_countof(shader) == _countof(filePath), "");
	for (int i = 0; i < _countof(shader); ++i)
		shader[i]->Load(filePath[i]);

	// 空のオブジェクトを入れておく
	Model* pNone = CreateObj<Model>("None");
	pNone->SetID("None");
	pNone->Init();
	pNone->GetComponent<ShaderComponent>()->SetVSShader(GetObj<Shader>("VS_Object"));		// Vsシェーダーセット
	pNone->GetComponent<ShaderComponent>()->SetOldVSShader(GetObj<Shader>("VS_Object"));	// 保存Vsシェーダーセット
	pNone->GetComponent<ShaderComponent>()->SetPSShader(GetObj<Shader>("PS_TexColor"));		// Psシェーダーセット
	pNone->GetComponent<ShaderComponent>()->SetOldPSShader(GetObj<Shader>("PS_TexColor"));	// 保存Psシェーダーセット
	ObjeList.push_back(pNone);		// リストに追加

	// Modelを取得
	Model* pModel = CreateObj<Model>("Model");
	pModel->SetID("Model");
	pModel->SetName(u8"ディファードモデル君");
	pModel->Init();
	pModel->GetComponent<ShaderComponent>()->SetVSShader(GetObj<Shader>("VS_Object"));		// Vsシェーダーセット
	pModel->GetComponent<ShaderComponent>()->SetOldVSShader(GetObj<Shader>("VS_Object"));	// 保存Vsシェーダーセット
	pModel->GetComponent<ShaderComponent>()->SetPSShader(GetObj<Shader>("PS_TexColor"));	// Psシェーダーセット
	pModel->GetComponent<ShaderComponent>()->SetOldPSShader(GetObj<Shader>("PS_TexColor"));// 保存Psシェーダーセット
	// モデルを読み込み
	pModel->AddComponent<ModelMeshLoad>()->Load(u8"asset/model/assimp/GothicMiku/Gothic Miku (Apearance Ver.)5.pmx");
	pModel->AddComponent<ModelMeshRenderer>()->Init(*pModel->GetComponent<ModelMeshLoad>());
	pModel->SetScale(DirectX::SimpleMath::Vector3(0.05f, 0.05f, 0.05f));	// サイズ
	ObjeList.push_back(pModel);		// リストに追加

}

void SceneDefferedEdit::Uninit()
{
	// 生成したオブジェクト分削除を行う
	for (int i = 0; i < ObjeList.size(); i++)
	{
		// リストに入っているオブジェクトの解放
		ObjeList[i]->Uninit();
	}

	// リストの解放
	ObjeList.clear();
}

void SceneDefferedEdit::Update(float tick)
{
	// 生成したオブジェクト分Loopする
	for (int i = 1; i < ObjeList.size(); i++)
	{
		// リストに入っているオブジェクトのアップデート
		ObjeList[i]->Update();
	}
}

void SceneDefferedEdit::Draw()
{
	// 共有オブジェクトの取得
	Camera* pCamera = GetObj<Camera>("Camera");
	MoveLight* pLight = GetObj<MoveLight>("Light");

	// 変換行列設定
	DirectX::XMFLOAT4X4 mat[3];
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixIdentity());		// ワールド
	mat[1] = pCamera->GetComponent<DXCamera>()->GetView();				// ビュー
	mat[2] = pCamera->GetComponent<DXCamera>()->GetProj();				// プロジェクション

	// シェーダーに定数バッファを渡す
	GetObj<Shader>("VS_Object")->WriteBuffer(0, mat);					// VSデフォルト（変換行列）

	// 生成したオブジェクト分Loopする
	for (int i = 1; i < ObjeList.size(); i++)
	{
		ObjeList[i]->GetComponent<ShaderComponent>()->SetVSShader(GetObj<Shader>("VS_Object"));	// 保存Vsシェーダーセット
		ObjeList[i]->GetComponent<ShaderComponent>()->SetPSShader(
			ObjeList[i]->GetComponent<ShaderComponent>()->GetOldPSShader());	// 前のPsシェーダーセット
		ObjeList[i]->Draw(GetObj<Shader>("VS_Object"), mat);					// ObjListの描画
	}
}

void SceneDefferedEdit::ImGui()
{

}
