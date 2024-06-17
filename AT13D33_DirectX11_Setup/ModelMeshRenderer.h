//-------------------------------------------------------------------------------------
// Class : ModelMeshRenderer
// 親Class   :   Component
// 
// ModelMeshLoadクラスからモデルの情報を受け取り描画するクラス
// モデルの描画機能を持つ 
// Yasuda.Aoto  2023.09.27
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
#include "ModelMeshLoad.h"
#include "component.h"

class ModelMeshRenderer :
	public Component
{
public:
	using Component::Component;

	void Draw() override;				// 受け取った情報から描画
	void Init(ModelMeshLoad& mesh);		// ロードしたモデルの情報を受け取る
	void Uninit() override;				// 不必要になったデータ開放

	void SetMaterialShader(bool _is);		// マテリアルセットするかどうかのセッター

	std::vector<ID3D11ShaderResourceView*> GetDiffuseRTV() { return m_DiffuseSRVtable; }	// ディフューズテクスチャSRVを返す
	std::vector<SUBSET> GetSubSet() { return m_Subsets; }									// サブセットを返す
	std::vector<CMaterial*> GetMaterial() { return m_Materials; }							// マテリアル群を返す
private:
	std::vector<ID3D11ShaderResourceView*>	m_DiffuseSRVtable;		// ディフューズテクスチャSRV
	std::vector<SUBSET>	m_Subsets;					// サブセット情報

	std::vector<CMaterial*>		m_Materials;		// マテリアル群
	CVertexBuffer<MODELVERTEX>	m_VertexBuffer;		// 頂点バッファ
	CIndexBuffer				m_IndexBuffer;		// インデックスバッファ

	bool m_isMaterial = false;	// マテリアル情報を与えるかどうか
};



