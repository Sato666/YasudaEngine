//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "ModelMeshRenderer.h"

#include "gameobject.h"
#include "ShaderComponent.h"
//-----------------------------------------------------------------------------
// 初期化処理です.
// ModelMeshLoadからモデルの情報を取得
//-----------------------------------------------------------------------------
void ModelMeshRenderer::Init(ModelMeshLoad& mesh)
{
	// 頂点バッファ生成
	m_VertexBuffer.Create(mesh.GetVertices());

	// インデックスバッファ生成
	m_IndexBuffer.Create(mesh.GetIndices());

	// マテリアル情報を取得しマテリアル毎に定数バッファを生成する
	const std::vector<MATERIAL>& materials = mesh.GetMaterials();

	for (auto& mtrl : materials)
	{
		CMaterial* m = new CMaterial();
		m->Init(mtrl);
		m_Materials.emplace_back(m);
	}

	// ディフューズテクスチャテーブルをメッシュから取得
	m_DiffuseSRVtable = mesh.GetSRVtable();

	// SUBSET情報取得
	m_Subsets = mesh.GetSubsets();
}
//-----------------------------------------------------------------------------
// 描画処理です.
//-----------------------------------------------------------------------------
void ModelMeshRenderer::Draw()
{
	// デバイスコンテキスト取得
	ID3D11DeviceContext* devicecontext;
	devicecontext = Renderer::GetDeviceContext();

	// 頂点バッファ設定
	m_VertexBuffer.SetGPU();
	// インデックスバッファ設定
	m_IndexBuffer.SetGPU();

	// トポロジーをセット（旧プリミティブタイプ）
	devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// サブセットの数分ループ（サブセット数はマテリアルの数）
	for (auto& subset : m_Subsets) {

		// (テクスチャ)SRVをセット
		devicecontext->PSSetShaderResources(0, 1, &m_DiffuseSRVtable[subset.MaterialIdx]);

		// マテリアルを取得する
		if (m_isMaterial)
		{
			m_Materials[subset.MaterialIdx]->SetShader(m_GameObject->GetComponent<ShaderComponent>()->GetVSShader());		// 現在のVSシェーダーを取得

			m_Materials[subset.MaterialIdx]->SetGPU(1);	// GPUにセット
		}

		// ドローコール
		devicecontext->DrawIndexed(
			subset.IndexNum,	// 描画するインデックス数
			subset.IndexBase,	// インデックスバッファベース開始位置（この場所がインデックス０番目になる）	
			subset.VertexBase);	// 頂点バッファベース開始位置（この場所が頂点の０番目になる）
	}
}
//-----------------------------------------------------------------------------
// 解放処理です.
//-----------------------------------------------------------------------------
void ModelMeshRenderer::Uninit()
{
	for (auto& mtrl : m_Materials)
	{
		delete mtrl;
	}
}
//-----------------------------------------------------------------------------
// マテリアルをセットする関数です.
//-----------------------------------------------------------------------------
void ModelMeshRenderer::SetMaterialShader(bool _is)
{
	m_isMaterial = _is;
}
