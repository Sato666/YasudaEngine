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

// ３Ｄ頂点データ
struct ANIMMODELVERTEX
{
	DirectX::SimpleMath::Vector3 Position;
	DirectX::SimpleMath::Vector3 Normal;
	DirectX::SimpleMath::Color	Diffuse;
	DirectX::SimpleMath::Vector2 TexCoord;
	int			BoneIndex[4];
	float		BoneWeight[4];
};

//変形後頂点構造体
struct DEFORM_VERTEX
{
	aiVector3D		Position;
	aiVector3D		Normal;
	int				BoneNum;
	std::string		BoneName[4];
	float			BoneWeight[4];
	int				BoneIdx[4];
};

//ボーン構造体
struct BONE
{
	aiMatrix4x4 Matrix;						// 座標変換に使用する行列
	aiMatrix4x4 AnimationMatrix;			// 自分の事だけ考えた行列
	aiMatrix4x4 OffsetMatrix;				// ボーンオフセット行列
	aiQuaternion BlendRFrom;				// モーションブレンドする際のFROM側
	aiQuaternion BlendRTo;					// モーションブレンドする際のTO側
	aiVector3D	BlendPosFrom;				// モーションブレンドする際のFROM側
	aiVector3D	BlendPosTo;					// モーションブレンドする際のTO側
	int			idx;						// 20230909
};

// メッシュ
struct ANIMSUBSET {
	unsigned int	IndexNum = 0;					// インデックス数
	unsigned int	VertexNum = 0;					// 頂点数
	unsigned int	BoneNum = 0;					// ボーン数
	unsigned int	IndexBase = 0;					// 開始インデックス
	unsigned int	VertexBase = 0;					// 頂点ベース
	unsigned int	BoneBase = 0;					// ボーンベース
	unsigned int	MaterialIdx = 0;				// マテリアルインデックス
};

class AnimaionModelLoad :
    public Component
{
private:
	std::vector<ANIMMODELVERTEX>			m_vertices;			// 頂点座標群
	std::vector<unsigned int>				m_indices;			// インデックスデータ群
	std::vector<ANIMSUBSET>					m_subsets;			// サブセット群
	std::vector<MATERIAL>					m_materials;		// マテリアルテーブル
	std::vector<DEFORM_VERTEX>*				m_DeformVertex;		// 変形後頂点データ
	std::unordered_map<std::string, BONE>	m_Bone;				// ボーンデータ（名前で参照）

	// メッシュに含まれる頂点を取り出す
	unsigned int GetVertices(const aiMesh* aimesh,int i);
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

	// テクスチャリソーステーブル
	std::vector<ID3D11ShaderResourceView*>	m_AllTexture;

	// テクスチャファイルを読み込んでSRVを生成する
	ID3D11ShaderResourceView* LoadDiffuseTexture(std::string filename);

	// ボーン生成
	void CreateBone(aiNode* node);	

	// このモデルのパス名
	std::string m_filename;
public:
	using Component::Component;

	// 初期化
	bool Load(std::string filename);

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

	// 変形後頂点データ取得
	const std::vector<DEFORM_VERTEX>* GetDeformVertex() {
		return m_DeformVertex;
	}

	// ボーンデータ取得
	const std::unordered_map<std::string, BONE> GetBone() {
		return m_Bone;
	}

	// サブセットデータ取得
	const std::vector<ANIMSUBSET>& GetSubsets() {
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

