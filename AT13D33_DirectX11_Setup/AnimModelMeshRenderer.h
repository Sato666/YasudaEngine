//-------------------------------------------------------------------------------------
// Class : AnimModelMeshRenderer
// 親Class   :   Component
// 
// AnimModelMeshLoadクラスからモデルの情報を受け取り描画するクラス
// モデルの描画機能を持つ 
// 
// Yasuda.Aoto  2023.10.23
//-------------------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include	<vector>
//-----------------------------------------------------------------------------
// その他
#include "renderer.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"
#include "AnimModelMeshLoad.h"
#include "component.h"

// ボーンコンビネーション行列用の定数バッファ
struct CBBoneCombMatrix {
	DirectX::XMFLOAT4X4 BoneCombMtx[400];	// ボーンコンビネーション行列
};

class AnimModelMeshRenderer :
	public Component
{
public:
	using Component::Component;

	void Draw() override;				// 受け取った情報から描画
	void Init(AnimModelMeshLoad& mesh);		// ロードしたモデルの情報を受け取る
	void Update(AnimModelMeshLoad& mesh);
	// 終了処理
	void Uninit() override;

	void SetMaterialShader(bool _is);		// マテリアルセットするかどうかのセッター

	std::vector<ID3D11ShaderResourceView*> GetDiffuseRTV() { return m_DiffuseSRVtable; }	// ディフューズテクスチャSRVを返す
	std::vector<SUBSET> GetSubSet() { return m_Subsets; }									// サブセットを返す
	std::vector<CMaterial*> GetMaterial() { return m_Materials; }							// マテリアル群を返す
private:
	ID3D11Buffer* m_BoneCombMtxCBuffer;

	std::vector<ID3D11ShaderResourceView*>	m_DiffuseSRVtable;	// ディフューズテクスチャSRV
	std::vector<SUBSET>	m_Subsets;					// サブセット情報

	std::vector<CMaterial*>			m_Materials;		// マテリアル群
	CVertexBuffer<ANIMMODELVERTEX>	m_VertexBuffer;		// 頂点バッファ
	CIndexBuffer					m_IndexBuffer;		// インデックスバッファ

	bool m_isMaterial = false;	// マテリアル情報を与えるかどうか
};
