//-------------------------------------------------------------------------------------
// Class : AnimModelMeshLoad
// 親Class   :   Component
// 
// Assimp5.3.0を使ってモデルをロードするコンポーネント
// アニメーション機能付きロードした情報を返す機能を持つ
// 
// Yasuda.Aoto  2023.10.23
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
#include "AnimationData.h"

// COMPTRを使えるように
using Microsoft::WRL::ComPtr;

// ３Ｄ頂点データ
struct ANIMMODELVERTEX
{
	DirectX::SimpleMath::Vector3	Position;
	DirectX::SimpleMath::Vector3	Normal;
	DirectX::SimpleMath::Color		Diffuse;
	DirectX::SimpleMath::Vector2	TexCoord;
	int		BoneIndex[4];	
	float	BoneWeight[4];	
	int		bonecnt = 0;			// 20231226
};

// ウェイト情報　20231225 追加
struct WEIGHT 
{
	std::string bonename;						// ボーン名
	std::string meshname;						// メッシュ名
	float weight;								// ウェイト値
	int	vertexindex;							// 頂点インデックス
};

//ボーン構造体
struct BONE
{
	std::string bonename;						// ボーン名
	std::string meshname;						// メッシュ名
	std::string armaturename;					// アーマチュア名
	aiMatrix4x4 Matrix{};						// 親子関係を考慮した行列
	aiMatrix4x4 AnimationMatrix{};				// 自分の事だけを考えた行列
	aiMatrix4x4 OffsetMatrix{};					// ボーンオフセット行列
	int			idx;							// 配列の何番目か			
	std::vector<WEIGHT> weights;				// このボーンが影響を与える頂点インデックス・ウェイト値
};

class AnimModelMeshLoad :
    public Component
{
private:
	const aiScene* m_AiScene = nullptr;			// assimp scene

	std::vector<ANIMMODELVERTEX>m_vertices;		// 頂点座標群
	std::vector<unsigned int>	m_indices;		// インデックスデータ群
	std::vector<SUBSET>			m_subsets;		// サブセット群
	std::vector<MATERIAL>		m_materials;	// マテリアルテーブル
	std::vector<aiMatrix4x4>	m_bonecombmtxcontainer;		// ボーンコンビネーション行列の配列

	// ボーン情報辞書（ボーン名で参照可能）				
	std::unordered_map<std::string, BONE> m_Bone;		
	// サブセット単位でボーン情報を集めたもの
	std::vector<std::vector<BONE>>	m_SubsetBones;
	// ボーン情報生成（ボーンの空情報を生成する）		
	void CreateBone(aiNode* node);
	// ボーン名、ボーンウェイトを頂点にセットする
	void SetBoneDataToVertices();						
	// ボーン情報を取得する
	std::vector<BONE> GetBoneInfo(const aiMesh* mesh);	

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
	using Component::Component;

	// 初期化
	bool Load(std::string filename);

	// 更新
	void Update(CAnimationData& animdata, const char* animname, int frmae);

	// 終了処理
	void Uninit() override;

	// 頂点データ取得
	const std::vector<ANIMMODELVERTEX>& GetVertices() {
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

	// ボーンコンビネーション行列を階層構造に従って更新する
	void UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix);

	// ボーンコンビネーション行列群を取得
	const std::vector<aiMatrix4x4>& GetBoneCombinationMatrix() {
		return m_bonecombmtxcontainer;
	}
};

