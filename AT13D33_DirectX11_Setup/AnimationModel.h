//-------------------------------------------------------------------------------------
// Class : AnimationModel
// 親Class   :   Component
// 
// Assimpを使ってロードとアニメーション更新処理を持つ
// モデルの描画機能を持つ 
// 
// Yasuda.Aoto  2023.09.25
//-------------------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <unordered_map>
#include <d3d11.h>
#include <DirectXMath.h>
#include "component.h"
#include "CMaterial.h"
//-----------------------------------------------------------------------------
// Asiimp
#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"

// debugかreleaseでAssimpライブラリを変更する
#if defined(DEBUG) || defined(_DEBUG)
#pragma comment (lib, "assimp-vc143-mtd.lib")
#else
#pragma comment (lib, "assimp-vc143-mt.lib")
#endif//defined(DEBUG) || defined(_DEBUG)

//-----------------------------------------------------------------------------
// Constant Values.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// ３Ｄ頂点データ
struct VERTEX_3D
{
	DirectX::SimpleMath::Vector3 Position;
	DirectX::SimpleMath::Vector3 Normal;
	DirectX::SimpleMath::Color	 Diffuse;
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
	std::string		BoneName[4];			// 本来はボーンインデックスで管理するべき
	float			BoneWeight[4];
	int				BoneIdx[4];
	// ボーン情報へのインデックス 20230909
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
// ボーン 20230909-02
struct CBBoneCombMatrx
{
	DirectX::XMFLOAT4X4 BoneCombMtx[400];			// ボーンコンビネーション行列
};

class AnimationModel :
	public Component
{
private:
	const aiScene* m_AiScene = nullptr;
	std::unordered_map<std::string, const aiScene*> m_Animation;	// アニメーションデータ

	ID3D11Buffer** m_VertexBuffer;	// 頂点バッファ
	ID3D11Buffer** m_IndexBuffer;	// インデックスバッファ

	// テクスチャ補間
	std::unordered_map<std::string, ID3D11ShaderResourceView*> m_Texture;

	std::vector<DEFORM_VERTEX>* m_DeformVertex;				//変形後頂点データ
	std::unordered_map<std::string, BONE> m_Bone;			//ボーンデータ（名前で参照）

	void CreateBone(aiNode* Node);								// ボーン生成
	void UpdateBoneMatrix(aiNode* Node, aiMatrix4x4 Matrix);	// ボーンマトリックス更新

	ID3D11Buffer* m_BoneCombMtxCBuffer;						// 定数バッファ　20230909-02

	CMaterial*	m_Material;		// マテリアル
public:
	using Component::Component;

	void Load(const char* FileName);
	void LoadAnimation(const char* FileName, const char* Name);
	void Uninit() override;
	void Update(const char* AnimationName1, int Frame1, const char* AnimationName2, int Frame2, float BlendRate);
	void Draw() override;
};
