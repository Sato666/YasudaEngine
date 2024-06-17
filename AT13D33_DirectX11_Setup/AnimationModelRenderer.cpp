#include "AnimationModelRenderer.h"

void AnimationModelRenderer::Draw()
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

	// 20230909-02 レジスタ2にセット
	Renderer::GetDeviceContext()->VSSetConstantBuffers(2, 1, &m_BoneCombMtxCBuffer);

	// サブセットの数分ループ（サブセット数はマテリアルの数）
	for (auto& subset : m_Subsets) {

		// (テクスチャ)SRVをセット
		devicecontext->PSSetShaderResources(0, 1, &m_DiffuseSRVtable[subset.MaterialIdx]);

		// マテリアル定数バッファをGPUにセット
		m_Materials[subset.MaterialIdx]->SetGPU();

		// ドローコール
		devicecontext->DrawIndexed(
			subset.IndexNum,	// 描画するインデックス数
			subset.IndexBase,	// インデックスバッファベース開始位置（この場所がインデックス０番目になる）	
			subset.VertexBase);	// 頂点バッファベース開始位置（この場所が頂点の０番目になる）

	}
}

void AnimationModelRenderer::Init(AnimaionModelLoad& mesh)
{
	// 頂点バッファ生成
	m_VertexBuffer.Create(mesh.GetVertices());

	// インデックスバッファ生成
	m_IndexBuffer.Create(mesh.GetIndices());

	// マテリアル情報を取得しマテリアル毎に定数バッファを生成する
	const std::vector<MATERIAL>& materials = mesh.GetMaterials();

	// マテリアル情報を取得しマテリアル毎に定数バッファを生成する
	m_Bone = mesh.GetBone();

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

	Model = &mesh;
}

void AnimationModelRenderer::Update(const char* AnimationName1, int Frame1, const char* AnimationName2, int Frame2, float BlendRate)
{
	// アニメーションありか？
	if (m_Animation.count(AnimationName1) == 0)
		return;
	if (m_Animation.count(AnimationName2) == 0)
		return;

	if (!m_Animation[AnimationName1]->HasAnimations())
		return;
	if (!m_Animation[AnimationName2]->HasAnimations())
		return;

	//アニメーションデータからボーンマトリクス算出
	aiAnimation* animation1 = m_Animation[AnimationName1]->mAnimations[0];
	aiAnimation* animation2 = m_Animation[AnimationName2]->mAnimations[0];

	// 現在のアニメーションについて関連するボーンを全て更新
	for (unsigned int c = 0; c < animation1->mNumChannels; c++)
	{
		aiNodeAnim* nodeAnim = animation1->mChannels[c];

		BONE* bone = &m_Bone[nodeAnim->mNodeName.C_Str()];

		int f;

		f = Frame1 % nodeAnim->mNumRotationKeys;//簡易実装
		aiQuaternion rot = nodeAnim->mRotationKeys[f].mValue;

		f = Frame1 % nodeAnim->mNumPositionKeys;//簡易実装
		aiVector3D pos = nodeAnim->mPositionKeys[f].mValue;

		bone->BlendPosFrom = pos;
		bone->BlendRFrom = rot;
	}

	// 現在のアニメーション2について関連するボーンを全て更新
	for (unsigned int c = 0; c < animation2->mNumChannels; c++)
	{
		aiNodeAnim* nodeAnim = animation2->mChannels[c];

		BONE* bone = &m_Bone[nodeAnim->mNodeName.C_Str()];

		int f;

		f = Frame1 % nodeAnim->mNumRotationKeys;//簡易実装
		aiQuaternion rot = nodeAnim->mRotationKeys[f].mValue;

		f = Frame1 % nodeAnim->mNumPositionKeys;//簡易実装
		aiVector3D pos = nodeAnim->mPositionKeys[f].mValue;

		bone->BlendPosTo = pos;
		bone->BlendRTo = rot;
	}

	// ブレンド
	for (unsigned int c = 0; c < animation2->mNumChannels; c++)
	{
		aiNodeAnim* nodeAnim = animation2->mChannels[c];

		BONE* bone = &m_Bone[nodeAnim->mNodeName.C_Str()];

		// 位置のブレンド
		aiVector3D pos1 = bone->BlendPosFrom;
		aiVector3D pos2 = bone->BlendPosTo;

		aiVector3D pos = pos1 * (1.0f - BlendRate) + pos2 * BlendRate;//線形補間

		// 姿勢のブレンド
		aiQuaternion rot1 = bone->BlendRFrom;
		aiQuaternion rot2 = bone->BlendRTo;

		aiQuaternion rot;
		aiQuaternion::Interpolate(rot, rot1, rot2, BlendRate);//球面線形補間

		bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot, pos);
	}

	//再帰的にボーンマトリクスを更新
	aiMatrix4x4 rootMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), aiQuaternion(AI_MATH_PI, 0.0f, 0.0f), aiVector3D(0.0f, 0.0f, 0.0f));
	rootMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), aiQuaternion(AI_MATH_PI, 0.0f, 0.0f), aiVector3D(0.0f, 0.0f, 0.0f));
	UpdateBoneMatrix(m_AiScene->mRootNode, rootMatrix);

	// 20230909 ボーンコンビネーション行列の配列を生成する
	std::vector<aiMatrix4x4> bonecombmtxcontainer;					// 20230909
	bonecombmtxcontainer.resize(m_Bone.size());						// 20230909
	for (auto data : m_Bone) {										// 20230909
		bonecombmtxcontainer[data.second.idx] = data.second.Matrix;	// 20230909
	}																// 20230909

	// 20230909 転置
	for (auto& bcmtx : bonecombmtxcontainer)
	{
		// 転置する
		bcmtx.Transpose();
	}

	// 20230909-02 定数バッファに反映させる
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	CBBoneCombMatrx* pData = nullptr;

	HRESULT hr = Renderer::GetDeviceContext()->Map(
		m_BoneCombMtxCBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&MappedResource);

	if (SUCCEEDED(hr))
	{
		pData = (CBBoneCombMatrx*)MappedResource.pData;
		memcpy(pData->BoneCombMtx,
			bonecombmtxcontainer.data(),
			sizeof(aiMatrix4x4) * bonecombmtxcontainer.size());
		Renderer::GetDeviceContext()->Unmap(m_BoneCombMtxCBuffer, 0);
	}
}

void AnimationModelRenderer::UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix)
{
	// 引数で渡されたノード名をキーとしてボーン情報を取得する
	BONE* bone = &m_Bone[node->mName.C_Str()];

	//マトリクスの乗算順番に注意
	aiMatrix4x4 worldMatrix;					// 初期値は単位行列

	//	worldMatrix *= matrix;						
	worldMatrix = matrix;						// 親の位置、姿勢が初期状態（逆ボーンオフセット行列）
	worldMatrix *= bone->AnimationMatrix;		// 引数で渡された行列を掛け算（自ノードのアニメーションを反映させる）（ボーン空間でのもの）

	bone->Matrix = worldMatrix;					// プログラム内に用意している行列に反映させる
	bone->Matrix *= bone->OffsetMatrix;			// オフセット行列を反映させる

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		UpdateBoneMatrix(node->mChildren[n], worldMatrix);
	}
}

void AnimationModelRenderer::Uninit()
{
	for (auto& mtrl : m_Materials)
	{
		mtrl->Uninit();
		delete mtrl;
	}

	for (auto& texres : m_DiffuseSRVtable)
	{
		if (texres != nullptr) {
			texres->Release();
		}
	}

	m_BoneCombMtxCBuffer->Release();						// 20230909-02

	aiReleaseImport(m_AiScene);

	// アニメーションが存在していたら解放する
	if (m_AiScene->HasAnimations())
	{
		for (std::pair<const std::string, const aiScene*> pair : m_Animation)
		{
			aiReleaseImport(pair.second);
		}
	}

	m_Bone.clear();
}

void AnimationModelRenderer::LoadAnimation(const char* FileName, const char* Name)
{
	m_Animation[Name] = aiImportFile(FileName, aiProcess_ConvertToLeftHanded);
	assert(m_Animation[Name]);
}