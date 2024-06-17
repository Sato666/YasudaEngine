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
#include "AnimaionModelLoad.h"
#include "component.h"

// ボーン 20230909-02
struct CBBoneCombMatrx
{
    DirectX::XMFLOAT4X4 BoneCombMtx[400];			// ボーンコンビネーション行列
};

class AnimationModelRenderer :
    public Component
{
public:
    using Component::Component;

    void LoadAnimation(const char* FileName, const char* Name);
    void Init(AnimaionModelLoad& mesh);		// ロードしたモデルの情報を受け取る
    void Update(const char* AnimationName1, int Frame1, const char* AnimationName2, int Frame2, float BlendRate);
    void UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix);	// ボーンマトリックス更新
    void Draw() override;				// 受け取った情報から描画
    void Uninit() override;				// 不必要になったデータ開放
private:
    AnimaionModelLoad* Model;
    std::unordered_map<std::string, BONE>	m_Bone;

    const aiScene* m_AiScene = nullptr;

    std::unordered_map<std::string, const aiScene*> m_Animation;	// アニメーションデータ

    std::vector<ID3D11ShaderResourceView*>	m_DiffuseSRVtable;		// ディフューズテクスチャSRV
    std::vector<ANIMSUBSET>	m_Subsets;					// サブセット情報

    ID3D11Buffer* m_BoneCombMtxCBuffer;						// 定数バッファ　20230909-02

    std::vector<CMaterial*>		    m_Materials;		    // マテリアル群
    CVertexBuffer<ANIMMODELVERTEX>	m_VertexBuffer;		    // 頂点バッファ
    CVertexBuffer<DEFORM_VERTEX>	m_DeformVertexBuffer;   // 変形後頂点バッファ
    CIndexBuffer				    m_IndexBuffer;		    // インデックスバッファ

};

