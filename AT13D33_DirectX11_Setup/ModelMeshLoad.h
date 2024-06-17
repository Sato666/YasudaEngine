//-------------------------------------------------------------------------------------
// Class : ModelMeshLoad
// 親Class   :   Component
// 
// Assimp5.3.0を使ってモデルをロードするコンポーネント
// ロードした情報を返す機能を持つ
// 
// Yasuda.Aoto  2023.09.27
//-------------------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include	<d3d11.h>
#include	<DirectXMath.h>
#include	<wrl/client.h>
#include	<vector>
#include	<string>
#include	<unordered_map>
#include	<WICTextureLoader.h>
//-----------------------------------------------------------------------------
// Assimp
#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"

// debugかreleaseでAssimpライブラリを変更する
#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib,"assimp-vc143-mtd.lib")
#else
#pragma comment (lib, "assimp-vc143-mt.lib")
#endif//defined(DEBUG) || defined(_DEBUG)
//-----------------------------------------------------------------------------
// その他
#include "component.h"
#include "CIndexBuffer.h"
#include "CMaterial.h"
#include "CVertexBuffer.h"

// COMPTRを使えるように
using Microsoft::WRL::ComPtr;

// ３Ｄ頂点データ
struct MODELVERTEX
{
	DirectX::SimpleMath::Vector3 Position;
	DirectX::SimpleMath::Vector3 Normal;
	DirectX::SimpleMath::Color	Diffuse;
	DirectX::SimpleMath::Vector2 TexCoord;
};

class ModelMeshLoad :
    public Component
{
public:
	using Component::Component;
private:
	const aiScene* m_AiScene = nullptr;			// assimp scene

	std::vector<MODELVERTEX>	m_vertices;		// 頂点座標群
	std::vector<unsigned int>	m_indices;		// インデックスデータ群
	std::vector<SUBSET>			m_subsets;		// サブセット群
	std::vector<MATERIAL>		m_materials;	// マテリアルテーブル

	// メッシュに含まれる頂点を取り出す
	unsigned int GetVertices(const aiMesh* aimesh);
	// メッシュに含まれるインデックスを取り出す
	unsigned int  GetIndices(const aiMesh* aimesh);

	// サブセットを１つにまとめるためのベースを計算する
	void CalcMeshBaseIndex();

	// 全テクスチャを取得(辞書を作成する為)
	void GetTextures(const aiScene* aiscene);

	// 内蔵テクスチャかどうかを識別する
	bool isEmbeddedTexure(const aiTexture* aitexture);

	// assimpの内部データにセットされているテクスチャ名をキーとして保存
	std::unordered_map<std::string, ID3D11ShaderResourceView*> m_Texturedic;

	// マテリアルに含まれるデータを取得する
	void GetMaterials(const aiScene* aiscene);				// 全マテリアル分ループ
	void GetaMatreial(const aiMaterial* aimtrl);			// １つのマテリアル

	Texture texture;
	// テクスチャリソーステーブル
	std::vector<ID3D11ShaderResourceView*>	m_AllTexture;

	// テクスチャファイルを読み込んでSRVを生成する
	ID3D11ShaderResourceView* LoadDiffuseTexture(aiString _path);

	// このモデルのパス名
	std::string m_filename;
public:
	// 初期化
	bool Load(std::string filename);

	// 終了処理
	void Uninit() override;

	// 頂点データ取得
	const std::vector<MODELVERTEX>& GetVertices() {
		return m_vertices;
	}

	// インデックスデータ取得
	const std::vector<unsigned int>& GetIndices() {
		return m_indices;
	}

	// サブセットデータ取得
	const std::vector<SUBSET>& GetSubsets() {
		return m_subsets;
	}

	// SRVテーブル取得
	const std::vector<ID3D11ShaderResourceView*>& GetSRVtable() {
		return m_AllTexture;
	}

	// マテリアルデータ取得
	const std::vector<MATERIAL>& GetMaterials() {
		return m_materials;
	}
};

