//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "ModelMeshLoad.h"
#include <SimpleMath.h>
#include <DirectXTex.h>
#include <iostream>		// std::string用
//-----------------------------------------------------------------------------
// 必要な機能
#include "main.h"
#include "renderer.h"
#include "utility.h"

#include "strconv2.h"
//-----------------------------------------------------------------------------
// ロード処理です.
//-----------------------------------------------------------------------------
bool ModelMeshLoad::Load(std::string filename)
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
	}

	// 頂点数表示
	std::cout << "頂点数 : " << m_vertices.size() << std::endl;
	std::cout << std::endl;

	// メッシュのベース情報を構築
	CalcMeshBaseIndex();

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
void ModelMeshLoad::Uninit()
{
	// SRV解放
	for (auto& texres : m_Texturedic)
	{
		if (texres.second != nullptr) 
		{
			texres.second->Release();
		}
	}

	m_vertices.clear();
	m_indices.clear();
	m_materials.clear();
	m_subsets.clear();
}
//-----------------------------------------------------------------------------
// メッシュ単位で頂点データを取得して返す
//-----------------------------------------------------------------------------
unsigned int ModelMeshLoad::GetVertices(const aiMesh* aimesh)
{
	// 頂点数分ループ
	for (unsigned int vidx = 0; vidx < aimesh->mNumVertices; vidx++)
	{

		MODELVERTEX v;

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
unsigned int ModelMeshLoad::GetIndices(const aiMesh* aimesh)
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
void ModelMeshLoad::CalcMeshBaseIndex()
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
void ModelMeshLoad::GetTextures(const aiScene* aiscene)
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

			// mHeight is 0, so try to load a compressed texture of mWidth bytes
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
bool ModelMeshLoad::isEmbeddedTexure(const aiTexture* aitexture)
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
void ModelMeshLoad::GetMaterials(const aiScene* aiscene)
{
	// このモデルで使用しているすべてのマテリアル情報を取得
	for (int mrlidx = 0; mrlidx < aiscene->mNumMaterials; mrlidx++) {

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
void ModelMeshLoad::GetaMatreial(const aiMaterial* aimtrl)
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
ID3D11ShaderResourceView* ModelMeshLoad::LoadDiffuseTexture(aiString _path)
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
