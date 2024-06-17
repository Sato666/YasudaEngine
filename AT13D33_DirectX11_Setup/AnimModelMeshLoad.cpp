//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "AnimModelMeshLoad.h"
#include <SimpleMath.h>
#include <DirectXTex.h>
#include <iostream>		// std::string用
//-----------------------------------------------------------------------------
// 必要な機能
#include "main.h"
#include "renderer.h"
#include "utility.h"
//-----------------------------------------------------------------------------
// ロード処理です.
//-----------------------------------------------------------------------------
bool AnimModelMeshLoad::Load(std::string filename)
{
	bool sts{};

	m_filename = filename;			// テクスチャ読み込み用に保存
	//-----------------------------------------------------------------------------
	// シーン情報構築
	Assimp::Importer importer;

	m_AiScene = importer.ReadFile(
		filename.c_str(),
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_FlipUVs |
		aiProcess_PreTransformVertices |
		aiProcess_MakeLeftHanded);

	// エラー処理
	if (m_AiScene == nullptr)
	{
		printf("load error %s ( %s ) \n ", filename.c_str(), importer.GetErrorString());
		return false;
	}

	// Model名表記
	std::filesystem::path p = std::filesystem::u8path(filename);
	std::cout << std::endl;
	std::cout << "Model名  : " << p.stem() << std::endl;
	std::cout << "拡張子 : " << p.extension() << std::endl;

	//-----------------------------------------------------------------------------
	// モデル情報取得
	
	//再帰的にボーン辞書（空で階層構造生成）
	CreateBone(m_AiScene->mRootNode);

	// ボーンの配列位置を格納する	
	unsigned int num = 0;			
	for (auto& data : m_Bone) {		
		data.second.idx = num;		
		num++;						
	}								

	// メッシュ数でメッシュコンテナ配列数を決定
	m_subsets.resize(m_AiScene->mNumMeshes);

	// メッシュ数分ループ
	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++) {

		aiMesh* mesh{};
		mesh = m_AiScene->mMeshes[m];

		// メッシュ単位で頂点データを取得
		int vnum = GetVertices(mesh);

		// メッシュ単位でインデックスデータ取得
		int idxnum = GetIndices(mesh);

		// メッシュで使用しているマテリアル情報を取得
		int mtrlidx = mesh->mMaterialIndex;

		// インデックス数を保存
		m_subsets[m].IndexNum = idxnum;

		// 頂点数を保存
		m_subsets[m].VertexNum = vnum;

		// マテリアルインデックスを保存
		m_subsets[m].MaterialIdx = mtrlidx;

		// ボーン情報を取得する(サブセット単位で)
		m_SubsetBones.emplace_back(GetBoneInfo(mesh));
	}

	// 頂点数表示
	std::cout << "頂点数 : " << m_vertices.size() << std::endl;
	std::cout << std::endl;

	// メッシュのベース情報を構築
	CalcMeshBaseIndex();

	// ボーンIDとウエイト値を頂点データに反映させる
	// 必ずCalcMeshBaseIndex()の後で行う事
	SetBoneDataToVertices();

	// 全テクスチャを取得
	GetTextures(m_AiScene);

	// マテリアルを取得
	GetMaterials(m_AiScene);

	std::cout << "----------------------------------------" << std::endl;

	return true;
}
//-----------------------------------------------------------------------------
// 解放処理です.
//-----------------------------------------------------------------------------
void AnimModelMeshLoad::Uninit()
{
	// SRV解放
	for (auto& texres : m_Texturedic)
	{
		if (texres.second != nullptr)
		{
			texres.second->Release();
		}
	}

	for (auto bone : m_Bone)
	{
		if (&bone.second != nullptr)
		{
			bone.second;
		}
	}

	m_AllTexture.clear();
	m_vertices.clear();
	m_indices.clear();
	m_materials.clear();
	m_subsets.clear();
	m_SubsetBones.clear();
	m_bonecombmtxcontainer.clear();
}
//-----------------------------------------------------------------------------
// メッシュ単位で頂点データを取得して返す
//-----------------------------------------------------------------------------
unsigned int AnimModelMeshLoad::GetVertices(const aiMesh* aimesh)
{
	// 頂点数分ループ
	for (unsigned int vidx = 0; vidx < aimesh->mNumVertices; vidx++)
	{
		ANIMMODELVERTEX v;

		// 位置
		v.Position.x = aimesh->mVertices[vidx].x;
		v.Position.y = aimesh->mVertices[vidx].y;
		v.Position.z = aimesh->mVertices[vidx].z;

		// 法線あり？
		if (aimesh->HasNormals()) {
			v.Normal.x = aimesh->mNormals[vidx].x;
			v.Normal.y = aimesh->mNormals[vidx].y;
			v.Normal.z = aimesh->mNormals[vidx].z;
		}
		else {
			v.Normal = DirectX::SimpleMath::Vector3(0, 0, 0);
		}

		// テクスチャあり？
		if (aimesh->HasTextureCoords(0)) {
			v.TexCoord.x = aimesh->mTextureCoords[0][vidx].x;				// 20230809 bug fix by ts
			v.TexCoord.y = aimesh->mTextureCoords[0][vidx].y;				// 20230809 bug fix by ts
		}
		else {
			v.TexCoord = DirectX::SimpleMath::Vector2(0, 0);
		}

		// 頂点カラーあり？
		if (aimesh->HasVertexColors(0)) {
			v.Diffuse.x = aimesh->mColors[0][vidx].r;
			v.Diffuse.y = aimesh->mColors[0][vidx].g;
			v.Diffuse.z = aimesh->mColors[0][vidx].b;
			v.Diffuse.w = aimesh->mColors[0][vidx].a;
		}
		else {
			v.Diffuse = DirectX::SimpleMath::Vector4(1, 1, 1, 1);								// 無かった場合は白
		}

		// 頂点データを保存
		m_vertices.emplace_back(v);
	}

	// aiMeshが保持する頂点数を戻す
	return aimesh->mNumVertices;
}
//-----------------------------------------------------------------------------
// メッシュ単位でインデックスデータを取得して返す
//-----------------------------------------------------------------------------
unsigned int AnimModelMeshLoad::GetIndices(const aiMesh* aimesh)
{
	// 面数分ループ
	for (unsigned int f = 0; f < aimesh->mNumFaces; f++)
	{
		const aiFace* face = &aimesh->mFaces[f];
		assert(face->mNumIndices <= 3);				// ポリゴンが3頂点ではなければエラー

		// インデックスデータを保存
		m_indices.emplace_back(face->mIndices[0]);
		m_indices.emplace_back(face->mIndices[1]);
		m_indices.emplace_back(face->mIndices[2]);
	}

	return aimesh->mNumFaces * 3;	// 3分割
}
//-----------------------------------------------------------------------------
// メッシュのベースを生成する処理
//-----------------------------------------------------------------------------
void AnimModelMeshLoad::CalcMeshBaseIndex()
{
	// サブセット数分ループ
	for (int m = 0; m < m_subsets.size(); m++)
	{
		// 頂点バッファのベースを計算
		m_subsets[m].VertexBase = 0;
		for (int i = m - 1; i >= 0; i--) {
			m_subsets[m].VertexBase += m_subsets[i].VertexNum;
		}

		// インデックスバッファのベースを計算
		m_subsets[m].IndexBase = 0;
		for (int i = m - 1; i >= 0; i--) {
			m_subsets[m].IndexBase += m_subsets[i].IndexNum;
		}
	}
}
//-----------------------------------------------------------------------------
// モデルのテクスチャーを取得する処理.
//-----------------------------------------------------------------------------
void AnimModelMeshLoad::GetTextures(const aiScene* aiscene)
{
	ID3D11DeviceContext* devicecontext;
	ID3D11Device* device;

	device = Renderer::GetDevice();
	devicecontext = Renderer::GetDeviceContext();

	// このシーンが持つテクスチャを全て読み込む
	for (int texidx = 0; texidx < aiscene->mNumTextures; texidx++)
	{
		// テクスチャを取得
		aiTexture* aitexture = aiscene->mTextures[texidx];

		// 内蔵テクスチャか？
		if (isEmbeddedTexure(aitexture))
		{
			ID3D11ShaderResourceView* srv;

			aiTexture* aitexture = aiscene->mTextures[texidx];

			const size_t size = aitexture->mWidth;

			// SRV生成
			HRESULT hr = DirectX::CreateWICTextureFromMemory(
				device,
				devicecontext,
				reinterpret_cast<const unsigned char*>(aitexture->pcData),
				aitexture->mWidth,
				nullptr,
				&srv);

			assert(srv);

			// assimpの内部データにセットされているテクスチャ名をキーとして保存
			m_Texturedic[aitexture->mFilename.data] = srv;
		}
		else
		{
			// 内蔵テクスチャではない場合

			// テクスチャファイル名を取得する
			std::string filename = GetFileName(aitexture->mFilename.C_Str());

			// テクスチャファイルを読み込みSRVを生成
			// SRV
			ID3D11ShaderResourceView* srv = nullptr;

			bool sts = CreateSRVfromFile(
				filename.c_str(),
				device,
				devicecontext,
				&srv);

			if (sts) {
				// assimpの内部データにセットされているテクスチャ名をキーとして保存
				m_Texturedic[aitexture->mFilename.data] = srv;
			}
			else {
				m_Texturedic[aitexture->mFilename.data] = nullptr;
			}
		}
	}
}
//-----------------------------------------------------------------------------
// モデルに内蔵テクスチャがあるかを返す処理.
//-----------------------------------------------------------------------------
bool AnimModelMeshLoad::isEmbeddedTexure(const aiTexture* aitexture)
{
	if (aitexture->mHeight == 0 ||
		aitexture->mWidth == 0 ||
		aitexture->pcData == nullptr) {
		return true;
	}
	return false;
}
//-----------------------------------------------------------------------------
// モデルのマテリアル情報を取得してセットする処理.
//-----------------------------------------------------------------------------
void AnimModelMeshLoad::GetMaterials(const aiScene* aiscene)
{
	// このモデルで使用しているすべてのマテリアル情報を取得
	for (unsigned int mrlidx = 0; mrlidx < aiscene->mNumMaterials; mrlidx++) {

		// このメッシュ用のマテリアルを取得
		aiMaterial* mtl;
		mtl = aiscene->mMaterials[mrlidx];

		// マテリアル情報取得
		GetaMatreial(mtl);
	}
}
//-----------------------------------------------------------------------------
// モデルのマテリアル情報を取得してセットする処理.
//-----------------------------------------------------------------------------
void AnimModelMeshLoad::GetaMatreial(const aiMaterial* aimtrl)
{
	// デバイス、デバイスコンテキスト取得
	ID3D11DeviceContext* devicecontext;
	ID3D11Device* device;

	device = Renderer::GetDevice();
	devicecontext = Renderer::GetDeviceContext();

	// マテリアル情報
	MATERIAL mymaterial{};

	// 各種パラメーター
	aiColor3D color(0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT4 diffuse(1.0f, 1.0f, 1.0f, 1.0f);
	DirectX::XMFLOAT4 ambient(0.3f, 0.3f, 0.3f, 1.0f);
	float shinness = 0.0f;

	// 環境光反射成分
	mymaterial.Ambient = aimtrl->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS ?
		DirectX::XMFLOAT4(color.r, color.g, color.b, 1.0f) : ambient;
	// 拡散反射成分
	mymaterial.Diffuse = aimtrl->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS ?
		DirectX::XMFLOAT4(color.r, color.g, color.b, 1.0f) : diffuse;
	// 鏡面反射成分
	mymaterial.Specular = aimtrl->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS ?
		DirectX::XMFLOAT4(color.r, color.g, color.b, shinness) : DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, shinness);
	// 自家発光成分
	mymaterial.Emission = aimtrl->Get(AI_MATKEY_COLOR_EMISSIVE, color) == AI_SUCCESS ?
		DirectX::XMFLOAT4(color.r, color.g, color.b, 1.0f) : DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	// 鏡面反射強度
	mymaterial.Shininess = aimtrl->Get(AI_MATKEY_SHININESS, shinness) == AI_SUCCESS ? shinness : 0.0f;

	// ディフューズテクスチャ数取得
	aiTextureType type = aiTextureType_DIFFUSE;
	int texnum = aimtrl->GetTextureCount(type);

	// 1メッシュにディフーズテクスチャ１枚しか許可せず
	assert(texnum <= 1);

	// マテリアル名
	aiString str;

	// assimpからテクスチャファイル名を取得
	aimtrl->GetTexture(type, 0, &str);

	// SRV
	ID3D11ShaderResourceView* srv = nullptr;

	aiString path;
	if (aimtrl->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) == AI_SUCCESS)
	{
		// SRV辞書に存在するか？
		if (m_Texturedic.find(str.data) == m_Texturedic.end())
		{
			mymaterial.TextureEnable = FALSE;
			srv = nullptr;

			// pmx の場合シーンに登録されていないテクスチャが存在する
			aimtrl->GetTexture(type, 0, &str);

			// テクスチャ取得
			srv = LoadDiffuseTexture(path);

			if (srv == nullptr)
			{
				mymaterial.TextureEnable = FALSE;
			}
			else
			{
				mymaterial.TextureEnable = TRUE;
			}
		}
	}
	else
	{
		mymaterial.TextureEnable = TRUE;
		srv = m_Texturedic[str.data];
	}

	// SRV保存
	m_AllTexture.emplace_back(srv);

	// マテリアル情報を保存
	m_materials.emplace_back(mymaterial);
}
//-----------------------------------------------------------------------------
// モデルに内蔵テクスチャがない場合同じディレクトリからテクスチャを取得する処理.
//-----------------------------------------------------------------------------
ID3D11ShaderResourceView* AnimModelMeshLoad::LoadDiffuseTexture(aiString _path)
{
	// u8エンコードでpathをコンストラクト
	std::filesystem::path p = std::filesystem::u8path(m_filename);

	// ファイルの探索
	std::string dir = p.string();
	dir = dir.substr(0, dir.find_last_of('/') + 1);		// 最初のファイルパスの～/まで取ってくる

	// テクスチャ
	aiString path = _path;	// モデルにアタッチされているテクスチャ名取得
	HRESULT hr = texture.Create(path.C_Str());		// そのまま探索

	// モデルと同じ階層を探索
	if (FAILED(hr)) {
		std::string file = dir;
		file += GetFileName(path.C_Str());
		hr = texture.Create(file.c_str());

		// ファイルパス取得
		p = file;
		std::cout << "ディレクトリ名 : " << file.c_str() << std::endl;
		std::cout << "ファイル名  : " << p.filename() << std::endl;
		std::cout << "拡張子 : " << p.extension() << std::endl;
	}
	// 失敗
	if (FAILED(hr)) {
		std::cout << "LoadTextureFile Error (" << GetFileName(path.C_Str()) << ")" << std::endl;
		return nullptr;
	}
	return texture.GetResource();
}

// ボーン情報生成(スタティックメッシュ内の全てのボーン辞書を生成する)
void AnimModelMeshLoad::CreateBone(aiNode* node)
{
	std::cout << "ボーン名 : " << GetFileName(node->mName.C_Str()) << std::endl;

	// ボーン名で参照できるように空のボーン情報をセットする
	m_Bone[node->mName.C_Str()];

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		CreateBone(node->mChildren[n]);
	}
}

// ボーン名、ボーンインデックス、ボーンウェイトを頂点にセットする
void AnimModelMeshLoad::SetBoneDataToVertices()
{
	// ボーンインデックスを初期化
	for (auto& v : m_vertices) {
		v.bonecnt = 0;
		for (int b = 0; b < 4; b++) {
			v.BoneIndex[b] = -1;
			v.BoneWeight[b] = 0.0f;
		}
	}

	// サブセットメッシュ毎のコンテナ
	int subsetid = 0;
	for (auto& bones : m_SubsetBones) {

		// このスタティックメッシュ内の頂点データのスタート位置を取得
		int vertexbase = m_subsets[subsetid].VertexBase;

		// このサブセット内のボーンをひとつづつ取り出す
		for (auto& bone : bones)
		{
			for (auto& w : bone.weights) {
				int& idx = m_vertices[vertexbase + w.vertexindex].bonecnt;
				m_vertices[vertexbase + w.vertexindex].BoneWeight[idx] = w.weight;	// weight値をセット
				m_vertices[vertexbase + w.vertexindex].BoneIndex[idx] = m_Bone[w.bonename].idx;
				//ボーンの配列番号をセット
				idx++;
				assert(idx <= 4);
			}
		}
		subsetid++;				// 次のサブセットへ
	}
}

// サブセットに紐づいているボーン情報を取得する
std::vector<BONE> AnimModelMeshLoad::GetBoneInfo(const aiMesh* mesh)
{
	std::vector<BONE> bones;		// このサブセットメッシュで使用されているボーンコンテナ

	// ボーン数分ループ
	for (unsigned int bidx = 0; bidx < mesh->mNumBones; bidx++) {

		BONE bone{};

		// ボーン名取得
		bone.bonename = mesh->mBones[bidx]->mName.C_Str();
		// メッシュノード名
		bone.meshname = mesh->mBones[bidx]->mNode->mName.C_Str();
		// アーマチュアノード名
		bone.armaturename = mesh->mBones[bidx]->mArmature->mName.C_Str();

		// ボーンオフセット行列取得
		bone.OffsetMatrix = mesh->mBones[bidx]->mOffsetMatrix;

		// ウェイト情報抽出
		bone.weights.clear();
		for (unsigned int widx = 0; widx < mesh->mBones[bidx]->mNumWeights; widx++) {

			WEIGHT w;
			w.meshname = bone.meshname;										// メッシュ名
			w.bonename = bone.bonename;										// ボーン名

			w.weight = mesh->mBones[bidx]->mWeights[widx].mWeight;			// 重み
			w.vertexindex = mesh->mBones[bidx]->mWeights[widx].mVertexId;	// 頂点インデックス
			bone.weights.emplace_back(w);
		}

		// コンテナに登録
		bones.emplace_back(bone);

		// ボーン辞書にも反映させる
		m_Bone[mesh->mBones[bidx]->mName.C_Str()].OffsetMatrix = mesh->mBones[bidx]->mOffsetMatrix;
	}

	return bones;
}

void AnimModelMeshLoad::UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix)
{
	// 引数で渡されたノード名をキーとしてボーン情報を取得する
	BONE* bone = &m_Bone[GetFileName(node->mName.C_Str())];

	//マトリクスの乗算順番に注意
	aiMatrix4x4 worldMatrix;					// 初期値は単位行列			

	worldMatrix = matrix;						// 親の位置、姿勢が初期状態（逆ボーンオフセット行列）
	worldMatrix *= bone->AnimationMatrix;		// 引数で渡された行列を掛け算（自ノードのアニメーションを反映させる）（ボーン空間でのもの）

	bone->Matrix = worldMatrix;					// プログラム内に用意している行列に反映させる
	bone->Matrix *= bone->OffsetMatrix;			// オフセット行列を反映させる

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		UpdateBoneMatrix(node->mChildren[n], worldMatrix);
	}
}

void AnimModelMeshLoad::Update(CAnimationData& animdata,const char* animname ,int frmae)
{
	// ルート行列生成
	aiMatrix4x4 rootMatrix = aiMatrix4x4(
		aiVector3D(1.0f, 1.0f, 1.0f),						// scale
		aiQuaternion(0.0f, 0.0f, 0.0f),						// rotation
		aiVector3D(0.0f, 0.0f, 0.0f));						// transform

	//アニメーションデータからボーンマトリクス算出
	aiAnimation* animation1 = animdata.GetAnimation(animname, 0);

	for (unsigned int c = 0; c < animation1->mNumChannels; c++)
	{
		aiNodeAnim* nodeAnim = animation1->mChannels[c];

		BONE* bone = &m_Bone[nodeAnim->mNodeName.C_Str()];

		int f;

		f = frmae % nodeAnim->mNumRotationKeys;				//簡易実装
		aiQuaternion rot = nodeAnim->mRotationKeys[f].mValue;

		f = frmae % nodeAnim->mNumPositionKeys;				//簡易実装
		aiVector3D pos = nodeAnim->mPositionKeys[f].mValue;

		bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot, pos);
	}

	//再帰的にボーンコンビネーション行列を更新
	UpdateBoneMatrix(m_AiScene->mRootNode, rootMatrix);

	// 定数バッファに書き込むための情報を生成
	m_bonecombmtxcontainer.clear();
	m_bonecombmtxcontainer.resize(m_Bone.size());
	for (auto data : m_Bone) {
		m_bonecombmtxcontainer[data.second.idx] = data.second.Matrix;
	}

	// DirectX用に転置する
	for (auto& bcmtx : m_bonecombmtxcontainer)
	{
		// 転置する
		bcmtx.Transpose();
	}
}
